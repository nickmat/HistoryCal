/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testcal.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the Cal library interface
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th October 2013
 * Copyright:   Copyright (c) 2013-2014, Nick Matthews.
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

#include <cal/calendars.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/config/SourcePrefix.h>

using namespace std;
using namespace Cal;

class TestCal : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestCal );
    CPPUNIT_TEST( testVersion );
    CPPUNIT_TEST( testConstructor );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testVersion();
    void testConstructor();
};


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestCal );



void TestCal::setUp()
{
}


void TestCal::tearDown()
{
}

void TestCal::testVersion()
{
#if defined(NDEBUG) || !defined(_DEBUG)
#define VERSION_CONFIG   ""
#else
#define VERSION_CONFIG   " debug"
#endif
    const string version( "0.2.3" VERSION_CONFIG );
    string calver( Calendars::version() ); 
    CPPUNIT_ASSERT_EQUAL( version, calver );
}

void TestCal::testConstructor()
{
    Cal::Calendars cal;  // Default constructor with no Schemes.
    Cal::SchemeList slist = cal.get_scheme_list();
    CPPUNIT_ASSERT_EQUAL( 0U, slist.size() );
    CPPUNIT_ASSERT_EQUAL( (Cal::SHandle) NULL, cal.get_scheme( "jdn" ) );
    cal.run_script( "scheme \"jdn\" {name \"Julian Day Number\"; base jdn;}" );
    slist = cal.get_scheme_list();
    CPPUNIT_ASSERT_EQUAL( 1U, slist.size() );
    CPPUNIT_ASSERT_EQUAL( slist[0].handle, cal.get_scheme( "jdn" ) );
}

// End of src/test/testcal.cpp file
