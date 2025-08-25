/*
The goal of this code is to load the llvm-ir file in a format
that does not use opaque pointers and transform it to use opaque pointers.

The input is a string view of the llvm-ir file, and the output is a new string transformed to use opaque pointers.
*/

#include "include/transformer.h"
#include "include/error_handler.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>

#include <string_view>
#include <memory>

namespace llvm_transformer
{

    inline std::string getAMDGCNTargetFeatures(AMDGCNTarget target) {
        switch (target) {
            case AMDGCNTarget::GFX1030: return "+gfx1030";
            case AMDGCNTarget::GFX1100: return "+gfx1100";
            case AMDGCNTarget::GFX1101: return "+gfx1101";
            case AMDGCNTarget::GFX1102: return "+gfx1102";
            case AMDGCNTarget::GFX1151: return "+gfx1151";
            case AMDGCNTarget::GFX1200: return "+gfx1200";
            case AMDGCNTarget::GFX1201: return "+gfx1201";
            case AMDGCNTarget::GENERIC:
            default:
                return "+gfx1030"; // Default to a common target
        }
    }

    inline std::string getAMDGCNTargetTriple() {
        return "amdgcn-amd-amdhsa";
    }

    inline const char* getAMDGCNDataLayout() {
        return "e-p:64:64-p1:64:64-p2:32:32-p3:32:32-p4:64:64-p5:32:32-p6:32:32-p7:160:256:256:32-p8:128:128-p9:192:256:256:32-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-v2048:2048-n32:64-S32-A5-G1-ni:7:8:9";

    }

