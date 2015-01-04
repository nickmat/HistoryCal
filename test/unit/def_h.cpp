/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_h.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Hebrew (h) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     4th January 2015
 * Copyright:   Copyright (c) 2015, Nick Matthews.
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

using namespace Cal;
using std::string;

class TestDef_h : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_h );
    CPPUNIT_TEST( testCreation );
//    CPPUNIT_TEST( testStrTable );
    CPPUNIT_TEST( testStringInput );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testCreation();
//    void testStrTable();
    void testStringInput();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_h );

void TestDef_h::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "h" );
}

void TestDef_h::tearDown()
{
    delete m_cal;
}

void TestDef_h::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Hebrew";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "h";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "h";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "hm" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "hw" ) {
            str = "Weekday names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_h::testStringInput()
{
    struct data { string in; string fmt; string out; Field beg; Field end; } t[] = {
        { "5708", "dmy", "5708", 2432444, 2432828 },
        { "ell5708", "dmy", "Elul 5708", 2432800, 2432828 },
        { "Rish15ell5708", "wdmy", "Yom Rishon 15 Elul 5708", 2432814, 2432814 },
        { "15ell5708", "dmy", "15 Elul 5708", 2432814, 2432814 },
    };
    size_t count = sizeof(t) / sizeof(data);

    for( size_t i = 0 ; i < count ; i++ ) {
        m_cal->set_input_format( m_sid, t[i].fmt );
        m_cal->set_output_format( m_sid, t[i].fmt );
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
        if( rl.size() ) {
            CPPUNIT_ASSERT_EQUAL( t[i].beg, rl[0].jdn1 );
            size_t j = rl.size() - 1;
            CPPUNIT_ASSERT_EQUAL( t[i].end, rl[j].jdn2 );
        }
    }
}

// End of src/test/def_h.cpp file
