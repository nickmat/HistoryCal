# Test CMake

cmake_minimum_required( VERSION 3.25 )

project( hcal )

option( BUILD_SHARED_LIBS "Set static libs" OFF )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED On )

add_subdirectory( 3rdparty/CppUnit/src/cppunit )
add_subdirectory( 3rdparty/wxWidgets )
add_subdirectory( src/cal )
add_subdirectory( src/utf8 )
add_subdirectory( src/hcs )
add_subdirectory( src/hcal )
add_subdirectory( test/hctest )
add_subdirectory( test/unit )
add_subdirectory( test/soak )
