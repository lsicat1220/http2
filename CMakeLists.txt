cmake_minimum_required(VERSION 3.15)

project(http 
	VERSION 0.0.1
	LANGUAGES C
)

add_subdirectory(src)
add_subdirectory(tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug)  # Or pass -DCMAKE_BUILD_TYPE=Debug
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall")  # Full debug, no opts


