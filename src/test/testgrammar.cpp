/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/test/testgrammar.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     CppUnit tests for the Grammar system.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     27th February 2014
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

class TestGrammar : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestGrammar );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testSchemeInput );
    CPPUNIT_TEST( testSchemeOutput );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    int             m_sid; // Scheme id

public:
    void setUp();
    void tearDown();

    void testScript();
    void testSchemeInput();
    void testSchemeOutput();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestGrammar );


void TestGrammar::setUp()
{
    m_sid = -1;
    m_cal = new Calendars;
    m_cal->read_script(
        "vocab m {"
        " name Month names; lang en; style-name Month Mon;"
        " tokens {"
        "  1 January Jan; 2 Febuary Feb; 3 March Mar;"
        "  4 April Apr; 5 May; 6 June Jun;"
        "  7 July Jul; 8 August Aug; 9 September Sep;"
        "  10 October Oct; 11 November Nov; 12 December Dec;"
        " };"
        "};\n"

        "vocab w {name Weekday names; lang en; style-name Weekday WDay;"
        " tokens {"
        "  1 Monday Mon; 2 Tuesday Tue; 3 Wednesday Wed;"
        "  4 Thursday Thur; 5 Friday Fri; 6 Saturday Sat;"
        "  7 Sunday Sun;"
        " };"
        "};\n"

        "grammar j {"
        " alias field { WDay wday; Day day; Month month; Year year; };"
        " alias format-number-code { WDay w; Day dd; Month mm; Year yyyy; };"
        " alias unit { d day; m month; y year; w week; };"
        " vocabs m w;"
        " format pref @(3:Day) @(2:Month:m.a) @(1:Year);"
        " format @(4:WDay:w.a) @(3:Day) @(2:Month:m.a) @(1:Year);"
        " format @(3:Day) @(2:Month:m) @(1:Year);"
        " format @(4:WDay:w) @(3:Day) @(2:Month:m) @(1:Year);"
        " format @(2:Month:m.a) @(3:Day), @(1:Year);"
        " format @(1:Year)@:(2:Month)@:(3:Day);"
        "};\n"

        "scheme jb {name Julian Base; base julian; grammar j;};\n"
    );
    m_sid = m_cal->get_scheme_id( "jb" );
}

void TestGrammar::tearDown()
{
    delete m_cal;
}

void TestGrammar::testScript()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
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
}

void TestGrammar::testSchemeInput()
{
    Scheme_input input;
    m_cal->get_scheme_input( &input, m_sid );
    string str = "Day Month Year";
    CPPUNIT_ASSERT_EQUAL( str, input.orders[0] );
    str = "WDay Day Month Year";
    CPPUNIT_ASSERT_EQUAL( str, input.orders[1] );
    str = "Month Day Year";
    CPPUNIT_ASSERT_EQUAL( str, input.orders[2] );
    str = "Year Month Day";
    CPPUNIT_ASSERT_EQUAL( str, input.orders[3] );
    CPPUNIT_ASSERT_EQUAL( 0, input.current );
}

void TestGrammar::testSchemeOutput()
{
    Scheme_output output;
    m_cal->get_scheme_output( &output, m_sid );
    string str = "dd Mon yyyy";
    CPPUNIT_ASSERT_EQUAL( str, output.formats[0] );
    str = "WDay dd Mon yyyy";
    CPPUNIT_ASSERT_EQUAL( str, output.formats[1] );
    str = "dd Month yyyy";
    CPPUNIT_ASSERT_EQUAL( str, output.formats[2] );
    str = "Weekday dd Month yyyy";
    CPPUNIT_ASSERT_EQUAL( str, output.formats[3] );
    str = "Mon dd, yyyy";
    CPPUNIT_ASSERT_EQUAL( str, output.formats[4] );
    str = "yyyy:mm:dd";
    CPPUNIT_ASSERT_EQUAL( str, output.formats[5] );
    CPPUNIT_ASSERT_EQUAL( 0, output.current );
}

// End of src/test/testgrammar.cpp file
