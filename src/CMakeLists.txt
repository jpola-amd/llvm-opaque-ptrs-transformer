
set(target_name llvmtransformer)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (BUILD_FOR_VRAY)
  set(llvm16_version 1606)
  message(STATUS "Build for Vray - ON, LLVM Ver: ${llvm16_version}")
  
  add_definitions(-Dllvm=VRay_llvm_v${llvm16_version})
	  if(UNIX)
		  add_definitions(
			  -D__STDC_LIMIT_MACROS
			  -D__STDC_CONSTANT_MACROS
			  -D__STDC_FORMAT_MACROS
		  )
	endif()
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

target_link_libraries(${target_name} PRIVATE ${llvm_libs})
target_include_directories(${target_name} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_options(${target_name} PRIVATE -fvisibility=hidden)

target_include_directories(${target_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(${target_name} PROPERTIES LINK_SEARCH_START_STATIC ON)

set_target_properties(${target_name} PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)


install(FILES
    include/transformer.h
    include/error_handler.h
    DESTINATION include
)

install(TARGETS ${target_name}
  EXPORT LLVMTransformerTargets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
)
