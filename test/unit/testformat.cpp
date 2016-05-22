/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testformat.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the Format system.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st March 2015
 * Copyright:   Copyright (c) 2015 ~ 2016, Nick Matthews.
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

class TestFormat : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestFormat );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testAddFormat );
    CPPUNIT_TEST( testMarkAddFormat );
    CPPUNIT_TEST( testUseFormat );
    CPPUNIT_TEST_SUITE_END();

    Calendars* m_cal;
    SHandle    m_sid; // Scheme handle
    FormatInfo m_inputs;
    FormatInfo m_outputs;

public:
    void setUp();
    void tearDown();

    void testScript();
    void testAddFormat();
    void testMarkAddFormat();
    void testUseFormat();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestFormat );


void TestFormat::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script(
        "vocab \"m\" {"
        " name \"Month names\"; lang \"en\"; stylename \"Month\", \"Mon\";"
        " tokens {"
        "  1, \"January\", \"Jan\"; 2, \"Febuary\", \"Feb\"; 3, \"March\", \"Mar\";"
        "  4, \"April\", \"Apr\"; 5, \"May\"; 6, \"June\", \"Jun\";"
        "  7, \"July\", \"Jul\"; 8, \"August\", \"Aug\"; 9, \"September\", \"Sep\";"
        "  10, \"October\", \"Oct\"; 11, \"November\", \"Nov\"; 12, \"December\", \"Dec\";"
        " }"
        "}\n"

        "vocab \"w\" {"
        " name \"Weekday names\"; lang \"en\"; stylename \"Weekday\", \"WDay\";"
        " tokens {"
        "  1, \"Monday\", \"Mon\"; 2, \"Tuesday\", \"Tue\"; 3, \"Wednesday\", \"Wed\";"
        "  4, \"Thursday\", \"Thur\"; 5, \"Friday\", \"Fri\"; 6, \"Saturday\", \"Sat\";"
        "  7, \"Sunday\", \"Sun\";"
        " }"
        "}\n"

        "grammar \"j\" {"
        " alias field {"
        "  \"WDay\",\"wday\"; \"Day\",\"day\"; \"Month\",\"month\"; \"Year\",\"year\";"
        " }"
        " alias stylename {"
        "  \"WDay\",\"w\"; \"Day\",\"dd\"; \"Month\",\"mm\"; \"Year\",\"yyyy\";"
        " }"
        " alias unit {"
        "  \"d\",\"day\"; \"m\",\"month\"; \"y\",\"year\"; \"w\",\"week\";"
        " }"
        " vocabs \"m\", \"w\";"
        " format \"dmy\", \"|(Day) |(Month:m.a) |(Year)\";"
        " format \"wdmy\", \"|(WDay:w.a) |(Day) |(Month:m.a) |(Year)\";"
        " pref \"dmy\";"
        "}\n"

        "scheme \"jb\" {name \"Julian Base\"; base julian; grammar \"j\";}\n"
    );
    m_sid = m_cal->get_scheme( "jb" );
    if( m_sid ) {
        m_cal->get_input_info( &m_inputs, m_sid );
        m_cal->get_output_info( &m_outputs, m_sid );
    }
}

void TestFormat::tearDown()
{
    delete m_cal;
}

void TestFormat::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Julian Base";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "j";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "m" ) {
            str = "Month names";
        } else if( info.vocab_codes[i] == "w" ) {
            str = "Weekday names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
    CPPUNIT_ASSERT( m_inputs.descs.size() == 2 );
    CPPUNIT_ASSERT( find_format( m_inputs, "dmy" ) >= 0 );
    CPPUNIT_ASSERT( find_format( m_inputs, "wdmy" ) >= 0 );
    CPPUNIT_ASSERT( m_outputs.descs.size() == 2 ); 
    CPPUNIT_ASSERT( find_format( m_outputs, "dmy" ) >= 0 );
    CPPUNIT_ASSERT( find_format( m_outputs, "wdmy" ) >= 0 );
}

void TestFormat::testAddFormat()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    // Confirm starting position
    CPPUNIT_ASSERT( m_inputs.descs.size() == 2 );
    CPPUNIT_ASSERT( m_outputs.descs.size() == 2 );
    string expect_err = "Error (1): Unable to create format.\n";
    string err = m_cal->run_script(
        "format \"j:dmy\", \"|(Day) |(Month:m.a) |(Year)\";"
    );
    CPPUNIT_ASSERT_EQUAL( expect_err, err );
    expect_err = "";
    err = m_cal->run_script(
        "format \"j:mdy\", \"|(Month:m.a) |(Day), |(Year)\";"
    );
    CPPUNIT_ASSERT_EQUAL( expect_err, err );
    m_cal->get_input_info( &m_inputs, m_sid );
    CPPUNIT_ASSERT( m_inputs.descs.size() == 3 );
    m_cal->get_output_info( &m_outputs, m_sid );
    CPPUNIT_ASSERT( m_outputs.descs.size() == 3 );
}

void TestFormat::testMarkAddFormat()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    // Confirm starting position
    CPPUNIT_ASSERT( m_inputs.descs.size() == 2 );
    CPPUNIT_ASSERT( m_outputs.descs.size() == 2 );
    string expect_err;
    string err = m_cal->run_script(
        "mark \"test\";"
        "format \"j:mdy\", \"|(Month:m.a) |(Day), |(Year)\";"
    );
    CPPUNIT_ASSERT_EQUAL( expect_err, err );
    m_cal->get_input_info( &m_inputs, m_sid );
    CPPUNIT_ASSERT( m_inputs.descs.size() == 3 );
    m_cal->get_output_info( &m_outputs, m_sid );
    CPPUNIT_ASSERT( m_outputs.descs.size() == 3 );

    err = m_cal->run_script(
        "mark \"test\";"
    );
    CPPUNIT_ASSERT_EQUAL( expect_err, err );
    m_cal->get_input_info( &m_inputs, m_sid );
    CPPUNIT_ASSERT( m_inputs.descs.size() == 2 );
    m_cal->get_output_info( &m_outputs, m_sid );
    CPPUNIT_ASSERT( m_outputs.descs.size() == 2 );
}

void TestFormat::testUseFormat()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    m_cal->run_script(
        "format \"j:mdy\", \"|(Month:m.a) |(Day), |(Year)\";"
    );
    Field jdn1 = m_cal->str_to_jdn( m_sid, "6Sep1948", "dmy" );
    CPPUNIT_ASSERT( jdn1 != f_invalid );
    Field jdn2 = m_cal->str_to_jdn( m_sid, "Sep6,1948", "mdy" );
    CPPUNIT_ASSERT( jdn1 == jdn2 );
}

// End of test/unit/testformat.cpp file
