# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        CMakeLists.txt
# Project:     hcal: HistoryCal Application.
# Author:      Nick Matthews
# Website:     http://historycal.org/
# Created:     22nd January 2021
# Copyright:   Copyright (c) 2021..2023, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cmake_minimum_required( VERSION 3.25 )

project( hcal )

option( BUILD_SHARED_LIBS "Set static libs" OFF )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED On )

add_subdirectory( 3rdparty/glich )
include_directories( 3rdparty/glich/include )
add_subdirectory( 3rdparty/wxWidgets )
add_subdirectory( src/hcal )
