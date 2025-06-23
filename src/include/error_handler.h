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

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace llvm_transformer {

enum class ErrorType {
    PARSING_ERROR,
    VALIDATION_ERROR,
    TRANSFORMATION_ERROR,
    IO_ERROR,
    INTERNAL_ERROR
};

enum class ErrorSeverity {
    WARNING,
    ERROR,
    FATAL
};

struct ErrorInfo {
    ErrorType type;
    ErrorSeverity severity;
    std::string message;
    std::string details;
    std::optional<size_t> line_number;
    std::optional<size_t> column_number;
    
    ErrorInfo(ErrorType t, ErrorSeverity s, const std::string& msg, 
              const std::string& det = "", 
              std::optional<size_t> line = std::nullopt,
              std::optional<size_t> col = std::nullopt)
        : type(t), severity(s), message(msg), details(det), line_number(line), column_number(col) {}
    
    std::string toString() const;
};

class ErrorHandler {
public:
    ErrorHandler() = default;
    ~ErrorHandler() = default;
    
    // Add errors
    void addError(ErrorType type, const std::string& message, const std::string& details = "");
    void addWarning(ErrorType type, const std::string& message, const std::string& details = "");
    void addFatal(ErrorType type, const std::string& message, const std::string& details = "");
    
    // Add errors with location info
    void addError(ErrorType type, const std::string& message, size_t line, size_t column = 0, const std::string& details = "");
    void addWarning(ErrorType type, const std::string& message, size_t line, size_t column = 0, const std::string& details = "");
    
    // Query methods
    bool hasErrors() const;
    bool hasWarnings() const;
    bool hasFatalErrors() const;
    bool hasAnyIssues() const;
    
    size_t getErrorCount() const;
    size_t getWarningCount() const;
    size_t getFatalErrorCount() const;
    
    // Get errors
    const std::vector<ErrorInfo>& getAllErrors() const { return errors_; }
    std::vector<ErrorInfo> getErrorsByType(ErrorType type) const;
    std::vector<ErrorInfo> getErrorsBySeverity(ErrorSeverity severity) const;
    
    // Output methods
    std::string getFormattedErrors() const;
    std::string getFormattedWarnings() const;
    std::string getFormattedAll() const;
    
    void printErrors() const;
    void printWarnings() const;
    void printAll() const;
    
    // Clear errors
    void clear();
    void clearWarnings();
    void clearErrors();
    
    // Static helper methods
    static std::string errorTypeToString(ErrorType type);
    static std::string severityToString(ErrorSeverity severity);

private:
    std::vector<ErrorInfo> errors_;
    
    void addErrorInfo(ErrorType type, ErrorSeverity severity, const std::string& message, 
                      const std::string& details = "", 
                      std::optional<size_t> line = std::nullopt,
                      std::optional<size_t> column = std::nullopt);
};

// Result wrapper that includes error handling
template<typename T>
class Result {
public:
    Result(T&& value) : value_(std::move(value)), has_value_(true) {}
    Result(const T& value) : value_(value), has_value_(true) {}
    Result(ErrorHandler&& error_handler) : error_handler_(std::move(error_handler)), has_value_(false) {}
    Result(const ErrorHandler& error_handler) : error_handler_(error_handler), has_value_(false) {}
    
    bool hasValue() const { return has_value_; }
    bool hasError() const { return !has_value_; }
    
    const T& getValue() const { 
        if (!has_value_) throw std::runtime_error("Attempting to get value from failed result");
        return value_; 
    }
    
    T& getValue() { 
        if (!has_value_) throw std::runtime_error("Attempting to get value from failed result");
        return value_; 
    }
    
    const ErrorHandler& getErrorHandler() const { return error_handler_; }
    ErrorHandler& getErrorHandler() { return error_handler_; }
    
    // Convenience methods
    operator bool() const { return has_value_; }
    const T& operator*() const { return getValue(); }
    T& operator*() { return getValue(); }

private:
    T value_{};
    ErrorHandler error_handler_;
    bool has_value_;
};

// Validation result structure
struct ModuleValidationResult {
    bool is_valid = false;
    std::string error_message;
    size_t function_count = 0;
    size_t functions_with_body = 0;
    size_t total_instructions = 0;
    bool has_global_variables = false;
};

} // namespace llvm_transformer
