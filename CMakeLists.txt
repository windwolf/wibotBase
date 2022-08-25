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
    ${CMAKE_CURRENT_LIST_DIR}/os/os.cpp
    ${OS_PORT_SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/os
    ${CMAKE_CURRENT_LIST_DIR}/os/${OS_PORT}
)

# peripheral
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/peripheral ${PROJECT_NAME})
list_source_files(${CMAKE_CURRENT_LIST_DIR}/peripheral/${BSP_PORT} _perip_SOURCE_FILES)
target_sources(${PROJECT_NAME}
    PRIVATE

    ${_perip_SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE

    ${CMAKE_CURRENT_LIST_DIR}/peripheral/${BSP_PORT}
)

if(NOT DEFINED USE_RTT_PRINT)
    message(STATUS "USE_RTT_PRINT: not set, use default value : ON")
    set(USE_RTT_PRINT ON)
else()
    message(STATUS "USE_RTT_PRINT: ${USE_RTT_PRINT}")
endif()

if(${USE_RTT_PRINT})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DUSE_RTT_PRINT)
    process_src_dir(${CMAKE_CURRENT_LIST_DIR}/rtt ${PROJECT_NAME})
endif()

if(NOT DEFINED ENABLE_FSM_TRANSITION_PREFILTER)
    message(STATUS "ENABLE_FSM_TRANSITION_PREFILTER: not set, use default value : ON")
    set(ENABLE_FSM_TRANSITION_PREFILTER ON)
else()
    message(STATUS "ENABLE_FSM_TRANSITION_PREFILTER: ${ENABLE_FSM_TRANSITION_PREFILTER}")
endif()

if(${ENABLE_FSM_TRANSITION_PREFILTER})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DFSM_TRANSITION_PREFILTER)
endif()

process_src_dir(${CMAKE_CURRENT_LIST_DIR}/base ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/temporal ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/test ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/graph ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/fonts ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/fsm ${PROJECT_NAME})