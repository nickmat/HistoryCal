/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_eng.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default English Hybrid (eng) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     6th March 2014
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

class TestDef_eng : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_eng );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_eng );

static const char* test_strs[MaxSample][2] = {
    // Start with Julian Annunciation
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
    // No example of Julian CE
    // Change to gregorian
    { "19 Jun 1768", "19 Jun 1768" },
    { "2 Aug 1819",  "2 Aug 1819"  },
    { "27 Mar 1839", "27 Mar 1839" },
    { "19 Apr 1903", "19 Apr 1903" },
    { "25 Aug 1929", "25 Aug 1929" },
    { "29 Sep 1941", "29 Sep 1941" },
    { "19 Apr 1943", "19 Apr 1943" },
    { "7 Oct 1943",  "7 Oct 1943"  },
    { "17 Mar 1992", "17 Mar 1992" },
    { "25 Feb 1996", "25 Feb 1996" },
    { "10 Nov 2038", "10 Nov 2038" },
    { "18 Jul 2094", "18 Jul 2094" }
};


bool TestDef_eng::setInputOrder( const string& order )
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

bool TestDef_eng::setOutputFormat( const string& format )
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

void TestDef_eng::setUp()
{
    m_cal = new Calendars(Init_schemes_default);
    m_sid = m_cal->get_scheme_id( "eng" );
}

void TestDef_eng::tearDown()
{
    delete m_cal;
}

void TestDef_eng::testCreation()
{
    CPPUNIT_ASSERT( m_sid >= 0 );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "English Hybrid";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "eng";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "hy";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "m" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "ojn" ) {
            str = "Scheme names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_eng::testStrTableInput()
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

void TestDef_eng::testStrTableOutput()
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
        if( testJdnValues[i] == 1892732 ) {
            string t;
        }
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][1];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }
}

void TestDef_eng::testRangeShorthand()
{
    struct data { string in; string out1; string out2; } t[] = {
        { 
            "Mar 1748", 
            "25 Mar 1748 ~ 31 Mar 1748 | 1 Mar 1748 ~ 24 Mar 1748",
            "25 Mar 1748 ~ 31 Mar 1748 | 1 Mar 1748/9 ~ 24 Mar 1748/9"
        },
        { "23feb1750", "23 Feb 1750", "23 Feb 1750/1" },
        { "23 Feb 1751", "", "" },                      // No such date
        { "1751", "25 Mar 1751 ~ 31 Dec 1751", "25 Mar 1751 ~ 31 Dec 1751" },
        { "mar1751", "25 Mar 1751 ~ 31 Mar 1751", "25 Mar 1751 ~ 31 Mar 1751" },
        { "23feb1752", "23 Feb 1752", "23 Feb 1752" },
        { "6 sep 1948", "6 Sep 1948", "6 Sep 1948" },
        { "23feb1942", "23 Feb 1942", "23 Feb 1942" },
        { "25 March 1448 ~ 24 3 1448", "1448", "1448 ~ 1448/9" },
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

// End of src/test/def_eng.cpp file
