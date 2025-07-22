/*
The goal of this code is to load the llvm-ir file in a format
that does not use opaque pointers and transform it to use opaque pointers.

The input is a string view of the llvm-ir file, and the output is a new string transformed to use opaque pointers.
*/

#include "include/transformer.h"
#include "include/error_handler.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
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
    
/*    static void cleanModuleForTargetIndependence(llvm::Module* module, const TransformOptions& options = {}) {
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
        
        // 3. Clean target-specific function attributes
        for (auto& function : *module) {
            if (function.isDeclaration()) continue;
            
            // Remove PTX-specific attributes
            function.removeFnAttr("target-features");
            
            // // Remove other target-specific attributes
            // function.removeFnAttr("disable-tail-calls");
            // function.removeFnAttr("frame-pointer");
            // function.removeFnAttr("less-precise-fpmad");
            // function.removeFnAttr("no-infs-fp-math");
            // function.removeFnAttr("no-jump-tables");
            // function.removeFnAttr("no-nans-fp-math");
            // function.removeFnAttr("no-signed-zeros-fp-math");
            // function.removeFnAttr("unsafe-fp-math");
            // function.removeFnAttr("use-soft-float");
            
            // Keep important optimization attributes
            // function.addFnAttr(llvm::Attribute::AlwaysInline); // if needed

            std::string target_features = options.target_features;
            if (target_features.empty()) {
                target_features = getAMDGCNTargetFeatures(options.target);
            }
            function.addFnAttr("target-features", target_features);
        

            // Convert main kernel function to amdgpu_kernel
            if (function.getName() == options.kernel_function_name) {
                // Change calling convention to AMDGPU kernel
                function.setCallingConv(llvm::CallingConv::AMDGPU_KERNEL);
                
                // Add kernel-specific attributes
                //function.addFnAttr("amdgpu-flat-work-group-size", "1,256");
                function.addFnAttr("target-cpu", 
                    getAMDGCNTargetFeatures(options.target).substr(1)); // Remove '+'
                
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
*/
    
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