/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/soak/soak_j325.cpp
 * Project:     Cal Soak test: Extended Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Julian Annunciation (j325) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     4th March 2014
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

class Soak_j325 : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( Soak_j325 );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testJ325Calendar );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    int             m_sid; // Scheme id

public:
    void setUp();
    void tearDown();

    void testScript();
    void testJ325Calendar();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Soak_j325 );


void Soak_j325::setUp()
{
    m_sid = -1;
    m_cal = new Calendars;
    m_cal->read_script(
        "scheme jb {name Julian Base; base julian;};"
        "scheme j325 {name Julian Lady Day; shift jb 1721142;};"
    );
    m_sid = m_cal->get_scheme_id( "j325" );
}

void Soak_j325::tearDown()
{
    delete m_cal;
}


void Soak_j325::testScript()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string code("j325");
    string name("Julian Lady Day");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

#if defined( CALTEST_SHORT )
#define J325_START_YEAR     1495     // div by 4 minus 1 (a leap year)
#define J325_START_JDN      2267472
#define J325_END_YEAR       1506
#endif

#if defined( CALTEST_LONG ) || defined( CALTEST_SOAK )
#define J325_START_YEAR     795     // div by 4 minus 1 (a leap year)
#define J325_START_JDN      2011797
#define J325_END_YEAR       2110
#endif

void Soak_j325::testJ325Calendar()
{
    Field LengthOfMonth[2][12] = {
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
    };

    CPPUNIT_ASSERT( m_sid >= 0 );
    string date = ymdToStr( J325_START_YEAR, 1, 1 );
    Field daycount = m_cal->str_to_jdn( m_sid, date );
    CPPUNIT_ASSERT_EQUAL( daycount, J325_START_JDN );

    Field j325_year = J325_START_YEAR;
    Field year = j325_year;
    int leap_count = 0;
    size_t leap_year = 1;
    for( Field year = J325_START_YEAR ; year < J325_END_YEAR ;  ) {
        year++;
        for( Field month = 1 ; month <= 12 ; month++ ) {
            Field month_length = LengthOfMonth[leap_year][month-1];
            for( Field day = 1 ; day <= month_length ; day++ ) {
                if( month == 3 && day == 25 ) {
                    j325_year++;
                    leap_count++;
                    if( leap_count == 4 ) {
                        leap_count = 0;
                        leap_year = 1;
                    } else {
                        leap_year = 0;
                    }
                }
                date = ymdToStr( j325_year, month, day );
                Field jdn = m_cal->str_to_jdn( m_sid, date );
                CPPUNIT_ASSERT_EQUAL( daycount, jdn );
                string cvtdate = m_cal->jdn_to_str( m_sid, jdn );
                CPPUNIT_ASSERT_EQUAL( date, cvtdate );
                daycount++;
            }
        }
    }
}

// End of test/soak/soak_j325.cpp file
