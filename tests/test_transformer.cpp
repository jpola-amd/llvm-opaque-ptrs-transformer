#include <gtest/gtest.h>
#include <string_view>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>

// Include your transformer header
#include "transformer.h"
#include "error_handler.h"

using namespace llvm_transformer;

std::string load_test_resource(const std::string& filename) {
    // Load the test resource from the resources directory
    std::cerr << "PWD: " << std::filesystem::current_path() << std::endl;
    std::string file_path = "resources/" + filename;
    std::ifstream file(file_path);

    if (!file) {
        throw std::runtime_error("Failed to open test resource: " + file_path);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
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

class OpaquePointerTransformerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }

    // Helper function to check if output contains opaque pointers
    bool containsOpaquePointers(const std::string& ir) {
        // Opaque pointers are represented as "ptr" instead of typed pointers like "i32*"
        return ir.find(" ptr ") != std::string::npos || 
               ir.find("(ptr") != std::string::npos ||
               ir.find("ptr,") != std::string::npos ||
               ir.find("ptr)") != std::string::npos;
    }

    // Helper function to check if output contains typed pointers
    bool containsTypedPointers(const std::string& ir) {
        // Look for patterns like "i32*", "i8*", etc.
        return ir.find("i32*") != std::string::npos ||
               ir.find("i8*") != std::string::npos ||
               ir.find("i64*") != std::string::npos ||
               ir.find("float*") != std::string::npos ||
               ir.find("double*") != std::string::npos;
    }
};

TEST_F(OpaquePointerTransformerTest, TransformSimpleFunction) {
    std::string_view input_ir = R"(
define i32 @test_func(i32* %arg) {
entry:
  %loaded = load i32, i32* %arg
  ret i32 %loaded
}
)";
    Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text())) << "Result should contain opaque pointers";
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text())) << "Result should not contain typed pointers";
}

TEST_F(OpaquePointerTransformerTest, TransformWithAlloca) {
    std::string_view input_ir = R"(
define void @test_alloca() {
entry:
  %ptr = alloca i32
  store i32 42, i32* %ptr
  ret void
}
)";

    Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
}

TEST_F(OpaquePointerTransformerTest, TransformWithGEP) {
    std::string_view input_ir = R"(
%struct.Test = type { i32, i64 }

define i32 @test_gep(%struct.Test* %s) {
entry:
  %field_ptr = getelementptr %struct.Test, %struct.Test* %s, i32 0, i32 0
  %value = load i32, i32* %field_ptr
  ret i32 %value
}
)";

    Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
}

TEST_F(OpaquePointerTransformerTest, TransformWithPHINodes) {
    std::string_view input_ir = R"(
define i32 @test_phi(i1 %cond, i32* %ptr1, i32* %ptr2) {
entry:
  br i1 %cond, label %then, label %else

then:
  br label %merge

else:
  br label %merge

merge:
  %selected_ptr = phi i32* [ %ptr1, %then ], [ %ptr2, %else ]
  %value = load i32, i32* %selected_ptr
  ret i32 %value
}
)";

    
    Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
}

TEST_F(OpaquePointerTransformerTest, TransformWithCalls) {
    std::string_view input_ir = R"(
declare void @external_func(i8*)

define void @test_call(i8* %ptr) {
entry:
  call void @external_func(i8* %ptr)
  ret void
}
)";

    Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
}

TEST_F(OpaquePointerTransformerTest, TransformAlreadyOpaquePointers) {
    std::string_view input_ir = R"(
define i32 @already_opaque(ptr %arg) {
entry:
  %loaded = load i32, ptr %arg
  ret i32 %loaded
}
)";

    Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
}

TEST_F(OpaquePointerTransformerTest, TransformEmptyModule) {
    std::string_view input_ir = "";
    
    
    Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasError()) << "Transformation should fail for empty input";   
}

TEST_F(OpaquePointerTransformerTest, TransformInvalidIR) {
    std::string_view input_ir = "invalid llvm ir code here";
    
     Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    // Should handle invalid IR gracefully by returning false
    EXPECT_TRUE(result.hasError()) << "Transformation should fail for invalid IR";
}

