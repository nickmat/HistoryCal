# Test CMake

cmake_minimum_required (VERSION 3.10)

project( hcal )

option( BUILD_SHARED_LIBS "Set static libs" ON )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(wxWidgets)
add_subdirectory(src/cal)
add_subdirectory(src/utf8)
add_subdirectory(src/hcs)
add_subdirectory(src/hcal)
add_subdirectory(test/hctest)
