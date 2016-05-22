/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_isoo.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the default ISO Ordinal (isoo) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     22nd March 2016
 * Copyright:   Copyright (c) 2016, Nick Matthews.
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
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_isoo );

static const char* test_strs[MaxSample][1] = {
    { "-586 205", },
    { "-168 340", },
    { "70 267",   },
    { "135 275",  },
    { "470 8",    },
    { "576 141",  },
    { "694 314",  },
    { "1013 115", },
    { "1096 145", },
    { "1190 82",  },
    { "1240 70",  },
    { "1288 93",  },
    { "1298 117", },
    { "1391 163", },
    { "1436 34",  },
    { "1492 100", },
    { "1553 262", },
    { "1560 65",  },
    { "1648 162", },
    { "1680 182", },
    { "1716 206", },
    { "1768 171", },
    { "1819 214", },
    { "1839 86",  },
    { "1903 109", },
    { "1929 237", },
    { "1941 272", },
    { "1943 109", },
    { "1943 280", },
    { "1992 77",  },
    { "1996 56",  },
    { "2038 314", },
    { "2094 199", }
};


void TestDef_isoo::setUp()
{
    m_cal = new Calendars(Init_script_default);
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

// End of src/test/def_isoo.cpp file
