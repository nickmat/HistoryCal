/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_isoo.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the default ISO Ordinal (isoo) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     22nd March 2016
 * Copyright:   Copyright (c) 2016 .. 2020, Nick Matthews.
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

class TestDef_isoo : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_isoo );
    CPPUNIT_TEST( testCreation );
    CPPUNIT_TEST( testStrTableInput );
    CPPUNIT_TEST( testStrTableOutput );
    CPPUNIT_TEST( testRangeShorthand );
    CPPUNIT_TEST( testISOFormats );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testCreation();
    void testStrTableInput();
    void testStrTableOutput();
    void testRangeShorthand();
    void testISOFormats();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_isoo );

static const char* test_strs[MaxSample][2] = {
    { "-586 205", "-0586-205", },
    { "-168 340", "-0168-340", },
    { "70 267",   "0070-267",  },
    { "135 275",  "0135-275",  },
    { "470 8",    "0470-008",  },
    { "576 141",  "0576-141",  },
    { "694 314",  "0694-314",  },
    { "1013 115", "1013-115",  },
    { "1096 145", "1096-145",  },
    { "1190 82",  "1190-082",  },
    { "1240 70",  "1240-070",  },
    { "1288 93",  "1288-093",  },
    { "1298 117", "1298-117",  },
    { "1391 163", "1391-163",  },
    { "1436 34",  "1436-034",  },
    { "1492 100", "1492-100",  },
    { "1553 262", "1553-262",  },
    { "1560 65",  "1560-065",  },
    { "1648 162", "1648-162",  },
    { "1680 182", "1680-182",  },
    { "1716 206", "1716-206",  },
    { "1768 171", "1768-171",  },
    { "1819 214", "1819-214",  },
    { "1839 86",  "1839-086",  },
    { "1903 109", "1903-109",  },
    { "1929 237", "1929-237",  },
    { "1941 272", "1941-272",  },
    { "1943 109", "1943-109",  },
    { "1943 280", "1943-280",  },
    { "1992 77",  "1992-077",  },
    { "1996 56",  "1996-056",  },
    { "2038 314", "2038-314",  },
    { "2094 199", "2094-199",  }
};


void TestDef_isoo::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_cal->run_script(
        "format \"isoo:-yd\" {\n"
        " rules iso8601 ordinal minus;\n"
        "}\n"
    );
    m_sid = m_cal->get_scheme( "isoo" );
}

void TestDef_isoo::tearDown()
{
    delete m_cal;
}

void TestDef_isoo::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "ISO:8601 Standard Ordinal";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "isoo";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "isoo";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
}

void TestDef_isoo::testStrTableInput()
{
    bool set = setInputFormatFromDesc( m_cal, m_sid, "Year Day" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][0] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }

    set = setInputFormatFromDesc( m_cal, m_sid, "ISO:8601 Ordinal" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][1] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }
}

void TestDef_isoo::testStrTableOutput()
{
    bool set = setOutputFormatFromDesc( m_cal, m_sid, "yyyy ddd" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][0];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }

    set = setOutputFormatFromDesc( m_cal, m_sid, "YYYY-DDD" );
    CPPUNIT_ASSERT( set == true );
    // Note, default script doesn't cope with negative years
    for( size_t i = 0 ; i < 2 ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( string(), str );
    }
    for( size_t i = 2 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][1];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }

    set = setOutputFormatFromDesc( m_cal, m_sid, "-YYYY-DDD" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][1];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }
}

void TestDef_isoo::testRangeShorthand()
{
    struct data { string in; string out; } t[] = {
        { "1948 38", "1948 38" },
        { "1948", "1948" },
        { "? 123", invalid }
    };
    size_t count = sizeof(t) / sizeof(data);

    bool set = setInputFormatFromDesc( m_cal, m_sid, "Year Day" );
    CPPUNIT_ASSERT( set == true );
    set = setOutputFormatFromDesc( m_cal, m_sid, "yyyy ddd" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestDef_isoo::testISOFormats()
{
    struct data { string in, out, jdn; } t[] = {
        { "1948-263", "1948-263", "2432814" },
        { "1948", "1948", "2432552 .. 2432917" },
    };
    size_t count = sizeof(t) / sizeof(data);

    bool set = setInputFormatFromDesc( m_cal, m_sid, "ISO:8601 Ordinal" );
    CPPUNIT_ASSERT( set == true );
    set = setOutputFormatFromDesc( m_cal, m_sid, "-YYYY-DDD" );
    CPPUNIT_ASSERT( set == true );
    SHandle jdnid = m_cal->get_scheme( "jdn" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( jdnid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].jdn, str );
        str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

// End of src/test/def_isoo.cpp file
