
set(target_name ToOpaquePtrsTransformer)


add_library(${target_name} STATIC
 transformer.cpp
 error_handler.cpp
)

llvm_map_components_to_libnames(llvm_libs support core irreader bitwriter)

message(STATUS "LLVM libraries: ${llvm_libs}")

target_link_libraries(${target_name} PRIVATE ${llvm_libs})

# Make LLVM includes available as SYSTEM headers (suppresses warnings)
target_include_directories(${target_name} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})

target_include_directories(${target_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(FILES
    include/transformer.h
    include/error_handler.h
    DESTINATION include
)

set_target_properties(${target_name} PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

install(TARGETS ${target_name}
  EXPORT ToOpaquePtrsTransformerTargets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
)