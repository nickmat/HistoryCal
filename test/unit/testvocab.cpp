/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testvocab.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
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
    m_cal->run_script(
        "vocab m {"
        " name Month names; lang en; style-name Month Mon;"
        " tokens {"
        "  1 January Jan; 2 Febuary Feb; 3 March Mar;"
        "  4 April Apr; 5 May; 6 June Jun;"
        "  7 July Jul; 8 August Aug; 9 September Sep;"
        "  10 October Oct; 11 November Nov; 12 December Dec;"
        " };"
        "};\n"
    );
}

void TestVocab::tearDown()
{
    delete m_cal;
}

void TestVocab::testScript()
{
    string code("m");
    Vocab_info info;
    m_cal->get_vocab_info( &info, code );
    string str = "Month names";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "en";
    CPPUNIT_ASSERT_EQUAL( str, info.lang );
    str = "Month";
    CPPUNIT_ASSERT_EQUAL( str, info.style_full_name );
    str = "Mon";
    CPPUNIT_ASSERT_EQUAL( str, info.style_abbrev_name );

    str = "January";
    CPPUNIT_ASSERT_EQUAL( str, info.words[1-1] );
    str = "Jan";
    CPPUNIT_ASSERT_EQUAL( str, info.abbrevs[1-1] );
    str = "September";
    CPPUNIT_ASSERT_EQUAL( str, info.words[9-1] );
    str = "Sep";
    CPPUNIT_ASSERT_EQUAL( str, info.abbrevs[9-1] );
}

// End of src/test/testvocab.cpp file
