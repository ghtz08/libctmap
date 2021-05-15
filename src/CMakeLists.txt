add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/src
)

install(FILES ${PROJECT_SOURCE_DIR}/src/ctmap.hpp
    DESTINATION include
)