    void overrideWcharSizeFlag(llvm::Module* module, size_t wchar_size = 2) {
        if (!module) return;

        llvm::LLVMContext& ctx = module->getContext();

        // Remove existing wchar_size module flags
        llvm::NamedMDNode* flags = module->getModuleFlagsMetadata();
        if (flags) {
            std::vector<llvm::MDNode*> keep; // which one to keep in module
            for (unsigned i = 0; i < flags->getNumOperands(); ++i) {
                llvm::MDNode* flag = flags->getOperand(i);
                if (flag->getNumOperands() >= 2) {
                    if (auto* str_md = llvm::dyn_cast<llvm::MDString>(flag->getOperand(1))) {
                        if (str_md->getString() == "wchar_size") { // Found wchar_size flag
                            continue;
                        }
                    }
                }
                keep.push_back(flag); // keep all others
            }
            flags->clearOperands();
            for (auto* flag : keep) {
                flags->addOperand(flag);
            }
        }

        // Add the new wchar_size flag
        module->addModuleFlag(llvm::Module::Error, "wchar_size",
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), wchar_size));
    }

    static void cleanModuleForAMDGCN(llvm::Module* module, const TransformOptions& options) {
        if (!module) return;
        
        // 1. Remove NVIDIA-specific metadata
        if (auto nvvm_annotations = module->getNamedMetadata("nvvm.annotations")) {
            module->eraseNamedMetadata(nvvm_annotations);
        }
        
        if (auto nvvmir_version = module->getNamedMetadata("nvvmir.version")) {
            module->eraseNamedMetadata(nvvmir_version);
        }
        
        // 2. Set AMDGCN target triple
        if (!options.target_triple.empty()) {
            module->setTargetTriple(options.target_triple);
        } else {
            module->setTargetTriple(getAMDGCNTargetTriple());
        }

        module->setDataLayout(getAMDGCNDataLayout());
        
        // 3. Clean and update function attributes
        for (auto& function : *module) {
            if (function.isDeclaration()) continue;
            
            // Remove PTX-specific attributes
            function.removeFnAttr("target-features");
            
            // TEST This
            function.removeFnAttr("disable-tail-calls");
            function.removeFnAttr("frame-pointer");
            function.removeFnAttr("less-precise-fpmad");
            function.removeFnAttr("no-infs-fp-math");
            function.removeFnAttr("no-jump-tables");
            function.removeFnAttr("no-nans-fp-math");
            function.removeFnAttr("no-signed-zeros-fp-math");
            function.removeFnAttr("unsafe-fp-math");
            function.removeFnAttr("use-soft-float");
            function.removeFnAttr("no-trapping-math");
            function.removeFnAttr("stack-protector-buffer-size");
            
            // Add AMDGCN target features
            std::string target_features = options.target_features;
            if (target_features.empty()) {
                target_features = getAMDGCNTargetFeatures(options.amdgcn_target);
            }
            function.addFnAttr("target-features", target_features);
            
            // Convert main kernel function to amdgpu_kernel
            if (function.getName() == options.kernel_function_name) {
                // Change calling convention to AMDGPU kernel
                // function.setCallingConv(llvm::CallingConv::AMDGPU_KERNEL);
                
                // Add kernel-specific attributes
                //function.addFnAttr("amdgpu-flat-work-group-size", "1,256");
                function.addFnAttr("target-cpu", 
                    getAMDGCNTargetFeatures(options.amdgcn_target).substr(1)); // Remove '+'
                
                // Remove function attributes that don't make sense for kernels
                function.removeFnAttr(llvm::Attribute::AlwaysInline);
                
                // Ensure proper linkage for kernel
                if (function.hasInternalLinkage()) {
                    function.setLinkage(llvm::GlobalValue::ExternalLinkage);
                }
            }
        }
        
        // 4. Clean up compiler identification metadata
        if (options.remove_compiler_info) {
            if (auto llvm_ident = module->getNamedMetadata("llvm.ident")) {
                module->eraseNamedMetadata(llvm_ident);
            }
        }
    }

    static void cleanModuleForTargetIndependence(llvm::Module* module, const TransformOptions& options = {}) {
        if (!module) return;
        
        // 1. Remove NVIDIA-specific metadata
        if (auto nvvm_annotations = module->getNamedMetadata("nvvm.annotations")) {
            module->eraseNamedMetadata(nvvm_annotations);
        }
        
        if (auto nvvmir_version = module->getNamedMetadata("nvvmir.version")) {
            module->eraseNamedMetadata(nvvmir_version);
        }
        
        // 2. Clean target-specific function attributes
        for (auto& function : *module) {
            if (function.isDeclaration()) continue;
            
            // Remove PTX-specific attributes
            function.removeFnAttr("target-features");
        }
        
        // 3. Optionally neutralize target triple for portability
        if (options.neutralize_target) {
            module->setTargetTriple(""); // or a generic triple
            module->setDataLayout("");
        }
        
        // 4. Clean up compiler identification metadata
        if (options.remove_compiler_info) {
            if (auto llvm_ident = module->getNamedMetadata("llvm.ident")) {
                module->eraseNamedMetadata(llvm_ident);
            }
        }
    }

    static std::unique_ptr<llvm::Module> parseIRFromStringView(std::string_view ir_data, llvm::LLVMContext& context, ErrorHandler& error_handler)
    {
        // Create a memory buffer from the string_view
        auto memory_buffer = llvm::MemoryBuffer::getMemBuffer(
            llvm::StringRef(ir_data.data(), ir_data.size()),
            "IR_Input",
            false // Don't require null terminator
        );

        // Try to parse as LLVM IR or bitcode
        llvm::SMDiagnostic parse_error;
        auto module = llvm::parseIR(*memory_buffer, parse_error, context);
        if (!module)
        {
            error_handler.addError(ErrorType::PARSING_ERROR, 
                                 "Failed to parse LLVM IR",
                                 parse_error.getMessage().str());
            return nullptr;
        }

        return module;
    }

    static Result<ModuleValidationResult> validateModuleWithCode(llvm::Module* module)
    {
        ErrorHandler error_handler;
        ModuleValidationResult validation_result;
        
        if (!module) {
            error_handler.addError(ErrorType::VALIDATION_ERROR, "Module is null");
            return Result<ModuleValidationResult>(std::move(error_handler));
        }
        
        // Run LLVM verifier
        std::string error_msg;
        llvm::raw_string_ostream error_stream(error_msg);
        if (llvm::verifyModule(*module, &error_stream)) {
            error_handler.addError(ErrorType::VALIDATION_ERROR, 
                                  "Module verification failed", 
                                  error_msg);
            return Result<ModuleValidationResult>(std::move(error_handler));
        }
        
        // Count functions and analyze content
        validation_result.function_count = module->size();
        
        for (const auto& function : *module) {
            if (!function.empty()) {
                validation_result.functions_with_body++;
                
                // Count instructions
                for (const auto& bb : function) {
                    validation_result.total_instructions += bb.size();
                }
            }
        }
        
        // Check for global variables
        validation_result.has_global_variables = !module->global_empty();
        
        // Validation criteria
        if (validation_result.function_count == 0 && !validation_result.has_global_variables) {
            error_handler.addError(ErrorType::VALIDATION_ERROR, 
                                  "Module contains no functions or global variables");
            return Result<ModuleValidationResult>(std::move(error_handler));
        }
        
        if (validation_result.functions_with_body == 0 && !validation_result.has_global_variables) {
            error_handler.addError(ErrorType::VALIDATION_ERROR, 
                                  "Module contains only function declarations and no global variables");
            return Result<ModuleValidationResult>(std::move(error_handler));
        }
        
        if (validation_result.total_instructions < 2) {
            error_handler.addWarning(ErrorType::VALIDATION_ERROR, 
                                    "Module contains very few instructions",
                                    "Total instructions: " + std::to_string(validation_result.total_instructions));
        }
        
        validation_result.is_valid = true;
        return Result<ModuleValidationResult>(std::move(validation_result));
    }

    /*
    // The following functions are for inserting AMD GPU printf calls
    // They mimic the __ockl_printf_* functions used in AMD GPU assembly.
    
    declare i64 @__ockl_printf_begin(i64)

    declare i64 @__ockl_printf_append_string_n(i64, ptr, i64, i32)

    declare i64 @__ockl_printf_append_args(i64, i32, i64, i64, i64, i64, i64, i64, i64, i32)
    */

    static void insertAMDGPUPrintf(llvm::IRBuilder<>& builder, llvm::Module* module, 
                                   const std::string& format_str, llvm::Value* int_arg) {
        llvm::LLVMContext& context = module->getContext();
        
        // Get or create __ockl_printf_* function declarations
        llvm::FunctionType* printf_begin_type = llvm::FunctionType::get(
            llvm::Type::getInt64Ty(context), 
            {llvm::Type::getInt64Ty(context)}, 
            false
        );
        
        llvm::Function* printf_begin_func = module->getFunction("__ockl_printf_begin");
        if (!printf_begin_func) {
            printf_begin_func = llvm::Function::Create(
                printf_begin_type, 
                llvm::Function::ExternalLinkage, 
                "__ockl_printf_begin", 
                *module
            );
        }

        llvm::FunctionType* printf_append_string_type = llvm::FunctionType::get(
            llvm::Type::getInt64Ty(context),
            {llvm::Type::getInt64Ty(context), llvm::Type::getInt8PtrTy(context), 
             llvm::Type::getInt64Ty(context), llvm::Type::getInt32Ty(context)},
            false
        );
        
        llvm::Function* printf_append_string_func = module->getFunction("__ockl_printf_append_string_n");
        if (!printf_append_string_func) {
            printf_append_string_func = llvm::Function::Create(
                printf_append_string_type,
                llvm::Function::ExternalLinkage,
                "__ockl_printf_append_string_n",
                *module
            );
        }

        llvm::FunctionType* printf_append_args_type = llvm::FunctionType::get(
            llvm::Type::getInt64Ty(context),
            {llvm::Type::getInt64Ty(context), llvm::Type::getInt32Ty(context),
             llvm::Type::getInt64Ty(context), llvm::Type::getInt64Ty(context),
             llvm::Type::getInt64Ty(context), llvm::Type::getInt64Ty(context),
             llvm::Type::getInt64Ty(context), llvm::Type::getInt64Ty(context),
             llvm::Type::getInt64Ty(context), llvm::Type::getInt32Ty(context)},
            false
        );
        
        llvm::Function* printf_append_args_func = module->getFunction("__ockl_printf_append_args");
        if (!printf_append_args_func) {
            printf_append_args_func = llvm::Function::Create(
                printf_append_args_type,
                llvm::Function::ExternalLinkage,
                "__ockl_printf_append_args",
                *module
            );
        }

        // Create global string constant in addrspace(4) - like @.str in device_code.ll
        llvm::ArrayType* string_type = llvm::ArrayType::get(
            llvm::Type::getInt8Ty(context), 
            format_str.length() + 1
        );
        llvm::Constant* string_constant = llvm::ConstantDataArray::getString(context, format_str, true);
        
        llvm::GlobalVariable* global_string = new llvm::GlobalVariable(
            *module,
            string_type,
            true, // isConstant
            llvm::GlobalValue::PrivateLinkage,
            string_constant,
            ".str.debug",
            nullptr,
            llvm::GlobalVariable::NotThreadLocal,
            4 // addrspace(4) - same as @.str in device_code.ll
        );
        global_string->setAlignment(llvm::Align(1));
        global_string->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);

        // Step 1: Begin printf - call __ockl_printf_begin(i64 0)
        llvm::Value* printf_handle = builder.CreateCall(printf_begin_func, {builder.getInt64(0)});
        
        // Step 2: Calculate string length and create addrspacecast
        // This mimics: addrspacecast (ptr addrspace(4) @.str to ptr)
        llvm::Value* string_ptr = builder.CreateAddrSpaceCast(
            global_string, 
            llvm::Type::getInt8PtrTy(context)
        );
        
        // String length calculation (simpler version than device_code.ll's loop)
        llvm::Value* string_len = builder.getInt64(format_str.length());
        
        // Step 3: Append string - call __ockl_printf_append_string_n
        llvm::Value* printf_handle2 = builder.CreateCall(printf_append_string_func, {
            printf_handle, 
            string_ptr, 
            string_len, 
            builder.getInt32(0)
        });
        
        // Step 4: Append integer argument - call __ockl_printf_append_args
        llvm::Value* int_arg_64 = builder.CreateZExt(int_arg, llvm::Type::getInt64Ty(context));
        builder.CreateCall(printf_append_args_func, {
            printf_handle2,
            builder.getInt32(1), // number of args
            int_arg_64,          // first argument (zext i32 to i64)
            builder.getInt64(0), // remaining 7 args are 0
            builder.getInt64(0),
            builder.getInt64(0),
            builder.getInt64(0),
            builder.getInt64(0),
            builder.getInt64(0),
            builder.getInt32(1)  // final i32 parameter
        });
    }

    static void enableDebugSwitchStatements(llvm::Module* module, const std::string& kernel_function_name) {
        if (!module) return;

        if (kernel_function_name.empty()) {
            llvm::errs() << "Kernel function name is empty, cannot enable debug switch statements.\n";
            return;
        }

        // find the desired function
        llvm::Function* kernel_function = module->getFunction(kernel_function_name);
        if (!kernel_function) {
            llvm::errs() << "Kernel function '" << kernel_function_name << "' not found in module.\n";
            return;
        }

        // Get or create printf function declaration
        llvm::LLVMContext& context = module->getContext();
        llvm::FunctionType* printf_type = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context), 
            llvm::Type::getInt8PtrTy(context), 
            true // varargs
        );
        
        llvm::Function* printf_func = module->getFunction("printf");
        if (!printf_func) {
            printf_func = llvm::Function::Create(
                printf_type, 
                llvm::Function::ExternalLinkage, 
                "printf", 
                *module
            );
        }

        // Find the single switch statement in the function
        llvm::SwitchInst* switch_inst = nullptr;
        for (auto& BB : *kernel_function) {
            for (auto& I : BB) {
                if (auto* SI = llvm::dyn_cast<llvm::SwitchInst>(&I)) {
                    switch_inst = SI;
                    break;
                }
            }
            if (switch_inst) break;
        }
        
        if (!switch_inst) {
            llvm::outs() << "No switch statement found in function '" << kernel_function_name << "'\n";
            return;
        }
        
        // Create IRBuilder positioned before the switch instruction
        llvm::IRBuilder<> builder(switch_inst);
        
        // Create format string with case values for better debugging
        std::string format_str = "Switch condition: %d (cases: ";
        for (auto case_it = switch_inst->case_begin(); case_it != switch_inst->case_end(); ++case_it) {
            int64_t case_value = case_it->getCaseValue()->getSExtValue();
            format_str += std::to_string(case_value) + ",";
        }
        format_str += "default)\\n";
        
        llvm::Value* format_string = builder.CreateGlobalStringPtr(format_str);
        
        // Get the switch condition value and insert printf call
        llvm::Value* switch_condition = switch_inst->getCondition();
        builder.CreateCall(printf_func, {format_string, switch_condition});
        
        llvm::outs() << "Successfully added debug printf for switch statement in function '" 
                     << kernel_function_name << "'\n";
    }

    static inline Result<TransformResult> transform(const std::string_view& input_ir, llvm::LLVMContext& context, ErrorHandler& error_handler, const TransformOptions& options = {})
    {       
        if (input_ir.empty()) {
            error_handler.addError(ErrorType::PARSING_ERROR, "Input IR is empty");
            return Result<TransformResult>(std::move(error_handler));
        }
        
        // Parse the input
        auto module = parseIRFromStringView(input_ir, context, error_handler);
        
        if (!module) {
            return Result<TransformResult>(std::move(error_handler));
        }
        
        // Validate the module
        auto validation_result = validateModuleWithCode(module.get());
        if (!validation_result.hasValue()) {
            return Result<TransformResult>(validation_result.getErrorHandler());
        }
        
        // Apply transformations based on options
        if (options.amdgcn_target != AMDGCNTarget::GENERIC || !options.target_triple.empty()) {
            cleanModuleForAMDGCN(module.get(), options);
        } else {
            cleanModuleForTargetIndependence(module.get(), options);
        }

        if (options.debug_switch_statements) {
            enableDebugSwitchStatements(module.get(), options.kernel_function_name);
        }

        overrideWcharSizeFlag(module.get(), 2);

        TransformResult result;
        
        if (options.output_bitcode) {
            // If bitcode output is requested, write to a binary format
            llvm::SmallVector<char> bitcode_data;
            llvm::raw_svector_ostream bitcode_stream(bitcode_data);
            llvm::WriteBitcodeToFile(*module, bitcode_stream);

            result.data = std::vector<uint8_t>(bitcode_data.begin(), bitcode_data.end());
            result.format = OutputFormat::BITCODE;
        }
        else {
            std::string text_ir;
            llvm::raw_string_ostream output_stream(text_ir);
            module->print(output_stream, nullptr);
        
            result.data = std::move(text_ir);
            result.format = OutputFormat::TEXT_IR;
        }
        
         return Result<TransformResult>(std::move(result));
    }

    Result<TransformResult> transform_llvm_ir_to_opaque_pointers(const std::string_view& input_ir, const TransformOptions& options)
    {
       // Set opaque pointers context
        llvm::LLVMContext context;
        context.setOpaquePointers(true);
        
        ErrorHandler error_handler;

        return transform(input_ir, context, error_handler, options);
    }
}