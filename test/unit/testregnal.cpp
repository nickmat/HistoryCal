/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testregnal.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the regnal variant calendars.
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

using std::string;
using namespace Cal;

class TestRegnal : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestRegnal );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testSamples );
    CPPUNIT_TEST_SUITE_END();

    Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testScript();
    void testSamples();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestRegnal );


void TestRegnal::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script(
        "scheme \"g\" {name \"Gregorian\"; base gregorian;}\n"
        "scheme \"er1\" {"
        " name \"English Regnal One\";"
        " regnal {"
        "  fields \"year\", \"month\", \"day\";"
        "  era {range past~2392181; scheme \"g\";}"
        "  era {range 2392181~2415407; scheme 1 {shift \"g\", 2392181;}}" // Victoria
        "  era {range 2415407~2418798; scheme 2 {shift \"g\", 2415407;}}" // Edward VII
        "  era {range 2418798~2428188; scheme 3 {shift \"g\", 2418798;}}" // George V
        "  era {range 2428188~2428514; scheme 4 {shift \"g\", 2428188;}}" // Edward VIII
        "  era {range 2428514~2434049; scheme 5 {shift \"g\", 2428514;}}" // George VI
        "  era {range 2434049~2457023; scheme 6 {shift \"g\", 2434049;}}" // Elizabeth II
        " }"
        "}\n"
    );
    m_sid = m_cal->get_scheme( "er1" );
}

void TestRegnal::tearDown()
{
    delete m_cal;
}

void TestRegnal::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("er1");
    string name("English Regnal One");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestRegnal::testSamples()
{
    struct data { string in; string out; Field start; } t[] = {
        { "5 12 9 19", "5 12 9 19", 2432814 },
        { "5 12 9", "5 12 9", 2432796 },
        { "5 12", "5 12", 2432531 },
        { "5", "5", 2428514 }
    };
    size_t count = sizeof(t) / sizeof(data);

    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        CPPUNIT_ASSERT( rl.size() > 0 );
        CPPUNIT_ASSERT_EQUAL( t[i].start, rl[0].jdn1 );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

// End of src/test/testregnal.cpp file
