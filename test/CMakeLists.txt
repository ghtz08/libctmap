aux_source_directory(. sources)

add_executable(test_libctmap ${sources})
target_link_libraries(test_libctmap PRIVATE libctmap)
