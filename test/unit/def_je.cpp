/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_je.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Julian Easter Sunday calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     10th March 2018
 * Copyright:   Copyright (c) 2018, Nick Matthews.
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

class TestDef_je : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_je );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_je );

static const char* test_strs[MaxSample][2] = {
    { "30 Jul -586", "Sun 30 Jul -586" },
    { "8 Dec -168",  "Wed 8 Dec -168"  },
    { "26 Sep 70",   "Wed 26 Sep 70"   },
    { "3 Oct 135",   "Sun 3 Oct 135"   },
    { "7 Jan 469",   "Wed 7 Jan 469"   },
    { "18 May 576",  "Mon 18 May 576"  },
    { "7 Nov 694",   "Sat 7 Nov 694"   },
    { "19 Apr 1013", "Sun 19 Apr 1013" },
    { "18 May 1096", "Sun 18 May 1096" },
    { "16 Mar 1189", "Fri 16 Mar 1189" },
    { "3 Mar 1239",  "Sat 3 Mar 1239"  },
    { "26 Mar 1287", "Fri 26 Mar 1287" },
    { "20 Apr 1298", "Sun 20 Apr 1298" },
    { "4 Jun 1391",  "Sun 4 Jun 1391"  },
    { "25 Jan 1435", "Wed 25 Jan 1435" },
    { "31 Mar 1491", "Sat 31 Mar 1491" },
    { "9 Sep 1553",  "Sat 9 Sep 1553"  },
    { "24 Feb 1559", "Sat 24 Feb 1559" },
    { "31 May 1648", "Wed 31 May 1648" },
    { "20 Jun 1680", "Sun 20 Jun 1680" },
    { "13 Jul 1716", "Fri 13 Jul 1716" },
    { "8 Jun 1768",  "Sun 8 Jun 1768"  },
    { "21 Jul 1819", "Mon 21 Jul 1819" },
    { "15 Mar 1838", "Wed 15 Mar 1838" },
    { "6 Apr 1903",  "Sun 6 Apr 1903"  },
    { "12 Aug 1929", "Sun 12 Aug 1929" },
    { "16 Sep 1941", "Mon 16 Sep 1941" },
    { "6 Apr 1942 Repeated", "Mon 6 Apr 1942" },
    { "24 Sep 1943", "Thur 24 Sep 1943" },
    { "4 Mar 1991",  "Tue 4 Mar 1991"  },
    { "12 Feb 1995", "Sun 12 Feb 1995" },
    { "28 Oct 2038", "Wed 28 Oct 2038" },
    { "5 Jul 2094",  "Sun 5 Jul 2094"  }
};


void TestDef_je::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "je" );
}

void TestDef_je::tearDown()
{
    delete m_cal;
}

void TestDef_je::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Julian Easter";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "je";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "je";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "m" ) {
            str = "Month names";
        }
        if ( info.vocab_codes[i] == "w" ) {
            str = "Weekday names";
        }
        if ( info.vocab_codes[i] == "erpt" ) {
            str = "Easter Repeated Date";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_je::testStrTableInput()
{
    bool set = setInputFormatFromDesc( m_cal, m_sid, "Day Month Year [First]" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][0] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }

    set = setInputFormatFromDesc( m_cal, m_sid, "WDay Day Month Year" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][1] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }
}

void TestDef_je::testStrTableOutput()
{
    bool set = setOutputFormatFromDesc( m_cal, m_sid, "dd Mon yyyy [Repeated]" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][0];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }

    set = setOutputFormatFromDesc( m_cal, m_sid, "WDay dd Mon yyyy" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][1];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }
}

void TestDef_je::testRangeShorthand()
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

    bool set = setInputFormatFromDesc( m_cal, m_sid, "Day Month Year" );
    CPPUNIT_ASSERT( set == true );
    set = setOutputFormatFromDesc( m_cal, m_sid, "dd Mon yyyy" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out1, str );
    }
    set = setOutputFormatFromDesc( m_cal, m_sid, "dd Mon yyyy/y" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out2, str );
    }
}

// End of src/test/def_je.cpp file
