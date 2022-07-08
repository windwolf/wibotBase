cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

function(switch_os_port base_dir os_port_name)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${base_dir}/os
    )
    message(STATUS "os ${base_dir}/os")

    # driver的OS port
    list_source_files(${base_dir}/os/${os_port_name}/src OS_SOURCE_FILES)
    target_sources(${PROJECT_NAME}
        PRIVATE
        ${OS_SOURCE_FILES}
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${base_dir}/os/${os_port_name}/inc
    )
endfunction()

if(NOT DEFINED BASE_OS_PORT_NAME)
    message(FATAL_ERROR "Error: BASE_OS_PORT_NAME not defined")
else()
    message(STATUS "BASE_OS_PORT_NAME: ${BASE_OS_PORT_NAME}")
endif()

# OS PORT
switch_os_port(${CMAKE_CURRENT_LIST_DIR} ${BASE_OS_PORT_NAME})

process_src_dir(${CMAKE_CURRENT_LIST_DIR} ${PROJECT_NAME})
