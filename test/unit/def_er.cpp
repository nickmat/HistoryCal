/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_er.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default English Regnal (er) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     23th November 2014
 * Copyright:   Copyright (c) 2014 - 2015, Nick Matthews.
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

class TestDef_er : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_er );
    CPPUNIT_TEST( testCreation );
//    CPPUNIT_TEST( testStrTableInput );
//    CPPUNIT_TEST( testStrTableOutput );
    CPPUNIT_TEST( testStringInput );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testCreation();
//    void testStrTableInput();
//    void testStrTableOutput();
    void testStringInput();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_er );

// TODO: Enable table check.
#if 0
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
#endif

void TestDef_er::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "er" );
}

void TestDef_er::tearDown()
{
    delete m_cal;
}

void TestDef_er::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "English Regnal";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "er";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "er";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "w" ) {
            str = "Weekday names";
        }
        if( info.vocab_codes[i] == "er" ) {
            str = "English Monarchs";
        }
        if( info.vocab_codes[i] == "m" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "on" ) {
            str = "Scheme names";
        }
        if( info.vocab_codes[i] == "part" ) {
            str = "Part of Reign Year";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

#if 0
void TestDef_er::testStrTableInput()
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

void TestDef_er::testStrTableOutput()
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
#endif

void TestDef_er::testStringInput()
{
    struct data { string in; string out; Field beg; Field end; } t[] = {
        { "10 6 ? 26 ? 37 1", "10 Jun 26 George II, ns", 2361491, 2361491 },
        { "11 6 ? 26 ? 37 1", "11 Jun 26 George II, ns", 2361492, 2361492 },
        { "com", "The Commonwealth", 2323386, 2327521 },
        { "1jan1000eng", "1 Jan 1000/1 English calendar, os", 2086674, 2086674 },
//        { "20may6john", "", 0, 0 }, // Currently, this is normalised to "20 May 7 John", should be invalid.
        { "8may3john", "8 May, First 3 John | 8 May, Second 3 John",
            2159851, 2160216 },
        { "12jun26geoii", "12 Jun 26 George II, os | 12 Jun 26 George II, ns",
            2361139, 2361493 },
        { "12jun26geoii,os", "12 Jun 26 George II, os",
            2361139, 2361139 },
        { "19sep12geovi", "19 Sep 12 George VI", 2432814, 2432814 },
    };
    size_t count = sizeof(t) / sizeof(data);

    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
        if( rl.size() ) {
            CPPUNIT_ASSERT_EQUAL( t[i].beg, rl[0].jdn1 );
            size_t j = rl.size() - 1;
            CPPUNIT_ASSERT_EQUAL( t[i].end, rl[j].jdn2 );
        }
    }
}

// End of src/test/def_er.cpp file
