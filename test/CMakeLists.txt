aux_source_directory(. sources)

add_executable(test_libctmap ${sources})
target_link_libraries(test_libctmap PRIVATE libctmap)

list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_std_17 support_cxx_17)
if (NOT ${support_cxx_17} EQUAL -1)
    target_compile_features(test_libctmap PRIVATE cxx_std_17)
    target_compile_definitions(test_libctmap PRIVATE CT_ENABLE_CXX_17)
endif()
