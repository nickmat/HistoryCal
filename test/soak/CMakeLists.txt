# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        test/soak/CMakeLists.txt
# Project:     Cal Soak test: Unit Tests for Cal library API.
# Author:      Nick Matthews
# Website:     http://thefamilypack.org
# Created:     24th August 2023
# Copyright:   Copyright (c) 2023, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set( SOAK_SOURCES
    soak_eng.cpp
    soak_er.cpp
    soak_fr.cpp
    soak_g.cpp
    soak_h.cpp
    soak_i.cpp
    soak_j.cpp
    soak_j325.cpp
    soakcommon.cpp
    soakmain.cpp
)

set( SOAK_HEADERS
    soakcommon.h
)

include_directories( ../../3rdparty/CppUnit/include )

add_executable( soak ${SOAK_SOURCES} ${SOAK_HEADERS} )

target_link_libraries( soak PRIVATE cppunit cal )
