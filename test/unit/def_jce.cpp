/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_jce.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Julian Common Era calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     31st July 2015
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

using namespace std;
using namespace Cal;

class TestDef_jce : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_jce );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_jce );

static const char* test_strs[MaxSample][2] = {
    { "30 Jul 587 BCE", },
    { "8 Dec 169 BCE",  },
    { "26 Sep 70 CE",   },
    { "3 Oct 135 CE",   },
    { "7 Jan 470 CE",   },
    { "18 May 576 CE",  },
    { "7 Nov 694 CE",   },
    { "19 Apr 1013 CE", },
    { "18 May 1096 CE", },
    { "16 Mar 1190 CE", },
    { "3 Mar 1240 CE",  },
    { "26 Mar 1288 CE", },
    { "20 Apr 1298 CE", },
    { "4 Jun 1391 CE",  },
    { "25 Jan 1436 CE", },
    { "31 Mar 1492 CE", },
    { "9 Sep 1553 CE",  },
    { "24 Feb 1560 CE", },
    { "31 May 1648 CE", },
    { "20 Jun 1680 CE", },
    { "13 Jul 1716 CE", },
    { "8 Jun 1768 CE",  },
    { "21 Jul 1819 CE", },
    { "15 Mar 1839 CE", },
    { "6 Apr 1903 CE",  },
    { "12 Aug 1929 CE", },
    { "16 Sep 1941 CE", },
    { "6 Apr 1943 CE",  },
    { "24 Sep 1943 CE", },
    { "4 Mar 1992 CE",  },
    { "12 Feb 1996 CE", },
    { "28 Oct 2038 CE", },
    { "5 Jul 2094 CE",  }
};


void TestDef_jce::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "jce" );
}

void TestDef_jce::tearDown()
{
    delete m_cal;
}

void TestDef_jce::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Julian Common Era";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "jce";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "jce";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "m" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "w" ) {
            str = "Weekday names";
        }
        if( info.vocab_codes[i] == "jce" ) {
            str = "Historic Era";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_jce::testStrTableInput()
{
    bool set = setInputFormatFromDesc( m_cal, m_sid, "Day Month Year CEra" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][0] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }
}

void TestDef_jce::testStrTableOutput()
{
    bool set = setOutputFormatFromDesc( m_cal, m_sid, "dd Mon yyyy CE" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][0];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }
}

void TestDef_jce::testRangeShorthand()
{
    struct data { string in; string out1; string out2; } t[] = {
        { "44 BCE", "44 BCE" },
        { "6 sep 1948 CE", "6 Sep 1948 CE" },
        { "1948ce", "1948 CE" },
        { "1feb1948ce ~ 29 Feb 1948 CE", "Feb 1948 CE" },
        { "Feb1948ce", "Feb 1948 CE" },
        { "1 Apr 1948 CE ~ 30 4 1948 1", "Apr 1948 CE" },
    };
    size_t count = sizeof(t) / sizeof(data);

    bool set = setInputFormatFromDesc( m_cal, m_sid, "Day Month Year CEra" );
    CPPUNIT_ASSERT( set == true );
    set = setOutputFormatFromDesc( m_cal, m_sid, "dd Mon yyyy CE" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out1, str );
    }
}

// End of test/unit/def_jce.cpp file
