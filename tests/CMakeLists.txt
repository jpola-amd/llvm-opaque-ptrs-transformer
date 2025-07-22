# Test CMakeLists.txt

# Find Google Test
find_package(GTest REQUIRED)

set(test_target test_transformer)

add_executable(${test_target}
    test_transformer.cpp
)

# message(STATUS "ZLIB: ${ZLIB_LIBRARY}")
# Link against your transformer library and GoogleTest
target_link_libraries(${test_target} PRIVATE
    ToOpaquePtrsTransformer
    GTest::gtest
    GTest::gtest_main
#    ${ZLIB_LIBRARY}
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

# Copy test resources to build directory
# This ensures tests can find the resource files regardless of where they run from
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/resources)
    message(STATUS "Copying test resources from ${CMAKE_CURRENT_SOURCE_DIR}/resources to ${CMAKE_CURRENT_BINARY_DIR}/resources")
    
    # Copy resources directory to build directory
    add_custom_command(TARGET ${test_target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
        COMMENT "Copying test resources to build directory"
    )
    
    # For multi-config generators (Visual Studio), also copy to config-specific directories
    if(CMAKE_CONFIGURATION_TYPES)
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            add_custom_command(TARGET ${test_target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/resources
                ${CMAKE_CURRENT_BINARY_DIR}/${config}/resources
                COMMENT "Copying test resources to ${config} build directory"
            )
        endforeach()
    endif()
else()
    message(WARNING "Test resources directory not found at ${CMAKE_CURRENT_SOURCE_DIR}/resources")
endif()

# Ensure the test executable can find the resources
# Set the working directory for the test executable
set_target_properties(${test_target} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Add test to CTest
add_test(NAME TransformerTests COMMAND ${test_target})

# Enable testing
enable_testing()

# Optional: Add a custom target to just copy resources (useful for development)
add_custom_target(copy_test_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMENT "Copying test resources"
)

add_dependencies(${test_target} copy_test_resources)