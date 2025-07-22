//////////////////////////////////////////////////////////////////////////////////////////
// 
//  Copyright (c) 2025 Advanced Micro Devices, Inc. All Rights Reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
#include <string>
#include <variant>
#include <vector>
#include "error_handler.h"

// Forward declarations
// namespace llvm {
//     class Module;
// }

namespace llvm_transformer {

    enum class OutputFormat {
        TEXT_IR,
        BITCODE
    };

    struct TransformResult {
        std::variant<std::string, std::vector<uint8_t>> data;
        OutputFormat format;
    
        // Convenience methods
        bool is_text() const { return format == OutputFormat::TEXT_IR; }
        bool is_bitcode() const { return format == OutputFormat::BITCODE; }
        
        const std::string& as_text() const { 
            return std::get<std::string>(data); 
        }
        
        const std::vector<uint8_t>& as_bitcode() const { 
            return std::get<std::vector<uint8_t>>(data); 
        }
        
        size_t size() const {
            return std::visit([](const auto& d) { return d.size(); }, data);
        }
    };

    enum class AMDGCNTarget {
        GFX1100,
        GFX1101,
        GFX1102,
        GFX1030,
        GENERIC,
        UNKNOWN
    };

    struct TransformOptions {
        bool neutralize_target = false;
        bool remove_compiler_info = false;
        std::string target_triple = "";
        std::string target_features = "";
        AMDGCNTarget amdgcn_target = AMDGCNTarget::GENERIC;
        std::string kernel_function_name = "evalGLSL";
        bool output_bitcode = false; // <-- Add this line
    };
   
    /**
     * Transform LLVM IR with typed pointers to use opaque pointers (with error handling).
     * 
     * @param input_ir The input LLVM IR code as a string view (can be text IR or bitcode)
     * @return Result containing the transformed IR or error information
     */
    Result<TransformResult> transform_llvm_ir_to_opaque_pointers(const std::string_view& input_ir, const TransformOptions& options = {}
    );
    
}
