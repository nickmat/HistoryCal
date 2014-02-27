/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/test/testweekly.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     CppUnit tests for the Weekly calendar.
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

class TestWeekly : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestWeekly );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testValues );
    CPPUNIT_TEST( testRanges );
    CPPUNIT_TEST( testWeeklyCalendar );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    int             m_sid; // Scheme id

public:
    void setUp();
    void tearDown();

    void testScript();
    void testValues();
    void testRanges();
    void testWeeklyCalendar();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestWeekly );


void TestWeekly::setUp()
{
    m_sid = -1;
    m_cal = new Calendars;
    m_cal->read_script( "scheme w {name Weekly; cyclic;};" );
    m_sid = m_cal->get_scheme_id( "w" );
}

void TestWeekly::tearDown()
{
    delete m_cal;
}

Field testWValues[MaxSample] = {
// day
    7, // Sunday
    3, // Wednesday
    3, // etc
    7,
    3,
    1,
    6,
    7,
    7,
    5,
    6,
    5,
    7,
    7,
    3,
    6,
    6,
    6,
    3,
    7,
    5,
    7,
    1,
    3,
    7,
    7,
    1,
    1,
    4,
    2,
    7,
    3,
    7
};


void TestWeekly::testScript()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string code("w");
    string name("Weekly");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestWeekly::testValues()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    for( int i = 0 ; i < MaxSample ; i++ ) {
        FieldVec fields = m_cal->jdn_to_fieldvec( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( 2U, fields.size() );
        CPPUNIT_ASSERT_EQUAL( testWValues[i], fields[1] );
    }
}

void TestWeekly::testRanges()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        Range range;
        range.jdn1 = testJdnValues[i];
        range.jdn2 = testJdnValues[i+1];
        string date = m_cal->range_to_str( m_sid, range );
        Range result = m_cal->str_to_range( m_sid, date/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( range.jdn1, result.jdn1 );
        CPPUNIT_ASSERT_EQUAL( range.jdn2, result.jdn2 );
    }
}

#define CALTEST_W_START_COUNT    347545
#define CALTEST_W_START_JDN      2432815
#define CALTEST_W_END_COUNT      347600

void TestWeekly::testWeeklyCalendar()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string date = ymToStr( CALTEST_W_START_COUNT, 1 );
    Field daycount = m_cal->str_to_jdn( m_sid, date );
    CPPUNIT_ASSERT_EQUAL( daycount, CALTEST_W_START_JDN );

    string cvtdate;
    Range range;
    for( Field wnum = CALTEST_W_START_COUNT ; wnum < CALTEST_W_END_COUNT ; wnum++ ) {
        Field wstart = daycount;
        for( Field day = 1 ; day <= 7 ; day++ ) {
            date = ymToStr( wnum, day );
            Field jdn = m_cal->str_to_jdn( m_sid, date );
            CPPUNIT_ASSERT_EQUAL( daycount, jdn );
            cvtdate = m_cal->jdn_to_str( m_sid, jdn );
            CPPUNIT_ASSERT_EQUAL( date, cvtdate );
            daycount++;
        }
        date = yToStr( wnum );
        range = m_cal->str_to_range( m_sid, date/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( wstart, range.jdn1 );
        CPPUNIT_ASSERT_EQUAL( daycount-1, range.jdn2 );
        cvtdate = m_cal->range_to_str( m_sid, range );
        CPPUNIT_ASSERT_EQUAL( date, cvtdate );
    }
}


// End of src/test/testjulian.cpp file
