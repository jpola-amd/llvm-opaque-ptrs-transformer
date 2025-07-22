#include "include/error_handler.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace llvm_transformer {

std::string ErrorInfo::toString() const {
    std::ostringstream oss;
    oss << "[" << ErrorHandler::severityToString(severity) << "] "
        << ErrorHandler::errorTypeToString(type) << ": " << message;
    
    if (line_number.has_value()) {
        oss << " (line " << line_number.value();
        if (column_number.has_value()) {
            oss << ", column " << column_number.value();
        }
        oss << ")";
    }
    
    if (!details.empty()) {
        oss << "\n  Details: " << details;
    }
    
    return oss.str();
}

void ErrorHandler::addError(ErrorType type, const std::string& message, const std::string& details) {
    addErrorInfo(type, ErrorSeverity::S_ERROR, message, details);
}

void ErrorHandler::addWarning(ErrorType type, const std::string& message, const std::string& details) {
    addErrorInfo(type, ErrorSeverity::S_WARNING, message, details);
}

void ErrorHandler::addFatal(ErrorType type, const std::string& message, const std::string& details) {
    addErrorInfo(type, ErrorSeverity::S_FATAL, message, details);
}

void ErrorHandler::addError(ErrorType type, const std::string& message, size_t line, size_t column, const std::string& details) {
    addErrorInfo(type, ErrorSeverity::S_ERROR, message, details, line, column);
}

void ErrorHandler::addWarning(ErrorType type, const std::string& message, size_t line, size_t column, const std::string& details) {
    addErrorInfo(type, ErrorSeverity::S_WARNING, message, details, line, column);
}

bool ErrorHandler::hasErrors() const {
    return std::any_of(errors_.begin(), errors_.end(), 
        [](const ErrorInfo& info) { return info.severity == ErrorSeverity::S_ERROR; });
}

bool ErrorHandler::hasWarnings() const {
    return std::any_of(errors_.begin(), errors_.end(), 
        [](const ErrorInfo& info) { return info.severity == ErrorSeverity::S_WARNING; });
}

bool ErrorHandler::hasFatalErrors() const {
    return std::any_of(errors_.begin(), errors_.end(), 
        [](const ErrorInfo& info) { return info.severity == ErrorSeverity::S_FATAL; });
}

bool ErrorHandler::hasAnyIssues() const {
    return !errors_.empty();
}

size_t ErrorHandler::getErrorCount() const {
    return std::count_if(errors_.begin(), errors_.end(), 
        [](const ErrorInfo& info) { return info.severity == ErrorSeverity::S_ERROR; });
}

size_t ErrorHandler::getWarningCount() const {
    return std::count_if(errors_.begin(), errors_.end(), 
        [](const ErrorInfo& info) { return info.severity == ErrorSeverity::S_WARNING; });
}

size_t ErrorHandler::getFatalErrorCount() const {
    return std::count_if(errors_.begin(), errors_.end(), 
        [](const ErrorInfo& info) { return info.severity == ErrorSeverity::S_FATAL; });
}

std::vector<ErrorInfo> ErrorHandler::getErrorsByType(ErrorType type) const {
    std::vector<ErrorInfo> result;
    std::copy_if(errors_.begin(), errors_.end(), std::back_inserter(result),
        [type](const ErrorInfo& info) { return info.type == type; });
    return result;
}

std::vector<ErrorInfo> ErrorHandler::getErrorsBySeverity(ErrorSeverity severity) const {
    std::vector<ErrorInfo> result;
    std::copy_if(errors_.begin(), errors_.end(), std::back_inserter(result),
        [severity](const ErrorInfo& info) { return info.severity == severity; });
    return result;
}

std::string ErrorHandler::getFormattedErrors() const {
    std::ostringstream oss;
    for (const auto& error : errors_) {
        if (error.severity == ErrorSeverity::S_ERROR || error.severity == ErrorSeverity::S_FATAL) {
            oss << error.toString() << "\n";
        }
    }
    return oss.str();
}

std::string ErrorHandler::getFormattedWarnings() const {
    std::ostringstream oss;
    for (const auto& error : errors_) {
        if (error.severity == ErrorSeverity::S_WARNING) {
            oss << error.toString() << "\n";
        }
    }
    return oss.str();
}

std::string ErrorHandler::getFormattedAll() const {
    std::ostringstream oss;
    for (const auto& error : errors_) {
        oss << error.toString() << "\n";
    }
    return oss.str();
}

void ErrorHandler::printErrors() const {
    std::cerr << getFormattedErrors();
}

void ErrorHandler::printWarnings() const {
    std::cout << getFormattedWarnings();
}

void ErrorHandler::printAll() const {
    std::cerr << getFormattedAll();
}

void ErrorHandler::clear() {
    errors_.clear();
}

void ErrorHandler::clearWarnings() {
    errors_.erase(std::remove_if(errors_.begin(), errors_.end(),
        [](const ErrorInfo& info) { return info.severity == ErrorSeverity::S_WARNING; }),
        errors_.end());
}

void ErrorHandler::clearErrors() {
    errors_.erase(std::remove_if(errors_.begin(), errors_.end(),
        [](const ErrorInfo& info) { return info.severity != ErrorSeverity::S_WARNING; }),
        errors_.end());
}

std::string ErrorHandler::errorTypeToString(ErrorType type) {
    switch (type) {
        case ErrorType::PARSING_ERROR: return "Parsing Error";
        case ErrorType::VALIDATION_ERROR: return "Validation Error";
        case ErrorType::TRANSFORMATION_ERROR: return "Transformation Error";
        case ErrorType::IO_ERROR: return "I/O Error";
        case ErrorType::INTERNAL_ERROR: return "Internal Error";
        default: return "Unknown Error";
    }
}

std::string ErrorHandler::severityToString(ErrorSeverity severity) {
    switch (severity) {
        case ErrorSeverity::S_WARNING: return "WARNING";
        case ErrorSeverity::S_ERROR: return "ERROR";
        case ErrorSeverity::S_FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

void ErrorHandler::addErrorInfo(ErrorType type, ErrorSeverity severity, const std::string& message, 
                                const std::string& details, 
                                std::optional<size_t> line,
                                std::optional<size_t> column) {
    errors_.emplace_back(type, severity, message, details, line, column);
}

} // namespace llvm_transformer
