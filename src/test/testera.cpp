/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/test/testera.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     CppUnit tests for the era variant calendars.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     24th March 2014
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

class TestEra : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestEra );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testOverlap );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestEra );


void TestEra::setUp()
{
    m_sid = -1;
    m_cal = new Calendars;
    m_cal->read_script(
        "scheme g {name Gregorian; base gregorian;};\n"
        "scheme er1 {"
        " name English Regnal One;"
        " era g year {"
        // 0 Pre Regnal
        "  1721426 -2147483646;" // Gregorian Epoch
        // 1 Vic            2 Ed VII         3 Geo V
        "  2392181 2392181; 2415407 2415407; 2418798 2418798;"
        // 4 Ed VII         5 Geo VI         6 Eliz II
        "  2428188 2428188; 2428514 2428514; 2434049 2434049;"
        " };"
        "};\n"
    );
    m_sid = m_cal->get_scheme_id( "er1" );
}

void TestEra::tearDown()
{
    delete m_cal;
}

void TestEra::testScript()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    string code("er1");
    string name("English Regnal One");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestEra::testOverlap()
{
    struct data { string in; string out; } t[] = {
        { "5 12 9 19", "5 12 9 19" }
    };
    size_t count = sizeof(t) / sizeof(data);

    CPPUNIT_ASSERT( m_sid >= 0 );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

// End of src/test/testera.cpp file
