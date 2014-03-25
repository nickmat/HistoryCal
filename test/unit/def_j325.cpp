/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_j325.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Julian Annunciation (j325) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     4th March 2014
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

class TestDef_j325 : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_j325 );
    CPPUNIT_TEST( testCreation );
    CPPUNIT_TEST( testStrTableInput );
    CPPUNIT_TEST( testStrTableOutput );
    CPPUNIT_TEST( testRangeShorthand );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    int             m_sid; // Scheme id

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
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_j325 );

static const char* test_strs[MaxSample][2] = {
    { "30 Jul -586", "30 Jul -586" },
    { "8 Dec -168",  "8 Dec -168"  },
    { "26 Sep 70",   "26 Sep 70"   },
    { "3 Oct 135",   "3 Oct 135"   },
    { "7 Jan 469",   "7 Jan 469/70" },
    { "18 May 576",  "18 May 576"  },
    { "7 Nov 694",   "7 Nov 694"   },
    { "19 Apr 1013", "19 Apr 1013" },
    { "18 May 1096", "18 May 1096" },
    { "16 Mar 1189", "16 Mar 1189/90" },
    { "3 Mar 1239",  "3 Mar 1239/40" },
    { "26 Mar 1288", "26 Mar 1288" },
    { "20 Apr 1298", "20 Apr 1298" },
    { "4 Jun 1391",  "4 Jun 1391"  },
    { "25 Jan 1435", "25 Jan 1435/6" },
    { "31 Mar 1492", "31 Mar 1492" },
    { "9 Sep 1553",  "9 Sep 1553"  },
    { "24 Feb 1559", "24 Feb 1559/60" },
    { "31 May 1648", "31 May 1648" },
    { "20 Jun 1680", "20 Jun 1680" },
    { "13 Jul 1716", "13 Jul 1716" },
    { "8 Jun 1768",  "8 Jun 1768"  },
    { "21 Jul 1819", "21 Jul 1819" },
    { "15 Mar 1838", "15 Mar 1838/9" },
    { "6 Apr 1903",  "6 Apr 1903"  },
    { "12 Aug 1929", "12 Aug 1929" },
    { "16 Sep 1941", "16 Sep 1941" },
    { "6 Apr 1943",  "6 Apr 1943"  },
    { "24 Sep 1943", "24 Sep 1943" },
    { "4 Mar 1991",  "4 Mar 1991/2" },
    { "12 Feb 1995", "12 Feb 1995/6" },
    { "28 Oct 2038", "28 Oct 2038" },
    { "5 Jul 2094",  "5 Jul 2094"  }
};


bool TestDef_j325::setInputOrder( const string& order )
{
    Scheme_input input;
    m_cal->get_scheme_input( &input, m_sid );
    for( size_t i = 0 ; i < input.orders.size() ; i++ ) {
        if( input.orders[i] == order ) {
            m_cal->set_scheme_order( m_sid, i );
            return true;
        }
    }
    return false;
}

bool TestDef_j325::setOutputFormat( const string& format )
{
    Scheme_output output;
    m_cal->get_scheme_output( &output, m_sid );
    for( size_t i = 0 ; i < output.formats.size() ; i++ ) {
        if( output.formats[i] == format ) {
            m_cal->set_scheme_format( m_sid, i );
            return true;
        }
    }
    return false;
}

void TestDef_j325::setUp()
{
    m_cal = new Calendars(Init_schemes_default);
    m_sid = m_cal->get_scheme_id( "j325" );
}

void TestDef_j325::tearDown()
{
    delete m_cal;
}

void TestDef_j325::testCreation()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Julian Annunciation";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "j325";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "j.sh";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "m" ) {
            str = "Month names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_j325::testStrTableInput()
{
    bool set = setInputOrder( "Day Month Year" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][0] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }

    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][1] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }
}

void TestDef_j325::testStrTableOutput()
{
    bool set = setOutputFormat( "dd Mon yyyy" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][0];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }

    set = setOutputFormat( "dd Mon yyyy/y" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][1];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }
}

void TestDef_j325::testRangeShorthand()
{
    struct data { string in; string out1; string out2; } t[] = {
        { "6 sep 1948", "6 Sep 1948", "6 Sep 1948" },
        { "23feb1942", "23 Feb 1942", "23 Feb 1942/3" },
        { "25 March 1948 ~ 24 3 1948", "1948", "1948 ~ 1948/9" },
        { "1948", "1948", "1948 ~ 1948/9" },
        { "1feb1948 ~ 28 feb 1948", "Feb 1948", "Feb 1948/9" },
        { "Feb1948", "Feb 1948", "Feb 1948/9" },
        { "1 Apr 1948 ~ 30 4 1948", "Apr 1948", "Apr 1948" },
        { "? 4 1948", "Apr 1948", "Apr 1948" },
        { 
            "Mar 1948", 
            "25 Mar 1948 ~ 31 Mar 1948 | 1 Mar 1948 ~ 24 Mar 1948",
            "25 Mar 1948 ~ 31 Mar 1948 | 1 Mar 1948/9 ~ 24 Mar 1948/9"
        },
        { "25 Mar 1948 ~ Apr 1948", "25 Mar 1948 ~ 30 Apr 1948", "25 Mar 1948 ~ 30 Apr 1948" },
        { "19 ? 1948", "", "" },
    };
    size_t count = sizeof(t) / sizeof(data);

    bool set = setInputOrder( "Day Month Year" );
    CPPUNIT_ASSERT( set == true );
    set = setOutputFormat( "dd Mon yyyy" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out1, str );
    }
    set = setOutputFormat( "dd Mon yyyy/y" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out2, str );
    }
}

// End of src/test/def_j325.cpp file
