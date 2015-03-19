/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/soak/soak_i.cpp
 * Project:     Cal Soak test: Extended Unit Tests for Cal library API.
 * Purpose:     CppUnit soak tests for the Islamic Arithmetical calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     10th December 2014
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

#include "soakcommon.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/config/SourcePrefix.h>

using namespace std;
using namespace Cal;

class SoakIslamic : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( SoakIslamic );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testIslamic );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    bool IsLeapYear( Field year );
    Field LastDayInMonth( Field year, Field month );

    void testScript();
    void testIslamic();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( SoakIslamic );


void SoakIslamic::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"i\" {name \"Islamic Tabular\"; base islamic IIc;}" );
    m_sid = m_cal->get_scheme( "i" );
}

void SoakIslamic::tearDown()
{
    delete m_cal;
}

void SoakIslamic::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("i");
    string name("Islamic Tabular");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

#ifdef CALTEST_SHORT
#define CALTEST_I_START_YEAR  1305
#define CALTEST_I_START_JDN   2410534
#define CALTEST_I_END_YEAR    1430
#endif

#ifdef CALTEST_LONG
#define CALTEST_I_START_YEAR  600
#define CALTEST_I_START_JDN   2160706
#define CALTEST_I_END_YEAR    1475
#endif

#ifdef CALTEST_SOAK
#define CALTEST_I_START_YEAR  1
#define CALTEST_I_START_JDN   1948440
#define CALTEST_I_END_YEAR    2200
#endif



bool SoakIslamic::IsLeapYear( Field year )
{
    switch( year % 30 )
    {
    case 2: case 5: case 7: case 10:
    case 13: case 16: case 18: case 21:
    case 24: case 26: case 29:
        return true;
    }
    return false;
}

Field SoakIslamic::LastDayInMonth( Field year, Field month )
{
    switch( month )
    {
    case 1: case 3: case 5: case 7: case 9: case 11:
        return 30;
    case 2: case 4: case 6: case 8: case 10:
        return 29;
    case 12:
        return IsLeapYear( year ) ? 30 : 29;
    }
    return f_invalid;
}

void SoakIslamic::testIslamic()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string date = ymdToStr( CALTEST_I_START_YEAR, 1, 1 );
    Field daycount = m_cal->str_to_jdn( m_sid, date );
    CPPUNIT_ASSERT_EQUAL( daycount, CALTEST_I_START_JDN );
    string cvtdate;
    Range range;
    for( Field year = CALTEST_I_START_YEAR ; year < CALTEST_I_END_YEAR ; year++ ) {
        Field month1 = daycount;
        for( Field month = 1 ; month <= 12 ; month++ ) {
            if( year == 15 && month == 12 ) {
                int x = 0;
            }
            Field day1 = daycount;
            int month_length = LastDayInMonth( year, month );
            for( Field day = 1 ; day <= month_length ; day++ ) {
                date = ymdToStr( year, month, day );
                Field jdn = m_cal->str_to_jdn( m_sid, date );
                CPPUNIT_ASSERT_EQUAL( daycount, jdn );
                cvtdate = m_cal->jdn_to_str( m_sid, jdn );
                CPPUNIT_ASSERT_EQUAL( date, cvtdate );
                daycount++;
            }
            date = ymToStr( year, month );
            range = m_cal->str_to_range( m_sid, date );
            CPPUNIT_ASSERT_EQUAL( day1, range.jdn1 );
            CPPUNIT_ASSERT_EQUAL( daycount-1, range.jdn2 );
            cvtdate = m_cal->range_to_str( m_sid, range );
            CPPUNIT_ASSERT_EQUAL( date, cvtdate );
        }
        date = yToStr( year );
        range = m_cal->str_to_range( m_sid, date );
        CPPUNIT_ASSERT_EQUAL( month1, range.jdn1 );
        CPPUNIT_ASSERT_EQUAL( daycount-1, range.jdn2 );
        cvtdate = m_cal->range_to_str( m_sid, range );
        CPPUNIT_ASSERT_EQUAL( date, cvtdate );
    }
}

// End of test/soak/soak_i.cpp file
