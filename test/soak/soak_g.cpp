/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/soak/soak_g.cpp
 * Project:     Cal Soak test: Extended Unit Tests for Cal library API.
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

#include "soakcommon.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/config/SourcePrefix.h>

using namespace std;
using namespace Cal;

class SoakGregorian : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( SoakGregorian );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testGregorianCalendar );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testScript();
    void testGregorianCalendar();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( SoakGregorian );


void SoakGregorian::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"g\" {name \"Gregorian\"; base gregorian;}" );
    m_sid = m_cal->get_scheme( "g" );
}

void SoakGregorian::tearDown()
{
    delete m_cal;
}

void SoakGregorian::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("g");
    string name("Gregorian");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}


#ifdef CALTEST_SHORT
#define CALTEST_G_START_YEAR     1890
#define CALTEST_G_START_JDN      2411369
#define CALTEST_G_END_YEAR       2010
#endif

#ifdef CALTEST_LONG
#define CALTEST_G_START_YEAR     1200
#define CALTEST_G_START_JDN      2159351
#define CALTEST_G_END_YEAR       2050
#endif

#ifdef CALTEST_SOAK
#define CALTEST_G_START_YEAR     -1000
#define CALTEST_G_START_JDN      1355818
#define CALTEST_G_END_YEAR       2150
#endif

void SoakGregorian::testGregorianCalendar()
{
    Field LengthOfMonth[2][12] = {
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
    };

    CPPUNIT_ASSERT( m_sid >= 0 );
    string date = ymdToStr( CALTEST_G_START_YEAR, 1, 1 );
    Field daycount = m_cal->str_to_jdn( m_sid, date );
    CPPUNIT_ASSERT_EQUAL( daycount, CALTEST_G_START_JDN );

    string cvtdate;
    Range range;
    for( Field year = CALTEST_G_START_YEAR ; year < CALTEST_G_END_YEAR ; year++ ) {
        Field month1 = daycount;
        size_t leap_year = 0;
        if( year % 4 == 0 ) leap_year = 1;
        if( year % 100 == 0 ) leap_year = 0;
        if( year % 400 == 0 ) leap_year = 1;
        for( Field month = 1 ; month <= 12 ; month++ ) {
            int month_length = LengthOfMonth[leap_year][month-1];
            Field day1 = daycount;
            for( Field day = 1 ; day <= month_length ; day++ ) {
                date = ymdToStr( year, month, day );
                Field jdn = m_cal->str_to_jdn( m_sid, date );
                CPPUNIT_ASSERT_EQUAL( daycount, jdn );
                cvtdate = m_cal->jdn_to_str( m_sid, jdn );
                CPPUNIT_ASSERT_EQUAL( date, cvtdate );
                daycount++;
            }
            date = ymToStr( year, month );
            range = m_cal->str_to_range( m_sid, date/*, 0*/ );
            CPPUNIT_ASSERT_EQUAL( day1, range.jdn1 );
            CPPUNIT_ASSERT_EQUAL( daycount-1, range.jdn2 );
            cvtdate = m_cal->range_to_str( m_sid, range );
            CPPUNIT_ASSERT_EQUAL( date, cvtdate );
        }
        date = yToStr( year );
        range = m_cal->str_to_range( m_sid, date/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( month1, range.jdn1 );
        CPPUNIT_ASSERT_EQUAL( daycount-1, range.jdn2 );
        cvtdate = m_cal->range_to_str( m_sid, range );
        CPPUNIT_ASSERT_EQUAL( date, cvtdate );
    }
}

// End of test/soak/soak_g.cpp file