TEST_F(OpaquePointerTransformerTest, TransformComplexModule) {
    std::string_view input_ir = R"(
%struct.Node = type { i32, %struct.Node* }

define %struct.Node* @create_node(i32 %value) {
entry:
  %node = alloca %struct.Node
  %value_ptr = getelementptr %struct.Node, %struct.Node* %node, i32 0, i32 0
  store i32 %value, i32* %value_ptr
  %next_ptr = getelementptr %struct.Node, %struct.Node* %node, i32 0, i32 1
  store %struct.Node* null, %struct.Node** %next_ptr
  ret %struct.Node* %node
}

define i32 @get_value(%struct.Node* %node) {
entry:
  %value_ptr = getelementptr %struct.Node, %struct.Node* %node, i32 0, i32 0
  %value = load i32, i32* %value_ptr
  ret i32 %value
}
)";

    Result<TransformResult> result =  llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
    
    // Should contain both functions
    EXPECT_TRUE(result.getValue().as_text().find("create_node") != std::string::npos);
    EXPECT_TRUE(result.getValue().as_text().find("get_value") != std::string::npos);
}

TEST_F(OpaquePointerTransformerTest, TransformWithArrays) {
    std::string_view input_ir = R"(
define i32 @array_access(i32* %arr, i32 %index) {
entry:
  %element_ptr = getelementptr i32, i32* %arr, i32 %index
  %value = load i32, i32* %element_ptr
  ret i32 %value
}
)";

    Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
}

TEST_F(OpaquePointerTransformerTest, TransformWithReturnPointer) {
    std::string_view input_ir = R"(
define i32* @return_pointer(i32* %input) {
entry:
  ret i32* %input
}
)";

     Result<TransformResult> result = llvm_transformer::transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
}

// Test the new error handling version
TEST_F(OpaquePointerTransformerTest, TransformWithErrorHandling_SimpleFunction) {
    std::string_view input_ir = R"(
define i32 @test_func(i32* %arg) {
entry:
  %loaded = load i32, i32* %arg
  ret i32 %loaded
}
)";

     Result<TransformResult> result = transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    if (result.hasValue()) {
        EXPECT_FALSE(result.getValue().as_text().empty());
    }
}

TEST_F(OpaquePointerTransformerTest, TransformMDLShader) {
    std::string_view input_ir = R"(
; ModuleID = 'lambda_mod'

%State_core = type { [3 x float], [3 x float], [3 x float], float, [3 x float]*, [3 x float]*, [3 x float]*, [4 x float]*, i8*, [4 x float]*, [4 x float]*, i32, float }
%Res_data_pair = type { i8*, i8* }

; Function Attrs: nofree nounwind willreturn
define void @tint(<3 x float>* noalias nocapture %sret_ptr, %State_core* noalias nocapture readonly %state, %Res_data_pair* noalias nocapture readnone %res_data_pair, i8* noalias nocapture readnone %captured_arguments) local_unnamed_addr #0 {
start:
  %.elt.i = getelementptr inbounds %State_core, %State_core* %state, i64 0, i32 0, i64 0
  %.unpack.i = load float, float* %.elt.i, align 4, !alias.scope !0
  %.elt1.i = getelementptr inbounds %State_core, %State_core* %state, i64 0, i32 0, i64 1
  %.unpack2.i = load float, float* %.elt1.i, align 4, !alias.scope !0
  %.elt3.i = getelementptr inbounds %State_core, %State_core* %state, i64 0, i32 0, i64 2
  %.unpack4.i = load float, float* %.elt3.i, align 4, !alias.scope !0
  %0 = insertelement <3 x float> undef, float %.unpack.i, i32 0
  %1 = insertelement <3 x float> %0, float %.unpack2.i, i32 1
  %2 = insertelement <3 x float> %1, float %.unpack4.i, i32 2
  %3 = fmul fast <3 x float> %2, <float 0.000000e+00, float 1.000000e+00, float 0.000000e+00>
  store <3 x float> %3, <3 x float>* %sret_ptr, align 4
  ret void
}

attributes #0 = { nofree nounwind willreturn "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="gfx1036" "target-features"="+16-bit-insts,+ci-insts,+dl-insts,+dot1-insts,+dot10-insts,+dot2-insts,+dot5-insts,+dot6-insts,+dot7-insts,+dpp,+gfx10-3-insts,+gfx10-insts,+gfx8-insts,+gfx9-insts,+s-memrealtime,+s-memtime-inst,+wavefrontsize32" "unsafe-fp-math"="true" }

!0 = !{!1}
!1 = distinct !{!1, !2, !"_ZN5state6normalEv: %state"}
!2 = distinct !{!2, !"_ZN5state6normalEv"}
)";

    Result<TransformResult> result = transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_FALSE(result.getValue().as_text().empty());
    EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text()));
    EXPECT_FALSE(containsTypedPointers(result.getValue().as_text()));
    
    // Check if the function is transformed correctly
    EXPECT_TRUE(result.getValue().as_text().find("tint") != std::string::npos) << "Transformed IR should contain the function 'tint'";
}


