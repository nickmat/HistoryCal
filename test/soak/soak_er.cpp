/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/soak/soak_er.cpp
 * Project:     Cal Soak test: Extended Unit Tests for Cal library API.
 * Purpose:     CppUnit for default English Regnal calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     11th April 2014
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

class Soak_er : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( Soak_er );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testErCalendar );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    int             m_sid; // Scheme id

public:
    void setUp();
    void tearDown();

    void testScript();
    void testErCalendar();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Soak_er );


void Soak_er::setUp()
{
    m_cal = new Calendars(Init_schemes_default);
    m_sid = m_cal->get_scheme_id( "er" );
}

void Soak_er::tearDown()
{
    delete m_cal;
}


void Soak_er::testScript()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string code("er");
    string name("English Regnal");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

#if 0
#if defined( CALTEST_SHORT )
#define HIS_START_YEAR     1695
#define HIS_END_YEAR       1806
#define ENG_START_JDN      2340157
#endif

#if defined( CALTEST_LONG ) || defined( CALTEST_SOAK )
#define HIS_START_YEAR     990
#define HIS_END_YEAR       2110
#define ENG_START_JDN      2082656
#endif

#define SCHEME_1_START 2360975
#define SCHEME_2_START 2361222
#endif


static string fieldsToStr( Field d, Field m, Field y, Field e, Field s )
{
    stringstream tst;
    tst << d << " " << m << " " << y << " " << e;
    if( s >= 0 ) {
        tst << " " << s;
    }
    return tst.str();
}

static int is_leap_year( Field year )
{
    int leap = 0;
    if( year % 4 == 0 ) leap = 1;
    if( year > 1752 ) { // for England changover
        if( year % 100 == 0 ) leap = 0;
        if( year % 400 == 0 ) leap = 1;
    }
    return leap;
}

#define ER_START_JDN 2347462 // 1 Jan 1715 (j)
#define ER_END_JDN   2456659 // 1 Jan 2014 (g)

void Soak_er::testErCalendar()
{
    Field LengthOfMonth[2][12] = {
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
    };

    Field day = 0;
    Field month = 1;
    Field year = 1;
    Field era = 36;
    Field hyear = 1715;
    Field scheme = -1;
    int leapyear = is_leap_year( hyear );
    for( Field daycount = ER_START_JDN ; daycount < ER_END_JDN ; daycount++ ) {
        day++;
        if( day > LengthOfMonth[leapyear][month-1] ) {
            day = 1;
            month++;
            if( month > 12 ) {
                month = 1;
                hyear++;
                leapyear = is_leap_year( hyear );
            }
        }
        if( era == 36 && day ==  1 && month ==  8 ) year++; // Anne
        if( era == 36 && day == 11 && month ==  6 && hyear == 1727 ) { era++; year = 0; scheme++; }
        if( scheme == 0 && era == 37 && day == 11 && month ==  6 ) year++; // Geo II old style
        // Change from Julian to Gregorian
        if( scheme == 0 && day == 3 && month == 9 && hyear == 1752 ) { day = 14; scheme++; }
        if( scheme == 1 && era == 37 && day == 22 && month ==  6 ) year++; // Geo II new style
        if( era == 37 && day == 25 && month == 10 && hyear == 1760 ) { era++; year = 0; scheme = -1; }
        if( era == 38 && day == 25 && month == 10 ) year++; // Geo III
        if( era == 38 && day == 29 && month ==  1 && hyear == 1820 ) { era++; year = 0; }
        if( era == 39 && day == 29 && month ==  1 ) year++; // Geo IV
        if( era == 39 && day == 26 && month ==  6 && hyear == 1830 ) { era++; year = 0; }
        if( era == 40 && day == 26 && month ==  6 ) year++; // Wm IV
        if( era == 40 && day == 20 && month ==  6 && hyear == 1837 ) { era++; year = 0; }
        if( era == 41 && day == 20 && month ==  6 ) year++; // Vic
        if( era == 41 && day == 22 && month ==  1 && hyear == 1901 ) { era++; year = 0; }
        if( era == 42 && day == 22 && month ==  1 ) year++; // Ed VII
        if( era == 42 && day ==  6 && month ==  5 && hyear == 1910 ) { era++; year = 0; }
        if( era == 43 && day ==  6 && month ==  5 ) year++; // Geo V
        if( era == 43 && day == 20 && month ==  1 && hyear == 1936 ) { era++; year = 0; }
        if( era == 44 && day == 20 && month ==  1 ) year++; // Ed VIII
        if( era == 44 && day == 11 && month == 12 && hyear == 1936 ) { era++; year = 0; }
        if( era == 45 && day == 11 && month == 12 ) year++; // Geo VI
        if( era == 45 && day ==  6 && month ==  2 && hyear == 1936 ) { era++; year = 0; }
        if( era == 46 && day ==  6 && month ==  2 ) year++; // Eliz II
        string datestr = fieldsToStr( day, month, year, era, scheme );
        Field jdn = m_cal->str_to_jdn( m_sid, datestr );
        CPPUNIT_ASSERT_EQUAL( daycount, jdn );
    }
}

// End of test/soak/soak_er.cpp file
