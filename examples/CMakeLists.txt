set(target_example example_transformer)

add_executable(${target_example}
    error_handling_example.cpp
)

target_link_libraries(${target_example} PRIVATE
    ToOpaquePtrsTransformer
)

set_target_properties(${target_example} PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

install(TARGETS ${target_example}
  EXPORT ToOpaquePtrsTransformerExampleTargets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)


set(target_transform transoform)
add_executable(${target_transform} transform.cpp)
target_link_libraries(${target_transform} PRIVATE ToOpaquePtrsTransformer)
set_target_properties(${target_transform} PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

install(TARGETS ${target_transform}
  EXPORT ToOpaquePtrsTransformerExampleTargets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)