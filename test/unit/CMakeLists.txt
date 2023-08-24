# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        test/unit/CMakeLists.txt
# Project:     Cal Unit test: Unit Tests for Cal library API.
# Author:      Nick Matthews
# Website:     http://thefamilypack.org
# Created:     24th August 2023
# Copyright:   Copyright (c) 2023, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set( TESTS_FILES
    api_expr_str.cpp
    def_eng.cpp
    def_er.cpp
    def_expr_str.cpp
    def_fr.cpp
    def_g.cpp
    def_glit.cpp
    def_h.cpp
    def_i.cpp
    def_isoo.cpp
    def_isow.cpp
    def_j325.cpp
    def_jce.cpp
    def_je.cpp
    def_jlit.cpp
    testcal.cpp
    testchinese.cpp
    testcommon.cpp
    testformat.cpp
    testfrenchrep.cpp
    testgrammar.cpp
    testgregorian.cpp
    testhebrew.cpp
    testhybrid.cpp
    testislamic.cpp
    testiso.cpp
    testj325.cpp
    testjulian.cpp
    testmain.cpp
    testregnal.cpp
    testshiftday.cpp
    testvocab.cpp
)

set( TESTS_HEADERS
    testcommon.h
)

include_directories( ../../3rdparty/CppUnit/include )

add_executable( unit ${TESTS_FILES} ${TESTS_HEADERS} )

target_link_libraries( unit PRIVATE cppunit cal )
