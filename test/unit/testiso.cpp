/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testiso.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the ISO:8601 Formated values.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     27th March 2016
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

using namespace Cal;
using std::string;

class TestIso : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestIso );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testAddFormat );
    CPPUNIT_TEST( testInputOutput );
    CPPUNIT_TEST( testInputFormatsGreg );
    CPPUNIT_TEST( testOutputFormatsGreg );
    CPPUNIT_TEST( testInputFormatsWeek );
    CPPUNIT_TEST( testInputFormatsOrdinal );
    CPPUNIT_TEST( testDateSets );
    CPPUNIT_TEST_SUITE_END();

    Calendars*    m_cal;
    SHandle       m_sid; // Scheme handle
    SchemeFormats m_inputs;
    SchemeFormats m_outputs;

public:
    void setUp();
    void tearDown();

    int find_format( const SchemeFormats& fmts, const string& code );

    void testScript();
    void testAddFormat();
    void testInputOutput();
    void testInputFormatsGreg();
    void testOutputFormatsGreg();
    void testInputFormatsWeek();
    void testInputFormatsOrdinal();
    void testDateSets();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestIso );


void TestIso::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script(
        "grammar \"iso\" {"
        " format \"ymd\" {"
        "  rules iso8601 caldate;"
        " }"
        " format \"y6md\" {"
        "  rules iso8601 caldate year6;"
        " }"
        " format \"ywd\" {"
        "  rules iso8601 week;"
        " }"
        " format \"yd\" {"
        "  rules iso8601 ordinal;"
        " }"
        " format \"set\" {"
        "  rules iso8601 dateset;"
        " }"
        " format \"set5\" {"
        "  rules iso8601 year5 dateset;"
        " }"
        " pref \"ymd\";"
        "}"
        "scheme \"isog\" {"
        " name \"ISO:8601 Standard Date\";"
        " base gregorian;"
        " grammar \"iso\";"
        "}"
        "scheme \"isow\" {"
        " name \"ISO:8601 Standard Week\";"
        " base isoweek;"
        " grammar \"iso\";"
        "}"
        "scheme \"isoo\" {"
        " name \"ISO:8601 Standard Ordinal\";"
        " base isoordinal;"
        " grammar \"iso\";"
        "}"
    );
    m_sid = m_cal->get_scheme( "isog" );
    if( m_sid ) {
        m_cal->get_scheme_input( &m_inputs, m_sid );
        m_cal->get_scheme_input( &m_outputs, m_sid );
    }
}

void TestIso::tearDown()
{
    delete m_cal;
}

int TestIso::find_format( const SchemeFormats& fmts, const string& code )
{
    for( size_t i = 0 ; i < fmts.code.size() ; i++ ) {
        if( fmts.code[i] == code ) {
            return i;
        }
    }
    return -1;
}

void TestIso::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "ISO:8601 Standard Date";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "iso";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == 0 );

    CPPUNIT_ASSERT( m_inputs.code.size() == 6 );
    CPPUNIT_ASSERT( find_format( m_inputs, "ymd" ) >= 0 );
    CPPUNIT_ASSERT( m_outputs.code.size() == 6 ); 
    CPPUNIT_ASSERT( find_format( m_outputs, "ymd" ) >= 0 );
}

void TestIso::testAddFormat()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    // Confirm starting position
    CPPUNIT_ASSERT( m_inputs.code.size() == 6 );
    CPPUNIT_ASSERT( m_outputs.code.size() == 6 );
    string expect_err = "Error (1): Unable to create format.\n";
    string err = m_cal->run_script(
        "format \"iso:ymd\", \"(year)|-(month)|-(day)\";"
    );
    CPPUNIT_ASSERT_EQUAL( expect_err, err );
    expect_err = "";
    err = m_cal->run_script(
        "format \"iso:mdy\", \"(month) |(day), |(year)\";"
    );
    CPPUNIT_ASSERT_EQUAL( expect_err, err );
    m_cal->get_scheme_input( &m_inputs, m_sid );
    CPPUNIT_ASSERT( m_inputs.code.size() == 7 );
    m_cal->get_scheme_input( &m_outputs, m_sid );
    CPPUNIT_ASSERT( m_outputs.code.size() == 7 );
}

void TestIso::testInputOutput()
{
    struct data { string basic; string extended; } t[] = {
        { "19480919", "1948-09-19" },
        { "1948-09", "1948-09" },
        { "1948", "1948" },
        { "? 123", invalid },
    };
    size_t count = sizeof(t) / sizeof(data);

    m_cal->set_input_format( m_sid, "ymd" );
    m_cal->set_output_format( m_sid, "ymd" );
    // Output is always 'extended'
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].basic );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].extended, str );
        rl = m_cal->str_to_rangelist( m_sid, t[i].extended );
        str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].extended, str );
    }
}

