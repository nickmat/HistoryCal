/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/soak/soak_fr.cpp
 * Project:     Cal Soak test: Extended Unit Tests for Cal library API.
 * Purpose:     CppUnit soak tests for the French Republican calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     9th December 2014
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

class SoakFrenchRep : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( SoakFrenchRep );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( TestFrench );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();
    Field FrenchLastDayInMonth( Field year, Field month );

    void testScript();
    void TestFrench();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( SoakFrenchRep );


void SoakFrenchRep::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"fr\" {name \"French Republican\"; base french;}" );
    m_sid = m_cal->get_scheme( "fr" );
}

void SoakFrenchRep::tearDown()
{
    delete m_cal;
}

void SoakFrenchRep::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("fr");
    string name("French Republican");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}



#ifdef CALTEST_SHORT
#define CALTEST_FR_START_YEAR     0
#define CALTEST_FR_START_JDN      2375475
#define CALTEST_FR_END_YEAR       50
#endif

#ifdef CALTEST_LONG
#define CALTEST_FR_START_YEAR     0
#define CALTEST_FR_START_JDN      2375475
#define CALTEST_FR_END_YEAR       230
#endif

#ifdef CALTEST_SOAK
#define CALTEST_FR_START_YEAR     0
#define CALTEST_FR_START_JDN      2375475
#define CALTEST_FR_END_YEAR       230
#endif

Field SoakFrenchRep::FrenchLastDayInMonth( Field year, Field month )
{
    if( month == 13 ) {
        FieldVec fields( 3, f_invalid );
        fields[0] = year+1;
        fields[1] = 1;
        fields[2] = 1;
        Field jdn = m_cal->fieldvec_to_jdn( m_sid, fields );
        FieldVec fields_r = m_cal->jdn_to_fieldvec( m_sid, jdn - 1 );
        return fields_r[2];
    }
    return 30;
}

void SoakFrenchRep::TestFrench()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string date = ymdToStr( CALTEST_FR_START_YEAR, 1, 1 );
    Field daycount = m_cal->str_to_jdn( m_sid, date );
    CPPUNIT_ASSERT_EQUAL( daycount, CALTEST_FR_START_JDN );
    string cvtdate;
    Range range;
    for( Field year = CALTEST_FR_START_YEAR ; year < CALTEST_FR_END_YEAR ; year++ ) {
        Field month1 = daycount;
        for( Field month = 1 ; month <= 13 ; month++ ) {
            int month_length = FrenchLastDayInMonth( year, month );
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

// End of test/soak/soak_fr.cpp file