TEST_F(OpaquePointerTransformerTest, ErrorHandling_EmptyInput) {
    std::string_view input_ir = "";
    
    auto result = transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_FALSE(result.hasValue()) << "Empty input should fail";
    EXPECT_TRUE(result.getErrorHandler().hasErrors());
    
    auto errors = result.getErrorHandler().getErrorsByType(ErrorType::PARSING_ERROR);
    EXPECT_FALSE(errors.empty());
    EXPECT_EQ(errors[0].message, "Input IR is empty");
}

TEST_F(OpaquePointerTransformerTest, ErrorHandling_InvalidIR) {
    std::string_view input_ir = "invalid llvm ir code here";
    
    auto result = transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_FALSE(result.hasValue()) << "Invalid IR should fail";
    EXPECT_TRUE(result.getErrorHandler().hasErrors());
    
    auto errors = result.getErrorHandler().getErrorsByType(ErrorType::PARSING_ERROR);
    EXPECT_FALSE(errors.empty());
}

TEST_F(OpaquePointerTransformerTest, ErrorHandling_ModuleValidation) {
    // Test with IR that parses but has no meaningful content
    std::string_view input_ir = R"(
; ModuleID = 'test'
source_filename = "test.c"
)";
    
    auto result = transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_FALSE(result.hasValue()) << "Module with no functions should fail validation";
    EXPECT_TRUE(result.getErrorHandler().hasErrors());
    
    auto errors = result.getErrorHandler().getErrorsByType(ErrorType::VALIDATION_ERROR);
    EXPECT_FALSE(errors.empty());
}





TEST_F(OpaquePointerTransformerTest, TransformRealFile_BeforeReplace) {
    // The resources are now copied to the build directory

   std::string input_ir = load_test_resource("before_replace.ptx.ll");
    
    ASSERT_FALSE(input_ir.empty()) << "File content should not be empty";
    
    // Transform to opaque pointers
    auto result = transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed for real file";
    if (result.hasValue()) {
        EXPECT_FALSE(result.getValue().as_text().empty());
        EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text())) 
            << "Result should contain opaque pointers";
        
        // Check that key functions are preserved
        EXPECT_TRUE(result.getValue().as_text().find("evalGLSL") != std::string::npos)
            << "Function evalGLSL should be preserved";
    }
}

TEST_F(OpaquePointerTransformerTest, TransformRealFile_AfterSubstitution) {
    std::string resource_path = "resources/after_substitution.1.ptx.ll";

    std::string input_ir = load_test_resource("after_substitution.1.ptx.ll");        
    
    ASSERT_FALSE(input_ir.empty()) << "File content should not be empty";
    
    auto result = transform_llvm_ir_to_opaque_pointers(input_ir);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed for after_substitution file";
    if (result.hasValue()) {
        EXPECT_FALSE(result.getValue().as_text().empty());
        EXPECT_TRUE(containsOpaquePointers(result.getValue().as_text())) 
            << "Result should contain opaque pointers";
        
        // Check that key functions are preserved
        EXPECT_TRUE(result.getValue().as_text().find("evalGLSL") != std::string::npos)
            << "Function evalGLSL should be preserved";
    }
}

TEST_F(OpaquePointerTransformerTest, TransformRealFile_Performance) {
    std::string input_ir = load_test_resource("before_replace.ptx.ll"); 
    // Measure transformation time
    auto start = std::chrono::high_resolution_clock::now();
    auto result = transform_llvm_ir_to_opaque_pointers(input_ir);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed";
    EXPECT_LT(duration.count(), 5000) << "Transformation should complete within 5 seconds";
    
    std::cout << "Transformation took: " << duration.count() << " ms" << std::endl;
}


TEST_F(OpaquePointerTransformerTest, TransformRealFile_SaveBitcode) {

    std::string input_ir = load_test_resource("before_replace.ptx.ll");
    
    ASSERT_FALSE(input_ir.empty()) << "File content should not be empty";
    
    // Set options to output bitcode
    TransformOptions options;
    options.output_bitcode = true; // Enable bitcode output
     options.amdgcn_target = AMDGCNTarget::GFX1030;
    options.remove_compiler_info = true;
    
    auto result = transform_llvm_ir_to_opaque_pointers(input_ir, options);
    
    EXPECT_TRUE(result.hasValue()) << "Transformation should succeed for real file with bitcode output";

    save_result_to_file ("bitcode.gfx1030.bc", result.getValue());

}