/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_isow.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the default ISO Week (isow) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     17th March 2016
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

class TestDef_isow : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_isow );
    CPPUNIT_TEST( testCreation );
    CPPUNIT_TEST( testStrTableInput );
    CPPUNIT_TEST( testStrTableOutput );
    CPPUNIT_TEST( testRangeShorthand );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    bool setInputOrder( const string& order );
    bool setOutputFormat( const string& format );

    void setUp();
    void tearDown();

    void testCreation();
    void testStrTableInput();
    void testStrTableOutput();
    void testRangeShorthand();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_isow );

static const char* test_strs[MaxSample][1] = {
    { "-586 29 7", },
    { "-168 49 3", },
    { "70 39 3",  },
    { "135 39 7", },
    { "470 2 3", },
    { "576 21 1", },
    { "694 45 6", },
    { "1013 16 7", },
    { "1096 21 7", },
    { "1190 12 5", },
    { "1240 10 6", },
    { "1288 14 5", },
    { "1298 17 7", },
    { "1391 23 7", },
    { "1436 5 3", },
    { "1492 14 6", },
    { "1553 38 6", },
    { "1560 9 6", },
    { "1648 24 3", },
    { "1680 26 7", },
    { "1716 30 5", },
    { "1768 24 7", },
    { "1819 31 1", },
    { "1839 13 3", },
    { "1903 16 7", },
    { "1929 34 7", },
    { "1941 40 1", },
    { "1943 16 1", },
    { "1943 40 4", },
    { "1992 12 2", },
    { "1996 8 7", },
    { "2038 45 3", },
    { "2094 28 7", }
};


bool TestDef_isow::setInputOrder( const string& order )
{
    SchemeFormats input;
    m_cal->get_scheme_input( &input, m_sid );
    for( size_t i = 0 ; i < input.code.size() ; i++ ) {
        if( input.descrip[i] == order ) {
            m_cal->set_input_format( m_sid, input.code[i] );
            return true;
        }
    }
    return false;
}

bool TestDef_isow::setOutputFormat( const string& format )
{
    SchemeFormats output;
    m_cal->get_scheme_output( &output, m_sid );
    for( size_t i = 0 ; i < output.descrip.size() ; i++ ) {
        if( output.descrip[i] == format ) {
            m_cal->set_output_format( m_sid, output.code[i] );
            return true;
        }
    }
    return false;
}

void TestDef_isow::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "isow" );
}

void TestDef_isow::tearDown()
{
    delete m_cal;
}

void TestDef_isow::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "ISO Week";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "isow";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "isow";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "w" ) {
            str = "Weekday names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_isow::testStrTableInput()
{
    bool set = setInputOrder( "Year Week Day" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][0] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }
}

void TestDef_isow::testStrTableOutput()
{
    bool set = setOutputFormat( "yyyy ww d" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][0];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }
}

void TestDef_isow::testRangeShorthand()
{
    struct data { string in; string out; } t[] = {
        { "1948 38sun", "1948 38 7" },
        { "1948 38", "1948 38" },
        { "1948", "1948" },
        { "1948 ? 7", invalid }
    };
    size_t count = sizeof(t) / sizeof(data);

    bool set = setInputOrder( "Year Week Day" );
    CPPUNIT_ASSERT( set == true );
    set = setOutputFormat( "yyyy ww d" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

// End of src/test/def_isow.cpp file
