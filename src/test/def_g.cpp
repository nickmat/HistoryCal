/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/test/def_g.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     CppUnit tests for the default Gregorian (g) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th February 2014
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

class TestDef_g : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_g );
    CPPUNIT_TEST( testCreation );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    int             m_sid; // Scheme id

public:
    void setUp();
    void tearDown();

    void testCreation();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_g );


void TestDef_g::setUp()
{
    m_cal = new Calendars(Init_schemes_default);
    m_sid = m_cal->get_scheme_id( "g" );
}

void TestDef_g::tearDown()
{
    delete m_cal;
}

void TestDef_g::testCreation()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Gregorian";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "g";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "j";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "m1" ) {
            str = "Short Month names";
        } else if( info.vocab_codes[i] == "m2" ) {
            str = "Full Month names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

// End of src/test/def_g.cpp file
