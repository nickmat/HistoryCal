/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testhybrid.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
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
    CPPUNIT_TEST( testOverlap );
    CPPUNIT_TEST( testGap );
    CPPUNIT_TEST( testOverlap2 );
    CPPUNIT_TEST( testGap2 );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;

public:
    void setUp();
    void tearDown();

    void testScript();
    void testOverlap();
    void testGap();
    void testOverlap2();
    void testGap2();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestHybrid );


void TestHybrid::setUp()
{
    m_cal = new Calendars;
    m_cal->run_script(
        "scheme \"j\" {name \"Julian\"; base julian;}\n"
        "scheme \"ja\" {name \"Julian Annunciation\"; shift \"j\", 1721507;}\n"
        "scheme \"g\" {name \"Gregorian\"; base gregorian;}\n"

        // This creates an overlap between the 3 and 13 incl. of sep 1752
        "scheme \"t1\" {name \"Test Hybrid Overlap\";" 
        " hybrid {"
        "  fields \"year\", \"month\", \"day\";"
        "  scheme \"g\";"
        "  change 2361222;"
        "  scheme \"j\";"
        " }"
        "}\n"
        // This creates an gap between the 3 and 13 incl. of sep 1752
        "scheme \"t2\" {name \"Test Hybrid Gap\";" 
        " hybrid {"
        "  fields \"year\", \"month\", \"day\";"
        "  scheme \"j\";"
        "  change 2361222;"
        "  scheme \"g\";"
        " }"
        "}\n"
        // This creates an overlap between the 1 Jan the 24 Mar 1751 incl.
        "scheme \"t3\" {name \"Test Hybrid Overlap2\";" 
        " hybrid {"
        "  fields \"year\", \"month\", \"day\";"
        "  scheme \"j\";"
        "  change 2360976;"
        "  scheme \"ja\";"
        " }"
        "}\n"
        // This creates an gap between the 1 Jan the 24 Mar 1751 incl.
        "scheme \"t4\" {name \"Test Hybrid Gap2\";" 
        " hybrid {"
        "  fields \"year\", \"month\", \"day\";"
        "  scheme \"ja\";"
        "  change 2360976;"
        "  scheme \"j\";"
        " }"
        "}\n"
    );
}

void TestHybrid::tearDown()
{
    delete m_cal;
}

void TestHybrid::testScript()
{
    Cal::SHandle sid = m_cal->get_scheme( "t1" );
    CPPUNIT_ASSERT( sid != NULL );
    string code("t1");
    string name("Test Hybrid Overlap");
    Scheme_info info;
    m_cal->get_scheme_info( &info, sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestHybrid::testOverlap()
{
    struct data { string in; string out; } t[] = {
        { "1752 9 2", "0 1752 9 2" },
        { "1752 9 3", "0 1752 9 3 | 1 1752 9 3" },
        { "1752 9 13", "0 1752 9 13 | 1 1752 9 13" },
        { "1752 9 14", "1 1752 9 14" },
        { "1752 9", "0 1752 9 ~ 1 1752 9" },
    };
    size_t count = sizeof(t) / sizeof(data);

    Cal::SHandle sid = m_cal->get_scheme( "t1" );
    CPPUNIT_ASSERT( sid >= 0 );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( sid, t[i].in );
        string str = m_cal->rangelist_to_str( sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestHybrid::testGap()
{
    struct data { string in; string out; } t[] = {
        { "1752 9 2", "0 1752 9 2" },
        { "1752 9 3", "" },
        { "1752 9 13", "" },
        { "1752 9 14", "1 1752 9 14" },
        { "1752 9", "0 1752 9 ~ 1 1752 9" },
    };
    size_t count = sizeof(t) / sizeof(data);

    Cal::SHandle sid = m_cal->get_scheme( "t2" );
    CPPUNIT_ASSERT( sid >= 0 );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( sid, t[i].in );
        string str = m_cal->rangelist_to_str( sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestHybrid::testOverlap2()
{
    struct data { string in; string out; } t[] = {
        { "1750 12 31", "0 1750 12 31" },
        { "1751 1 1", "0 1751 1 1 | 1 1751 1 1" },
        { "1751 3 24", "0 1751 3 24 | 1 1751 3 24" },
        { "1751 3 25", "0 1751 3 25" },
        { "1751 12 31", "0 1751 12 31" },
        { "1752 1 1", "1 1752 1 1" },
        { "1751", "0 1751 1 1 ~ 1 1751 3 24" },
        { "1751 2", "0 1751 2 | 1 1751 2" },
    };
    size_t count = sizeof(t) / sizeof(data);

    Cal::SHandle sid = m_cal->get_scheme( "t3" );
    CPPUNIT_ASSERT( sid >= 0 );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( sid, t[i].in );
        string str = m_cal->rangelist_to_str( sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestHybrid::testGap2()
{
    struct data { string in; string out; } t[] = {
        { "1750 12 31", "0 1750 12 31" },
        { "1751 1 1", "" },
        { "1751 3 24", "" },
        { "1751 3 25", "0 1751 3 25" },
        { "1751 12 31", "0 1751 12 31" },
        { "1752 1 1", "1 1752 1 1" },
        { "1751", "0 1751 3 25 ~ 0 1751 12 31" },
    };
    size_t count = sizeof(t) / sizeof(data);

    Cal::SHandle sid = m_cal->get_scheme( "t4" );
    CPPUNIT_ASSERT( sid >= 0 );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( sid, t[i].in );
        string str = m_cal->rangelist_to_str( sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

// End of src/test/testhybrid.cpp file
