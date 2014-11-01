/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/soak/soak_eng.cpp
 * Project:     Cal Soak test: Extended Unit Tests for Cal library API.
 * Purpose:     CppUnit for default English Hybrid calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     29th March 2014
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

class Soak_eng : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( Soak_eng );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testEngCalendar );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testScript();
    void testEngCalendar();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Soak_eng );


void Soak_eng::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script(
        "scheme \"j\" {name \"Julian\"; base julian;}"
        "scheme \"ja\" {name \"Julian Annunciation\"; shift \"j\", 1721507;}"
        "scheme \"g\" {name \"Gregorian\"; base gregorian;}"
        "scheme \"eng\" {name \"English Hybrid\";"
        " hybrid {"
        "  fields \"year\", \"month\", \"day\", \"unshift\";"
        "  scheme \"ja\";"
        "  change 2360975;"
        "  scheme \"j\";"
        "  change 2361222;"
        "  scheme \"g\";"
        " }"
        "}"
    );
    m_sid = m_cal->get_scheme( "eng" );
}

void Soak_eng::tearDown()
{
    delete m_cal;
}


void Soak_eng::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("eng");
    string name("English Hybrid");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

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


string fieldsToStr( Field s, Field y, Field m, Field d, Field h )
{
    stringstream tst;
    if( s == f_invalid ) {
        tst << "? ";
    } else {
        tst << s << " ";
    }
    tst << y << " " << m << " " << d;
    if( h != f_invalid ) {
        tst << " " << h;
    }
    return tst.str();
}

void Soak_eng::testEngCalendar()
{
    Field LengthOfMonth[2][12] = {
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
    };

    CPPUNIT_ASSERT( m_sid >= 0 );
    Field eng_year = HIS_START_YEAR-1;
    string date_in = fieldsToStr( f_invalid, eng_year, 1, 1, f_invalid );
    Field daycount = m_cal->str_to_jdn( m_sid, date_in );
    CPPUNIT_ASSERT_EQUAL( ENG_START_JDN, daycount );
    Field sch = 0;
    for( Field year = HIS_START_YEAR ; year < HIS_END_YEAR ; year++ ) {
        size_t leap_year = 0;
        if( year % 4 == 0 ) leap_year = 1;
        if( sch == 2 && year % 100 == 0 ) leap_year = 0;
        if( sch == 2 && year % 400 == 0 ) leap_year = 1;
        for( Field month = 1 ; month <= 12 ; month++ ) {
            Field month_length = LengthOfMonth[leap_year][month-1];
            for( Field day = 1 ; day <= month_length ; day++ ) {
                if( sch == 0 && month == 3 && day == 25 ) {
                    eng_year++;
                }
                if( sch > 0 && month == 1 && day == 1 ) {
                    eng_year++;
                }
                if( eng_year == 1751 && month == 12 && day == 31 ) {
                    sch++;
                }
                if( eng_year == 1752 && month == 9 && day == 3 ) {
                    sch++;
                    day = 14;
                }
                date_in = fieldsToStr( f_invalid, eng_year, month, day, f_invalid );
                Field jdn = m_cal->str_to_jdn( m_sid, date_in );
                CPPUNIT_ASSERT_EQUAL( daycount, jdn );
                Field hyear = ( sch == 0 ) ? year : f_invalid;
                string date_out = fieldsToStr( sch, eng_year, month, day, hyear );
                string cvtdate = m_cal->jdn_to_str( m_sid, jdn );
                CPPUNIT_ASSERT_EQUAL( date_out, cvtdate );
                daycount++;
            }
        }
    }
}

// End of test/soak/soak_eng.cpp file
