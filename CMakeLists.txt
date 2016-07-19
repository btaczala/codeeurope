project(codeeurope CXX)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(google-benchmark)
include_directories(${CMAKE_SOURCE_DIR}/google-benchmark/include)

include (CMakeParseArguments)

add_definitions("-fno-exceptions -fno-rtti")

function(add_exec) 

    set(oneValueArgs NAME STANDARD)
    set(multiValues FILES FLAGS)
    cmake_parse_arguments(add_exec "" "${oneValueArgs}" "${multiValues}" ${ARGN})

    if(NOT add_exec_FILES) 
        set(cpp_files ${add_exec_NAME}.cpp)
    else()
        set(cpp_files ${add_exec_FILES})
    endif()

    if(NOT add_exec_STANDARD)
        set(standard 14)
    else()
        set(standard ${add_exec_STANDARD})
    endif()

    if(add_exec_FLAGS)
        message(STATUS "Setting flags ${add_exec_FLAGS}")
        add_definitions(${add_exec_FLAGS})
    endif()

    set(name ${add_exec_NAME}_${standard})

    add_executable(${name} ${cpp_files})
    target_link_libraries(${name} benchmark)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD ${standard})
endfunction()

add_exec(NAME rvalues)
add_exec(NAME rvalues STANDARD 98 FLAGS "-fno-elide-constructors")
add_exec(NAME rvalues STANDARD 11)

add_exec(NAME smartptrs)

add_exec(NAME functions)
