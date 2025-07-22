#include "transformer.h"
#include "error_handler.h"
#include <iostream>

using namespace llvm_transformer;

int main() {
    // Example 1: Valid LLVM IR
    std::string_view valid_ir = R"(
define i32 @test_func(i32* %arg) {
entry:
  %loaded = load i32, i32* %arg
  ret i32 %loaded
}
)";
    
    std::cout << "=== Testing with valid LLVM IR ===\n";
    auto result = transform_llvm_ir_to_opaque_pointers(valid_ir);
    
    if (result.hasValue()) {
        std::cout << "SUCCESS: Transformation completed!\n";
        std::cout << "Output length: " << result.getValue().as_text().size() << " characters\n";
        
        // Print warnings if any
        if (result.getErrorHandler().hasWarnings()) {
            std::cout << "Warnings:\n" << result.getErrorHandler().getFormattedWarnings();
        }
    } else {
        std::cout << "FAILED: Transformation failed\n";
        result.getErrorHandler().printAll();
    }
    
    // Example 2: Invalid LLVM IR
    std::string_view invalid_ir = "invalid llvm ir code here";
    
    std::cout << "\n=== Testing with invalid LLVM IR ===\n";
    auto result2 = transform_llvm_ir_to_opaque_pointers(invalid_ir);
    
    if (result2.hasValue()) {
        std::cout << "SUCCESS: Transformation completed!\n";
    } else {
        std::cout << "FAILED: Transformation failed (as expected)\n";
        result2.getErrorHandler().printAll();
    }
    
    // Example 3: Empty input
    std::string_view empty_ir = "";
    
    std::cout << "\n=== Testing with empty input ===\n";
    auto result3 = transform_llvm_ir_to_opaque_pointers(empty_ir);
    
    if (result3.hasValue()) {
        std::cout << "SUCCESS: Transformation completed!\n";
    } else {
        std::cout << "FAILED: Transformation failed (as expected)\n";
        result3.getErrorHandler().printAll();
    }
    return 0;
}
