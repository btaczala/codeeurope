project(codeeurope CXX)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(google-benchmark)
include_directories(${CMAKE_SOURCE_DIR}/google-benchmark/include)

include (CMakeParseArguments)

#add_definitions("-fno-exceptions -fno-rtti")

function(add_benchmark) 

    set(oneValueArgs NAME STANDARD)
    set(multiValues FILES FLAGS)
    cmake_parse_arguments(add_exec "" "${oneValueArgs}" "${multiValues}" ${ARGN})

    if(NOT add_exec_FILES) 
        set(cpp_files benchmark_${add_exec_NAME}.cpp)
    else()
        set(cpp_files benchmark_${add_exec_FILES})
    endif()

    set(standard 17)
    if(add_exec_FLAGS)
        message(STATUS "Setting flags ${add_exec_FLAGS}")
        add_definitions(${add_exec_FLAGS})
    endif()

    set(name benchmark_${add_exec_NAME}_${standard})

    add_executable(${name} ${cpp_files})
    target_link_libraries(${name} benchmark)
endfunction()

add_definitions(-std=c++1z)

add_benchmark(NAME smartptrs)
add_benchmark(NAME functions)
add_benchmark(NAME array)
