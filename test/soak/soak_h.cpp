/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/soak/soak_h.cpp
 * Project:     Cal Soak test: Extended Unit Tests for Cal library API.
 * Purpose:     CppUnit soak tests for the Hebrew calendar.
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

class SoakHebrew : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( SoakHebrew );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testHebrew );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    Field DaysInYear( Field year );
    Field LastDayInMonth( Field month, Field diy );

    void testScript();
    void testHebrew();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( SoakHebrew );


void SoakHebrew::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"h\" {name \"Hebrew\"; base hebrew;}" );
    m_sid = m_cal->get_scheme( "h" );
}

void SoakHebrew::tearDown()
{
    delete m_cal;
}

void SoakHebrew::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("h");
    string name("Hebrew");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

#define CALTEST_H_START_YEAR     4120
#define CALTEST_H_START_WDAY     5      // Sun = 1
#define CALTEST_H_START_JDN      1852434

#ifdef CALTEST_SHORT
#define CALTEST_H_END_YEAR       4320
#endif

#ifdef CALTEST_LONG
#define CALTEST_H_END_YEAR       5120
#endif

#ifdef CALTEST_SOAK
#define CALTEST_H_END_YEAR       5910
#endif



Field SoakHebrew::DaysInYear( Field year )
{
    FieldVec fields( 3 );
    fields[0] = year;
    fields[1] = 7;
    fields[2] = 1;
    Field jdn1 = m_cal->fieldvec_to_jdn( m_sid, fields );
    fields[0]++;
    Field jdn2 = m_cal->fieldvec_to_jdn( m_sid, fields );
    Field diy = jdn2 - jdn1;
    bool inrange = false;
    switch( diy )
    {
    case 353: case 354: case 355:
    case 383: case 384: case 385:
        inrange = true;
    }
    CPPUNIT_ASSERT( inrange );
    return diy;
}

Field SoakHebrew::LastDayInMonth( Field month, Field diy )
{
    switch( month )
    {
    case 2: case 4: case 6: case 10: case 13:
        return 29;
    case 1: case 3: case 5: case 7: case 11:
        return 30;
    case 8:
        return ( diy != 355 && diy != 385 ) ? 29 : 30;
    case 9:
        return ( diy == 353 || diy == 383 ) ? 29 : 30;
    case 12:
        return ( diy > 355 ) ? 30 : 29;
    }
    return f_invalid;
}

void SoakHebrew::testHebrew()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string date = ymdToStr( CALTEST_H_START_YEAR, 7, 1 );
    Field daycount = m_cal->str_to_jdn( m_sid, date );
    CPPUNIT_ASSERT_EQUAL( daycount, CALTEST_H_START_JDN );
    Field wday = CALTEST_H_START_WDAY;
    string cvtdate;
    Range range;
    for( Field year = CALTEST_H_START_YEAR ; year < CALTEST_H_END_YEAR ; year++ ) {
        CPPUNIT_ASSERT( wday != 1 ); // Not Sunday
        CPPUNIT_ASSERT( wday != 4 ); // Not Wednesday
        CPPUNIT_ASSERT( wday != 6 ); // Not Friday
//        CPPUNIT_ASSERT( wday != 2 ); // Test
        Field diy = DaysInYear( year );
        Field miy = ( diy > 355 ) ? 13 : 12;
        Field month1 = daycount;
        for( Field month = 7 ; ; month++ ) {
            int month_length = LastDayInMonth( month, diy );
            Field day1 = daycount;
            for( Field day = 1 ; day <= month_length ; day++ ) {
                date = ymdToStr( year, month, day );
                Field jdn = m_cal->str_to_jdn( m_sid, date );
                CPPUNIT_ASSERT_EQUAL( daycount, jdn );
                cvtdate = m_cal->jdn_to_str( m_sid, jdn );
                CPPUNIT_ASSERT_EQUAL( date, cvtdate );
                daycount++;
                wday++;
                if( wday > 7 ) wday = 1;
            }
            date = ymToStr( year, month );
            range = m_cal->str_to_range( m_sid, date );
            CPPUNIT_ASSERT_EQUAL( day1, range.jdn1 );
            CPPUNIT_ASSERT_EQUAL( daycount-1, range.jdn2 );
            cvtdate = m_cal->range_to_str( m_sid, range );
            CPPUNIT_ASSERT_EQUAL( date, cvtdate );
            if( month == miy ) {
                month = 0;
            } else if( month == 6 ) {
                break;
            }
        }
        date = yToStr( year );
        range = m_cal->str_to_range( m_sid, date );
        CPPUNIT_ASSERT_EQUAL( month1, range.jdn1 );
        CPPUNIT_ASSERT_EQUAL( daycount-1, range.jdn2 );
        cvtdate = m_cal->range_to_str( m_sid, range );
        CPPUNIT_ASSERT_EQUAL( date, cvtdate );
    }
}

// End of test/soak/soak_h.cpp file