void TestIso::testInputFormatsGreg()
{
    struct data { string in, in2, out; } t[] = {
        { "19480919",   "+0019480919",   "1948-09-19" },
        { "1948-09-19", "+001948-09-19", "1948-09-19" },
        { "194809",     "+00194809",     "1948-09" },  // Input not strictly legal
        { "1948-09",    "+001948-09",    "1948-09" },
        { "1948",       "+001948",       "1948" },
        { "194",        "+00194",        "1940/1949" },
        { "19",         "+0019",         "1900/1999" },
        { "1948W387",   "+001948W387",   "1948-09-19" },
        { "1948-W38-7", "+001948-W38-7", "1948-09-19" },
        { "1948-W38",   "+001948-W38",   "1948-09-13/1948-09-19" },
        { "1948W38",    "+001948W38",    "1948-09-13/1948-09-19" },
        { "1948263",    "+001948263",    "1948-09-19" },
        { "1948-263",   "+001948-263",   "1948-09-19" },
        { "[1948..1950]", "[+001948..+001950]", "1948/1950" },
        { "1943-03-08/1948-09-19",
          "+001943-03-08/+001948-09-19",
          "1943-03-08/1948-09-19" },
    };
    size_t count = sizeof(t) / sizeof(data);

    m_cal->set_input_format( m_sid, "ymd" );
    m_cal->set_output_format( m_sid, "ymd" );
    // Output is always 'extended'
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
    m_cal->set_input_format( m_sid, "y6md" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in2 );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestIso::testOutputFormatsGreg()
{
    struct data1 { string in, out; } t1[] = {
        { "1948-09-19", "1948-W38-7" },
    };
    size_t count1 = sizeof(t1) / sizeof(data1);

    m_cal->set_input_format( m_sid, "ymd" );
    m_cal->set_output_format( m_sid, "ywd" );
    // Output is always 'extended'
    for( size_t i = 0 ; i < count1 ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t1[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t1[i].out, str );
    }

    struct data2 { string in, out; } t2[] = {
        { "1948-09-19", "1948-263" },
    };
    size_t count2 = sizeof(t2) / sizeof(data2);

    m_cal->set_output_format( m_sid, "yd" );
    // Output is always 'extended'
    for( size_t i = 0 ; i < count2 ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t2[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t2[i].out, str );
    }
}

void TestIso::testInputFormatsWeek()
{
    struct data { string in, in2, out; } t[] = {
        { "19480919",   "+0019480919",   "1948-W38-7" },
        { "1948-09-19", "+001948-09-19", "1948-W38-7" },
        { "194809",     "+00194809",     "1948-W36-3/1948-W40-4" },  // Input not strictly legal
        { "1948-09",    "+001948-09",    "1948-W36-3/1948-W40-4" },
        { "1948",       "+001948",       "1948-W01-4/1948-W53-5" },
        { "194",        "+00194",        "1940-W01-1/1949-W52-6" },
        { "19",         "+0019",         "1900-W01-1/1999-W52-5" },
        { "1948W387",   "+001948W387",   "1948-W38-7" },
        { "1948-W38-7", "+001948-W38-7", "1948-W38-7" },
        { "1948-W38",   "+001948-W38",   "1948-W38" },
        { "1948W38",    "+001948W38",    "1948-W38" },
        { "1948263",    "+001948263",    "1948-W38-7" },
        { "1948-263",   "+001948-263",   "1948-W38-7" },
    };
    size_t count = sizeof(t) / sizeof(data);

    m_sid = m_cal->get_scheme( "isow" );
    m_cal->set_input_format( m_sid, "ymd" );
    m_cal->set_output_format( m_sid, "ywd" );
    // Output is always 'extended'
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
    m_cal->set_input_format( m_sid, "y6md" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in2 );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestIso::testInputFormatsOrdinal()
{
    struct data { string in, in2, out; } t[] = {
        { "19480919",   "+0019480919",   "1948-263"          },
        { "1948-09-19", "+001948-09-19", "1948-263"          },
        { "194809",     "+00194809",     "1948-245/1948-274" },  // Input not strictly legal
        { "1948-09",    "+001948-09",    "1948-245/1948-274" },
        { "1948",       "+001948",       "1948"              },
        { "194",        "+00194",        "1940/1949"         },
        { "19",         "+0019",         "1900/1999"         },
        { "1948W387",   "+001948W387",   "1948-263"          },
        { "1948-W38-7", "+001948-W38-7", "1948-263"          },
        { "1948-W38",   "+001948-W38",   "1948-257/1948-263" },
        { "1948W38",    "+001948W38",    "1948-257/1948-263" },
        { "1948263",    "+001948263",    "1948-263"          },
        { "1948-263",   "+001948-263",   "1948-263"          },
    };
    size_t count = sizeof(t) / sizeof(data);

    m_sid = m_cal->get_scheme( "isoo" );
    m_cal->set_input_format( m_sid, "ymd" );
    m_cal->set_output_format( m_sid, "yd" );
    // Output is always 'extended'
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
    m_cal->set_input_format( m_sid, "y6md" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in2 );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestIso::testDateSets()
{
    struct data { string in, in2, out; } t[] = {
        { "[1948..1950]", "[+01948..+01950]", "[1948..1950]" },
        { "[1948-09..1950-08,1950]", "[+01948-09..+01950-08,+01950]", "[1948-09..1950-12]" },
        { "1943-03-08/1948-09-19",
          "+01943-03-08/+01948-09-19",
          "[1943-03-08..1948-09-19]" },
    };
    size_t count = sizeof(t) / sizeof(data);

    m_cal->set_input_format( m_sid, "set" );
    m_cal->set_output_format( m_sid, "set" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
    m_cal->set_input_format( m_sid, "set5" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in2 );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

// End of test/unit/testformat.cpp file
