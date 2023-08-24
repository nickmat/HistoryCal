/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testchinese.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the Chinese calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th June 2016
 * Copyright:   Copyright (c) 2016 .. 2020, Nick Matthews.
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

class TestChinese : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestChinese );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testValues );
    CPPUNIT_TEST( testRanges );
    CPPUNIT_TEST( testShortHand );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testScript();
    void testValues();
    void testRanges();
    void testShortHand();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestChinese );


void TestChinese::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme c {name \"Chinese\"; base chinese;}" );
    m_sid = m_cal->get_scheme( "c" );
}

void TestChinese::tearDown()
{
    delete m_cal;
}

struct CYMLDdate { Field cycle, year, month, lmonth, day; };


CYMLDdate testCValues[MaxSample] = {
//    cyc yr  mon lp day
    { 35, 11,  6, 0, 12 },
    { 42,  9, 10, 0, 27 },
    { 46,  7,  8, 0,  4 },
    { 47, 12,  8, 0,  9 },
    { 52, 46, 11, 0, 20 },
    { 54, 33,  4, 0,  5 },
    { 56, 31, 10, 0, 15 },
    { 61, 50,  3, 0,  7 },
    { 63, 13,  4, 0, 24 },
    { 64, 47,  2, 0,  9 },
    { 65, 37,  2, 0,  9 },
    { 66, 25,  2, 0, 23 },
    { 66, 35,  3, 0,  9 },
    { 68,  8,  5, 0,  2 },
    { 68, 53,  1, 0,  8 },
    { 69, 49,  3, 0,  4 },
    { 70, 50,  8, 0,  2 },
    { 70, 57,  1, 0, 29 },
    { 72, 25,  4, 1, 20 },
    { 72, 57,  6, 0,  5 },
    { 73, 33,  6, 0,  6 },
    { 74, 25,  5, 0,  5 },
    { 75, 16,  6, 0, 12 },
    { 75, 36,  2, 0, 13 },
    { 76, 40,  3, 0, 22 },
    { 77,  6,  7, 0, 21 },
    { 77, 18,  8, 0,  9 },
    { 77, 20,  3, 0, 15 },
    { 77, 20,  9, 0,  9 },
    { 78,  9,  2, 0, 14 },
    { 78, 13,  1, 0,  7 },
    { 78, 55, 10, 0, 14 },
    { 79, 51,  6, 0,  7 },
};


void TestChinese::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("c");
    string name("Chinese");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestChinese::testValues()
{
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst; 
        tst << testCValues[i].cycle << " "
            << testCValues[i].year << " "
            << testCValues[i].month << " "
            << testCValues[i].lmonth << " "
            << testCValues[i].day;
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( m_sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string jbstr = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( value, jbstr );
    }
}

void TestChinese::testRanges()
{
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst; 
        tst << testCValues[i].cycle << " "
            << testCValues[i].year << " "
            << testCValues[i].month << " "
            << testCValues[i].lmonth << " "
            << testCValues[i].day << " .. "
            << testCValues[i+1].cycle << " "
            << testCValues[i+1].year << " "
            << testCValues[i+1].month << " "
            << testCValues[i+1].lmonth << " "
            << testCValues[i+1].day;
        string value = tst.str();
        Range rng = m_cal->str_to_range( m_sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( m_sid, rng );
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
}

void TestChinese::testShortHand()
{
    struct data { string in; string out; Field beg; Field end; } t[] = {
        { "77", "", 2423821, 2445732 },
        { "77 25", "", 2432592, 2432945 },
        { "77 25 8", "", 2432798, 2432827 },
        { "77 25 8 0", "77 25 8", 2432798, 2432827 },
        { "77 25 8 0 17", "", 2432814, 2432814 },
    };
    size_t count = sizeof(t) / sizeof(data);

    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        string str2 = t[i].out;
        if( str2.empty() ) {
            str2 = t[i].in;
        }
        CPPUNIT_ASSERT_EQUAL( str2, str );
        CPPUNIT_ASSERT_EQUAL( size_t(1), rl.size() );
        CPPUNIT_ASSERT_EQUAL( t[i].beg, rl[0].jdn1 );
        CPPUNIT_ASSERT_EQUAL( t[i].end, rl[0].jdn2 );
    }
}

// End of src/test/testchinese.cpp file
