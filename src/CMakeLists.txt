
set(target_name ToOpaquePtrsTransformer)
if (WIN32)
  set(fat_target_name llvmtransformer.lib)
else()
  set(fat_target_name libllvmtransformer.a)
  endif()

add_library(${target_name} STATIC
 transformer.cpp
 error_handler.cpp
)
set_target_properties(${test_target} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

llvm_map_components_to_libnames(llvm_libs support core irreader bitwriter)

message(STATUS "LLVM libraries: ${llvm_libs}")

execute_process(
    COMMAND ${LLVM_TOOLS_BINARY_DIR}/llvm-config --link-static --libs  support core irreader bitwriter
    OUTPUT_VARIABLE LLVM_LIBS_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

separate_arguments(LLVM_LIBS_LIST NATIVE_COMMAND "${LLVM_LIBS_STRING}")
message(STATUS "LLVM libraries from llvm-config: ${LLVM_LIBS_LIST}")


target_link_libraries(${target_name} PRIVATE ${LLVM_LIBS_LIST})
target_include_directories(${target_name} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_options(${target_name} PRIVATE -fvisibility=hidden)

target_include_directories(${target_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(${target_name} PROPERTIES LINK_SEARCH_START_STATIC ON)

# Create a fat static library that includes all dependencies
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${fat_target_name}
    COMMAND lib.exe /OUT:${CMAKE_CURRENT_BINARY_DIR}/${fat_target_name} $<TARGET_FILE:${target_name}> ${LLVM_LIBS_LIST}
    DEPENDS ${target_name}
    COMMENT "Creating fat static library with all dependencies"
)
add_custom_target(${target_name}_fat ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${fat_target_name})

install(FILES
    include/transformer.h
    include/error_handler.h
    DESTINATION include
)

set_target_properties(${target_name}_fat PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${fat_target_name}
    DESTINATION lib
)