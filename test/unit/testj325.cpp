/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testj325.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the Julian Lady Day new year calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     9th October 2013
 * Copyright:   Copyright (c) 2013 .. 2020, Nick Matthews.
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

class TestJ325 : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestJ325 );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testValues );
    CPPUNIT_TEST( testRanges );
    CPPUNIT_TEST( testRangeShorthand );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testScript();
    void testValues();
    void testRanges();
    void testRangeShorthand();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestJ325 );


void TestJ325::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script(
        "scheme \"jb\" {name \"Julian Base\"; base julian;}"
        "scheme \"j325\" {name \"Julian Lady Day\"; epoch 1721507 julian;}"
    );
    m_sid = m_cal->get_scheme( "j325" );
}

void TestJ325::tearDown()
{
    delete m_cal;
}

DMYDateStr testJ325Values[MaxSample] = {
//    day mon year
    { 30,  7, -586, "-586 7 30" },
    {  8, 12, -168, "-168 12 8" },
    { 26,  9,   70, "70 9 26" },
    {  3, 10,  135, "135 10 3" },
    {  7,  1,  469, "469 1 7" },
    { 18,  5,  576, "576 5 18" },
    {  7, 11,  694, "694 11 7" },
    { 19,  4, 1013, "1013 4 19" },
    { 18,  5, 1096, "1096 5 18" },
    { 16,  3, 1189, "1189 3 16" },
    {  3,  3, 1239, "1239 3 3" },
    { 26,  3, 1288, "1288 3 26" },
    { 20,  4, 1298, "1298 4 20" },
    {  4,  6, 1391, "1391 6 4" },
    { 25,  1, 1435, "1435 1 25" },
    { 31,  3, 1492, "1492 3 31" },
    {  9,  9, 1553, "1553 9 9" },
    { 24,  2, 1559, "1559 2 24" },
    { 31,  5, 1648, "1648 5 31" },
    { 20,  6, 1680, "1680 6 20" },
    { 13,  7, 1716, "1716 7 13" },
    {  8,  6, 1768, "1768 6 8" },
    { 21,  7, 1819, "1819 7 21" },
    { 15,  3, 1838, "1838 3 15" },
    {  6,  4, 1903, "1903 4 6" },
    { 12,  8, 1929, "1929 8 12" },
    { 16,  9, 1941, "1941 9 16" },
    {  6,  4, 1943, "1943 4 6" },
    { 24,  9, 1943, "1943 9 24" },
    {  4,  3, 1991, "1991 3 4" },
    { 12,  2, 1995, "1995 2 12" },
    { 28, 10, 2038, "2038 10 28" },
    {  5,  7, 2094, "2094 7 5" }
};


void TestJ325::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("j325");
    string name("Julian Lady Day");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestJ325::testValues()
{
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst;
        tst << testJ325Values[i].year << " "
            << testJ325Values[i].month << " "
            << testJ325Values[i].day;
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( m_sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string jbstr = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( testJ325Values[i].str, jbstr );
    }
}

void TestJ325::testRanges()
{
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst;
        tst << testJ325Values[i].year << " "
            << testJ325Values[i].month << " "
            << testJ325Values[i].day << " .. "
            << testJ325Values[i+1].year << " "
            << testJ325Values[i+1].month << " "
            << testJ325Values[i+1].day;
        string value = tst.str();
        Range rng = m_cal->str_to_range( m_sid, value/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( m_sid, rng );
        value = testJ325Values[i].str + " .. " + testJ325Values[i+1].str;
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
}

void TestJ325::testRangeShorthand()
{
    struct data { string in; string out; } t[] = {
        { "1948 9 6", "1948 9 6" },
        { "1942 2 23", "1942 2 23" },
        { "1948 3 25 .. 1948 3 24", "1948" },
        { "1948 ? ?", "1948" },
        { "1948 2 1 .. 1948 2 28", "1948 2" },
        { "1948 2 ?", "1948 2" },
        { "1948 4 1 .. 1948 4 30", "1948 4" },
        { "1948 4 ?", "1948 4" },
        { "1948 3 ?", "1948 3 25 .. 1948 3 31 | 1948 3 1 .. 1948 3 24" },
        { "1948 3 25 .. 1948 4 3", "1948 3 25 .. 1948 4 3" },
        { "1948 ? 19", invalid },
    };
    size_t count = sizeof(t) / sizeof(data);

    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
    }
}

// End of src/test/testj325.cpp file
