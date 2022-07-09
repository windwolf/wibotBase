cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if(NOT DEFINED OS_PORT)
    message(FATAL_ERROR "Error: OS_PORT not defined")
else()
    message(STATUS "OS_PORT: ${OS_PORT}")
endif()

# OS PORT
list_source_files(${CMAKE_CURRENT_LIST_DIR}/os/${OS_PORT} OS_PORT_SOURCE_FILES)
message(STATUS "OS_PORT_SOURCE_FILES: ${OS_PORT_SOURCE_FILES}")
target_sources(${PROJECT_NAME}
    PRIVATE
    ${OS_PORT_SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/os/${OS_PORT}
)
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/os/os ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/base ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/test ${PROJECT_NAME})