find_package(GTest REQUIRED)

set(test_target test_transformer)

add_executable(${test_target}
    test_transformer.cpp
)

target_link_libraries(${test_target} PRIVATE
    llvmtransformer
    GTest::gtest
    GTest::gtest_main
)

set_target_properties(${test_target} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/resources)
    message(STATUS "Copying test resources from ${CMAKE_CURRENT_SOURCE_DIR}/resources to ${CMAKE_CURRENT_BINARY_DIR}/resources")
    
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

set_target_properties(${test_target} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(NAME TransformerTests COMMAND ${test_target})

enable_testing()

add_custom_target(copy_test_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMENT "Copying test resources"
)

add_dependencies(${test_target} copy_test_resources)