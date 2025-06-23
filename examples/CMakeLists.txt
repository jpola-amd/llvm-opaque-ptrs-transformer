set(target_example example_transformer)

add_executable(${target_example}
    error_handling_example.cpp
)

target_link_libraries(${target_example} PRIVATE
    ToOpaquePtrsTransformer
)


# target_include_directories(${target_example} PUBLIC
#    ${CMAKE_SOURCE_DIR}/include
# )

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