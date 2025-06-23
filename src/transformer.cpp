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
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
#include <string_view>
#include <memory>

namespace llvm_transformer
{

    std::unique_ptr<llvm::Module> parseIRFromStringView(std::string_view ir_data, llvm::LLVMContext& context, ErrorHandler& error_handler)
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

    Result<ModuleValidationResult> validateModuleWithCode(llvm::Module* module)
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

    Result<std::string> transform_llvm_ir_to_opaque_pointers(const std::string_view& input_ir)
    {
       // Set opaque pointers context
        llvm::LLVMContext context;
        context.setOpaquePointers(true);
        
        ErrorHandler error_handler;
        
        if (input_ir.empty()) {
            error_handler.addError(ErrorType::PARSING_ERROR, "Input IR is empty");
            return Result<std::string>(std::move(error_handler));
        }
        

        // Parse the input
        auto module = parseIRFromStringView(input_ir, context, error_handler);
        
        if (!module) {
            return Result<std::string>(std::move(error_handler));
        }
        
        // Validate the module
        auto validation_result = validateModuleWithCode(module.get());
        if (!validation_result.hasValue()) {
            return Result<std::string>(validation_result.getErrorHandler());
        }
        
        std::string output;
        llvm::raw_string_ostream output_stream(output);
        module->print(output_stream, nullptr);
        
        return Result<std::string>(std::move(output));
    }

}