/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/test/testhybrid.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     CppUnit tests for the hybrid variant calendars.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th March 2014
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

class TestHybrid : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestHybrid );
    CPPUNIT_TEST( testScript );
//    CPPUNIT_TEST( testOverlap );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    int             m_sid; // Scheme id

public:
    void setUp();
    void tearDown();

    void testScript();
    void testOverlap();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestHybrid );


void TestHybrid::setUp()
{
    m_sid = -1;
    m_cal = new Calendars;
    // the script creates an overlap between the 3 and 13 incl. of sep 1752
    m_cal->read_script(
        "scheme j {name Julian; base julian;};\n"
        "scheme g {name Gregorian; base gregorian;};\n"
        "scheme t1 {name Test Hybrid;" 
        " hybrid {"
        "  fields scheme year month day;"
        "  scheme g {"
        "   match {year year; month month; day day;};"
        "  };"
        "  scheme j {"
        "   begin 2361222;"
        "   match {year year; month month; day day;};"
        "  };"
        " };"
        "};\n"
    );
    m_sid = m_cal->get_scheme_id( "t1" );
}

void TestHybrid::tearDown()
{
    delete m_cal;
}

void TestHybrid::testScript()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string code("t1");
    string name("Test Hybrid");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestHybrid::testOverlap()
{
    struct data { string in; string out; } t[] = {
        { "? 1752 9 2", "0 1752 9 2" },
        { "? 1752 9 3", "0 1752 9 3 | 1 1752 9 3" },
        { "? 1752 9 13", "0 1752 9 13 | 1 1752 9 13" },
        { "? 1752 9 14", "1 1752 9 14" },
    };
    size_t count = sizeof(t) / sizeof(data);

    CPPUNIT_ASSERT( m_sid >= 0 );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

// End of src/test/testhybrid.cpp file
