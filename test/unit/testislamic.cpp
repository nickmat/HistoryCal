/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testislamic.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the Islamic Arithmetical calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     16th December 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
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

class TestIslamic : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestIslamic );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestIslamic );


void TestIslamic::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"i\" {name \"Islamic Tabular\"; base islamic;}" );
    m_sid = m_cal->get_scheme( "i" );
}

void TestIslamic::tearDown()
{
    delete m_cal;
}


DMYDate testIValues[MaxSample] = {
//    day mon year
    {  9, 12, -1245 },
    { 23,  2,  -813 },
    {  1,  4,  -568 },
    {  6,  4,  -501 },
    { 17, 10,  -157 },
    {  3,  6,   -47 },
    { 13,  7,    75 },
    {  5, 10,   403 },
    { 22,  5,   489 },
    {  7,  2,   586 },
    {  7,  8,   637 },
    { 20,  2,   687 },
    {  7,  7,   697 },
    {  1,  7,   793 },
    {  6,  7,   839 },
    {  1,  6,   897 },
    { 30,  9,   960 },
    { 27,  5,   967 },
    { 18,  5,  1058 },
    {  2,  6,  1091 },
    {  4,  8,  1128 },
    {  3,  2,  1182 },
    { 10, 10,  1234 },
    { 11,  1,  1255 },
    { 21,  1,  1321 },
    { 19,  3,  1348 },
    {  8,  9,  1360 },
    { 13,  4,  1362 },
    {  7, 10,  1362 },
    { 13,  9,  1412 },
    {  5, 10,  1416 },
    { 12, 10,  1460 },
    {  5,  3,  1518 }
};


void TestIslamic::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("i");
    string name("Islamic Tabular");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestIslamic::testValues()
{
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst; 
        tst << testIValues[i].year << " "
            << testIValues[i].month << " "
            << testIValues[i].day;
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( m_sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string jbstr = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( value, jbstr );
    }
}

void TestIslamic::testRanges()
{
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst; 
        tst << testIValues[i].year << " "
            << testIValues[i].month << " "
            << testIValues[i].day << " ~ "
            << testIValues[i+1].year << " "
            << testIValues[i+1].month << " "
            << testIValues[i+1].day;
        string value = tst.str();
        if( value == "897 6 1 ~ 960 9 30" ) { // this should collapse
            value = "897 6 ~ 960 9";         // to this.
        }
        Range rng = m_cal->str_to_range( m_sid, value/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( m_sid, rng );
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
}

// End of test/unit/testislamic.cpp file
