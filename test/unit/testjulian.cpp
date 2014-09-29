/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testjulian.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the Julian Base calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th October 2013
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

class TestJulian : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestJulian );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestJulian );


void TestJulian::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"jb\" {name \"Julian Base\"; base julian;}" );
    m_sid = m_cal->get_scheme( "jb" );
}

void TestJulian::tearDown()
{
    delete m_cal;
}

DMYDate testJBValues[MaxSample] = {
//    day mon year
    { 30,  7, -586 },
    {  8, 12, -168 },
    { 26,  9,   70 },
    {  3, 10,  135 },
    {  7,  1,  470 },
    { 18,  5,  576 },
    {  7, 11,  694 },
    { 19,  4, 1013 },
    { 18,  5, 1096 },
    { 16,  3, 1190 },
    {  3,  3, 1240 },
    { 26,  3, 1288 },
    { 20,  4, 1298 },
    {  4,  6, 1391 },
    { 25,  1, 1436 },
    { 31,  3, 1492 },
    {  9,  9, 1553 },
    { 24,  2, 1560 },
    { 31,  5, 1648 },
    { 20,  6, 1680 },
    { 13,  7, 1716 },
    {  8,  6, 1768 },
    { 21,  7, 1819 },
    { 15,  3, 1839 },
    {  6,  4, 1903 },
    { 12,  8, 1929 },
    { 16,  9, 1941 },
    {  6,  4, 1943 },
    { 24,  9, 1943 },
    {  4,  3, 1992 },
    { 12,  2, 1996 },
    { 28, 10, 2038 },
    {  5,  7, 2094 }
};


void TestJulian::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("jb");
    string name("Julian Base");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestJulian::testValues()
{
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst; 
        tst << testJBValues[i].year << " "
            << testJBValues[i].month << " "
            << testJBValues[i].day;
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( m_sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string jbstr = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( value, jbstr );
    }
}

void TestJulian::testRanges()
{
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst; 
        tst << testJBValues[i].year << " "
            << testJBValues[i].month << " "
            << testJBValues[i].day << " ~ "
            << testJBValues[i+1].year << " "
            << testJBValues[i+1].month << " "
            << testJBValues[i+1].day;
        string value = tst.str();
        Range rng = m_cal->str_to_range( m_sid, value/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( m_sid, rng );
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
}

// End of src/test/testjulian.cpp file
