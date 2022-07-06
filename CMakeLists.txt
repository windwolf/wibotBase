cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

list_source_files(${CMAKE_CURRENT_LIST_DIR}/src TINYUTILS_SOURCE_FILES)
target_sources(${PROJECT_NAME}
    PRIVATE
    ${TINYUTILS_SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)