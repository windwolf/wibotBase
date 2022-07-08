cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if(NOT DEFINED DRIVERS_OS)
    message(FATAL_ERROR "Error: DRIVERS_OS not defined")
else()
    message(STATUS "DRIVERS_OS: ${DRIVERS_OS}")
endif()

# OS PORT
drivers_os_port(${CMAKE_CURRENT_LIST_DIR})

process_directory(${CMAKE_CURRENT_LIST_DIR} ${PROJECT_NAME})
