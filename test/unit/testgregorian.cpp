/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testgregorian.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the Gregorian calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th October 2013
 * Copyright:   Copyright (c) 2013-2014, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  The Cal library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The Cal library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the Cal library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#include "testcommon.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/config/SourcePrefix.h>

using namespace std;
using namespace Cal;

class TestGregorian : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestGregorian );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testValues );
    CPPUNIT_TEST( testRanges );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testScript();
    void testValues();
    void testRanges();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestGregorian );


void TestGregorian::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"g\" {name \"Gregorian\"; base gregorian;}" );
    m_sid = m_cal->get_scheme( "g" );
}

void TestGregorian::tearDown()
{
    delete m_cal;
}


DMYDate testGValues[MaxSample] = {
//    day mon year
    { 24,  7, -586 },
    {  5, 12, -168 },
    { 24,  9,   70 },
    {  2, 10,  135 },
    {  8,  1,  470 },
    { 20,  5,  576 },
    { 10, 11,  694 },
    { 25,  4, 1013 },
    { 24,  5, 1096 },
    { 23,  3, 1190 },
    { 10,  3, 1240 },
    {  2,  4, 1288 },
    { 27,  4, 1298 },
    { 12,  6, 1391 },
    {  3,  2, 1436 },
    {  9,  4, 1492 },
    { 19,  9, 1553 },
    {  5,  3, 1560 },
    { 10,  6, 1648 },
    { 30,  6, 1680 },
    { 24,  7, 1716 },
    { 19,  6, 1768 },
    {  2,  8, 1819 },
    { 27,  3, 1839 },
    { 19,  4, 1903 },
    { 25,  8, 1929 },
    { 29,  9, 1941 },
    { 19,  4, 1943 },
    {  7, 10, 1943 },
    { 17,  3, 1992 },
    { 25,  2, 1996 },
    { 10, 11, 2038 },
    { 18,  7, 2094 }
};


void TestGregorian::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("g");
    string name("Gregorian");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestGregorian::testValues()
{
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst; 
        tst << testGValues[i].year << " "
            << testGValues[i].month << " "
            << testGValues[i].day;
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( m_sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string jbstr = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( value, jbstr );
    }
}

void TestGregorian::testRanges()
{
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst; 
        tst << testGValues[i].year << " "
            << testGValues[i].month << " "
            << testGValues[i].day << " ~ "
            << testGValues[i+1].year << " "
            << testGValues[i+1].month << " "
            << testGValues[i+1].day;
        string value = tst.str();
        Range rng = m_cal->str_to_range( m_sid, value/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( m_sid, rng );
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
}

// End of src/test/testgregorian.cpp file
