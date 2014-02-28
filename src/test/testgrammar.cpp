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
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    int             m_sid; // Scheme id

public:
    void setUp();
    void tearDown();

    void testScript();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestGrammar );


void TestGrammar::setUp()
{
    m_sid = -1;
    m_cal = new Calendars;
    m_cal->read_script(
        "vocab m1 {name Short Month names; style abrev; lang en; field Mon;"
        " tokens {1 Jan; 2 Feb; 3 Mar; 4 Apr; 5 May; 6 Jun;"
        " 7 Jul; 8 Aug; 9 Sep; 10 Oct; 11 Nov; 12 Dec;};"
        "};\n"
        "vocab m2 {name Full Month names; style full; lang en; field Month;"
        " tokens {1 January; 2 Febuary; 3 March; 4 April; 5 May; 6 June;"
        " 7 July; 8 August; 9 September; 10 October; 11 November; 12 December;};"
        "};\n"
        "grammar j {"
        " alias field { Day day; Month month; Year year; };"
        " alias unit { d day; m month; y year; w week; };"
        " vocabs m2 m1;"
        " format pref @(3:Day) @(2:Month:m1) @(1:Year);"
        " format @(3:Day) @(2:Month:m2) @(1:Year);"
        " format @(2:Month:m1) @(3:Day), @(1:Year);"
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
        if( info.vocab_codes[i] == "m1" ) {
            str = "Short Month names";
        } else if( info.vocab_codes[i] == "m2" ) {
            str = "Full Month names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

// End of src/test/testvocab.cpp file
