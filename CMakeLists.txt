cmake_minimum_required(VERSION 3.10)
project(ToOpaquePtrsTranformer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# LLVM Config
# Validate required variables
if(NOT DEFINED LLVM_ROOT_DIR)
    message(FATAL_ERROR "LLVM_ROOT_DIR is not set")
endif()
message(STATUS "LLVM path: ${LLVM_ROOT_DIR}")
#list(APPEND CMAKE_PREFIX_PATH "${LLVM_ROOT_DIR}")


# Find LLVM 16 modules 
find_package(LLVM 16 REQUIRED CONFIG
    HINTS ${LLVM_ROOT_DIR}
    PATHS ${LLVM_ROOT_DIR}/lib/cmake/llvm
    ) 

message(STATUS "LLVM_DIR: ${LLVM_DIR}")
message(STATUS "LLVM_PACKAGE_VERSION: ${LLVM_PACKAGE_VERSION}")

if (NOT LLVM_FOUND)
  message(FATAL_ERROR "LLVM 16 not found")  
endif()

# add project from src
add_subdirectory(src)

if(BUILD_TESTS)
  add_subdirectory(tests)
else()
  message(STATUS "Skipping tests as BUILD_TESTS is not set")
endif()

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
else()
  message(STATUS "Skipping examples as BUILD_EXAMPLES is not set")
endif()


