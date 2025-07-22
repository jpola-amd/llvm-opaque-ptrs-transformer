#include "transformer.h"
#include "error_handler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace llvm_transformer;


std::string load_file(const std::filesystem::path& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + file_path.string());
    }

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void save_result_to_file(const std::filesystem::path& file_path, const TransformResult& content) {

    auto mode = std::ios::out | std::ios::trunc;
    if (content.is_bitcode()) {
        mode |= std::ios::binary;
    }

    std::ofstream file(file_path, mode);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + file_path.string());
    }

    if (content.is_text()) {
        file << content.as_text();
    } else if (content.is_bitcode()) {
        file.write(reinterpret_cast<const char*>(content.as_bitcode().data()), content.as_bitcode().size());
    } else {
        throw std::runtime_error("Unsupported content format for saving");
    }
    
}
int main(int argc, char* argv[])
{

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return EXIT_FAILURE;
    }
    std::cout << "LLVM IR to Opaque Pointers Transformer\n";
    std::cout << "---------------------------------------\n";
    std::cout << "CWD: " << std::filesystem::current_path() << "\n";
    std::filesystem::path input_file = argv[1];
    std::filesystem::path output_file = argv[2];

    std::cout << "Input file: " << input_file << "\n";
    std::cout << "Output file: " << output_file << "\n";
    
    try {
        // Load the input file
        std::string input_content = load_file(input_file);
        
        // Transform the LLVM IR to opaque pointers
        TransformOptions options;
        options.neutralize_target = true; // Example option, adjust as needed
        options.remove_compiler_info = true; // Example option, adjust as needed 
        options.target_triple = "amdgcn-amd-amdhsa"; // Set target triple for AMDGCN
        options.amdgcn_target = AMDGCNTarget::GFX1030; // Set specific AMDGCN target
        options.output_bitcode = true; // Set to true if you want to output bitcode instead of text IR
        
        auto result = transform_llvm_ir_to_opaque_pointers(input_content, options);
        
        if (result.hasValue()) {
            save_result_to_file(output_file, result.getValue());
            std::cout << "Transformation completed successfully!\n";
            
            // Print warnings if any
            if (result.getErrorHandler().hasWarnings()) {
                std::cout << "Warnings:\n" << result.getErrorHandler().getFormattedWarnings();
            }
        } else {
            std::cerr << "Transformation failed:\n";
            result.getErrorHandler().printAll();
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}