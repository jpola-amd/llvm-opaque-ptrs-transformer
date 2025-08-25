project(hip_runner)


cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "HIP_ROOT_DIR: ${HIP_ROOT_DIR}")

set(HIP_INCLUDE_DIRS ${HIP_ROOT_DIR}/include)
set(HIP_LIBRARY_DIR ${HIP_ROOT_DIR}/lib)

set(hip_libraries 
    amdhip64.lib
    amd_comgr_2.lib
    amd_comgr0604.lib
    hiprtc.lib
    hiprtc-builtins.lib)

set(HIP_LIBRARIES)

foreach (lib ${hip_libraries})
    find_library(${lib}
        NAMES ${lib}
        HINTS ${HIP_LIBRARY_DIR}
        PATH_SUFFIXES lib64 lib)
    if(NOT ${lib})
        message(FATAL_ERROR "Could not find ${lib} library")
    endif()
    list(APPEND HIP_LIBRARIES ${HIP_LIBRARY_DIR}/${lib})
endforeach()

add_executable(hip_runner main.cpp)
target_include_directories(hip_runner PRIVATE ${HIP_INCLUDE_DIRS})
target_link_libraries(hip_runner PRIVATE ${HIP_LIBRARIES})
target_compile_definitions(hip_runner PRIVATE
    __HIP_PLATFORM_AMD__
    )
# install
install(TARGETS hip_runner
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)