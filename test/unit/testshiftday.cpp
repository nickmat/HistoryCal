/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testshiftday.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for ShiftDay variant calendars.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     18th March 2015
 * Copyright:   Copyright (c) 2015 .. 2020, Nick Matthews.
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

class TestShiftDay : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestShiftDay );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testValues );
    CPPUNIT_TEST( testRanges );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;

public:
    void setUp();
    void tearDown();

    void testScript();
    void testValues();
    void testRanges();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestShiftDay );


void TestShiftDay::setUp()
{
    m_cal = new Calendars;
    m_cal->run_script(
        "scheme \"jdn\" {name \"Julian Day Number\"; base jdn;}"
        "scheme \"mjd\" {name \"Modified Julian Day\"; epoch 2400001 jdn;}"
        "scheme \"rd\" {name \"Rata Die\"; epoch 1721425 jdn;}"
    );
}

void TestShiftDay::tearDown()
{
    delete m_cal;
}

static Field test_values[MaxSample][2] = {
    //  mjd       rd
    { -892769, -214193 },
    { -739963,  -61387 },
    { -653107,   25469 },
    { -629359,   49217 },
    { -507269,  171307 },
    { -468421,  210155 },
    { -425149,  253427 },
    { -308836,  369740 },
    { -278491,  400085 },
    { -244221,  434355 },
    { -225971,  452605 },
    { -208416,  470160 },
    { -204739,  473837 },
    { -170726,  507850 },
    { -154420,  524156 },
    { -133900,  544676 },
    { -111458,  567118 },
    { -109099,  569477 },
    {  -76860,  601716 },
    {  -65152,  613424 },
    {  -51980,  626596 },
    {  -33022,  645554 },
    {  -14352,  664224 },
    {   -7175,  671401 },
    {   16223,  694799 },
    {   25848,  704424 },
    {   30266,  708842 },
    {   30833,  709409 },
    {   31004,  709580 },
    {   48698,  727274 },
    {   50138,  728714 },
    {   65737,  744313 },
    {   86076,  764652 }
};

void TestShiftDay::testScript()
{
    Cal::SHandle sid = m_cal->get_scheme( "mjd" );
    CPPUNIT_ASSERT( sid != NULL );
    string code("mjd");
    string name("Modified Julian Day");
    Scheme_info info;
    m_cal->get_scheme_info( &info, sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );

    sid = m_cal->get_scheme( "rd" );
    CPPUNIT_ASSERT( sid != NULL );
    code = "rd";
    name = "Rata Die";
    m_cal->get_scheme_info( &info,sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestShiftDay::testValues()
{
    Cal::SHandle sid = m_cal->get_scheme( "mjd" );
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst;
        tst << test_values[i][0];
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( value, str );
    }
    sid = m_cal->get_scheme( "rd" );
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst;
        tst << test_values[i][1];
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( value, str );
    }
}

void TestShiftDay::testRanges()
{
    Cal::SHandle sid = m_cal->get_scheme( "mjd" );
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst;
        tst << test_values[i][0] << " .. "
            << test_values[i+1][0];
        string value = tst.str();
        Range rng = m_cal->str_to_range( sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( sid, rng );
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
    sid = m_cal->get_scheme( "rd" );
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst;
        tst << test_values[i][1] << " .. "
            << test_values[i+1][1];
        string value = tst.str();
        Range rng = m_cal->str_to_range( sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( sid, rng );
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
}

// End of test/unit/testshiftday.cpp file
