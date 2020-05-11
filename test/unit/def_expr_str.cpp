/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_expr_str.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the expr_str_to_rangelist Calendar member.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th April 2016
 * Copyright:   Copyright (c) 2016 .. 2020, Nick Matthews.
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

class DefExprStr : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( DefExprStr );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle
    string num( int n );

public:
    void setUp();
    void tearDown();

    void testScript();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DefExprStr );


void DefExprStr::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "g" );
}

void DefExprStr::tearDown()
{
    delete m_cal;
}

string DefExprStr::num( int n )
{
    ostringstream ss; 
    ss << " [" << n << "]"; 
    return ss.str();
}


void DefExprStr::testScript()
{
    struct data { string in; string out; } t[] = {
        { "1940..1950 | 1945..1955", "1940 .. 1955" },   // 0
        { "1940..1950 &. 1945..1955", "1945 .. 1950" },
        { "1940..1950 \\ 1945..1955", "1940 .. 1944" },
        { "1940..1950 ^ 1945..1955", "1940 .. 1944 | 1951 .. 1955" },
        { "!( 1940..1950 )", "past .. 1939 | 1951 .. future" },
        { "1940..1950 &. ( 1945..1955 | 1948..1957 )", "1945 .. 1950" }, // 5
        { "( 1940..1950 &. 1945..1955 ) | 1948..1957", "1945 .. 1957" },
        { "ja:dmy#23feb1942/3", "8 Mar 1943" },
        { "19sep1948+#7", "26 Sep 1948" },
    };
    size_t count = sizeof(t) / sizeof(data);

    CPPUNIT_ASSERT( m_sid != NULL );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->expr_str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl ) + num(i);
        CPPUNIT_ASSERT_EQUAL( t[i].out+num(i), str );
    }
}

// End of test/unit/def_sxpr_str.cpp file
