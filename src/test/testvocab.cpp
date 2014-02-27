/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/test/testvocab.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     CppUnit tests for the Vocab system.
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

class TestVocab : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestVocab );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;

public:
    void setUp();
    void tearDown();

    void testScript();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestVocab );


void TestVocab::setUp()
{
    m_cal = new Calendars;
    m_cal->read_script(
        "vocab m2 {name Full Month names; style full; lang en; field Month;"
        " tokens {1 January; 2 Febuary; 3 March; 4 April; 5 May; 6 June;"
        " 7 July; 8 August; 9 September; 10 October; 11 November; 12 December;};"
        "};\n"
    );
}

void TestVocab::tearDown()
{
    delete m_cal;
}

void TestVocab::testScript()
{
    string code("m2");
    Vocab_info info;
    m_cal->get_vocab_info( &info, code );
    string str = "Full Month names";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "full";
    CPPUNIT_ASSERT_EQUAL( str, info.style );
    str = "en";
    CPPUNIT_ASSERT_EQUAL( str, info.lang );
    str = "Month";
    CPPUNIT_ASSERT_EQUAL( str, info.field );

    str = "January";
    CPPUNIT_ASSERT_EQUAL( str, info.tokens[1-1] );
    str = "September";
    CPPUNIT_ASSERT_EQUAL( str, info.tokens[9-1] );
}

// End of src/test/testvocab.cpp file
