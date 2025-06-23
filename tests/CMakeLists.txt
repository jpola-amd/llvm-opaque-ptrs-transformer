# Test CMakeLists.txt

# Find Google Test
find_package(GTest REQUIRED)

set(test_target test_transformer)

add_executable(${test_target}
    test_transformer.cpp
)

message(STATUS "ZLIB: ${ZLIB_LIBRARY}")
# Link against your transformer library and GoogleTest
target_link_libraries(${test_target} PRIVATE
    ToOpaquePtrsTransformer
    GTest::gtest
    GTest::gtest_main
    ${ZLIB_LIBRARY}
)

# Make LLVM includes available as system headers to suppress warnings
target_include_directories(${test_target} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})

# # Include the source directory for the transformer header
# target_include_directories(${test_target} PRIVATE
#     ${CMAKE_SOURCE_DIR}/include
# )

# Add compiler definitions if needed
target_compile_definitions(${test_target} PRIVATE ${LLVM_DEFINITIONS})

# Set C++ standard
set_target_properties(${test_target} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add test to CTest
add_test(NAME TransformerTests COMMAND ${test_target})

# Enable testing
enable_testing()
