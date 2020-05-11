/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_g.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the default Gregorian (g) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th February 2014
 * Copyright:   Copyright (c) 2014 .. 2020, Nick Matthews.
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

class TestDef_g : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_g );
    CPPUNIT_TEST( testCreation );
    CPPUNIT_TEST( testStrTableInput );
    CPPUNIT_TEST( testStrTableOutput );
    CPPUNIT_TEST( testRangeShorthand );
    CPPUNIT_TEST( testInputFormats );
    CPPUNIT_TEST( testMasking );
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
    void testInputFormats();
    void testMasking();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_g );

static const char* test_strs[MaxSample][2] = {
    { "24 Jul -586", "Jul 24, -586" },
    { "5 Dec -168", "Dec 5, -168" },
    { "24 Sep 70", "Sep 24, 70" },
    { "2 Oct 135", "Oct 2, 135" },
    { "8 Jan 470", "Jan 8, 470" },
    { "20 May 576", "May 20, 576" },
    { "10 Nov 694", "Nov 10, 694" },
    { "25 Apr 1013", "Apr 25, 1013" },
    { "24 May 1096", "May 24, 1096" },
    { "23 Mar 1190", "Mar 23, 1190" },
    { "10 Mar 1240", "Mar 10, 1240" },
    { "2 Apr 1288", "Apr 2, 1288" },
    { "27 Apr 1298", "Apr 27, 1298" },
    { "12 Jun 1391", "Jun 12, 1391" },
    { "3 Feb 1436", "Feb 3, 1436" },
    { "9 Apr 1492", "Apr 9, 1492" },
    { "19 Sep 1553", "Sep 19, 1553" },
    { "5 Mar 1560", "Mar 5, 1560" },
    { "10 Jun 1648", "Jun 10, 1648" },
    { "30 Jun 1680", "Jun 30, 1680" },
    { "24 Jul 1716", "Jul 24, 1716" },
    { "19 Jun 1768", "Jun 19, 1768" },
    { "2 Aug 1819", "Aug 2, 1819" },
    { "27 Mar 1839", "Mar 27, 1839" },
    { "19 Apr 1903", "Apr 19, 1903" },
    { "25 Aug 1929", "Aug 25, 1929" },
    { "29 Sep 1941", "Sep 29, 1941" },
    { "19 Apr 1943", "Apr 19, 1943" },
    { "7 Oct 1943", "Oct 7, 1943" },
    { "17 Mar 1992", "Mar 17, 1992" },
    { "25 Feb 1996", "Feb 25, 1996" },
    { "10 Nov 2038", "Nov 10, 2038" },
    { "18 Jul 2094", "Jul 18, 2094" }
};

void TestDef_g::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "g" );
}

void TestDef_g::tearDown()
{
    delete m_cal;
}

void TestDef_g::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Gregorian";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "g";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "g";
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

void TestDef_g::testStrTableInput()
{
    bool set = setInputFormatFromDesc( m_cal, m_sid, "Day Month Year" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][0] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }

    set = setInputFormatFromDesc( m_cal, m_sid, "Month Day Year" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        Field jdn = m_cal->str_to_jdn( m_sid, test_strs[i][1] );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
    }
}

void TestDef_g::testStrTableOutput()
{
    bool set = setOutputFormatFromDesc( m_cal, m_sid, "dd Mon yyyy" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][0];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }

    set = setOutputFormatFromDesc( m_cal, m_sid, "Mon dd, yyyy" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string str = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        string tbl = test_strs[i][1];
        CPPUNIT_ASSERT_EQUAL( tbl, str );
    }
}

void TestDef_g::testRangeShorthand()
{
    struct data { string in; string out; } t[] = {
        { "19sep1948", "19 Sep 1948" },
        { "sep1948", "Sep 1948" },
        { "1948", "1948" },
        { "1feb1948 .. 29feb1948", "Feb 1948" },
        { "1jan1948 .. 31dec1948", "1948" },
        { "1948 .. sep1948", "Jan 1948 .. Sep 1948" },
        { "1948..19sep1948", "1 Jan 1948 .. 19 Sep 1948" },
        { "19sep1948..1948", "19 Sep 1948 .. 31 Dec 1948" },
        { "19 ? 1948", invalid }
    };
    size_t count = sizeof(t) / sizeof(data);

    bool set = setInputFormatFromDesc( m_cal, m_sid, "Day Month Year" );
    CPPUNIT_ASSERT( set == true );
    set = setOutputFormatFromDesc( m_cal, m_sid, "dd Mon yyyy" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestDef_g::testInputFormats()
{
    struct data { string in; string out; } t[] = {
        { "sun19sep1948", "Sun 19 Sep 1948" },
    };
    size_t count = sizeof(t) / sizeof(data);

    m_cal->set_input_format( m_sid, "wdmy" );
    m_cal->set_output_format( m_sid, "wdmy" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
    m_cal->set_input_format( m_sid, "dmy" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

void TestDef_g::testMasking()
{
#if 0 // not yet ready
    struct data { string in; string out; } t[] = {
        { "Sun ? Sep 1948", "5 Sep 1948 | 12 Sep 1948 | 19 Sep 1948 | 26 Sep 1948" }
    };
    size_t count = sizeof(t) / sizeof(data);

    bool set = setInputOrder( "WDay Day Month Year" );
    CPPUNIT_ASSERT( set == true );
    set = setOutputFormat( "dd Mon yyyy" );
    CPPUNIT_ASSERT( set == true );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
#endif
}

// End of src/test/def_g.cpp file
