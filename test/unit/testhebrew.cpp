/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testhebrew.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the Hebrew calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     9th December 2014
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

class TestHebrew : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestHebrew );
    CPPUNIT_TEST( testScript );
    CPPUNIT_TEST( testValues );
    CPPUNIT_TEST( testRanges );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testScript();
    void testValues();
    void testRanges();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestHebrew );


void TestHebrew::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"h\" {name \"Hebrew\"; base hebrew;}" );
    m_sid = m_cal->get_scheme( "h" );
}

void TestHebrew::tearDown()
{
    delete m_cal;
}


DMYDate testHValues[MaxSample] = {
//    day mon year
    { 10,  5, 3174 },
    { 25,  9, 3593 },
    {  3,  7, 3831 },
    {  9,  7, 3896 },
    { 18, 10, 4230 },
    {  4,  3, 4336 },
    { 13,  8, 4455 },
    {  6,  2, 4773 },
    { 23,  2, 4856 },
    {  7,  1, 4950 },
    {  8, 13, 5000 },
    { 21,  1, 5048 },
    {  7,  2, 5058 },
    {  1,  4, 5151 },
    {  7, 11, 5196 },
    {  3,  1, 5252 },
    {  1,  7, 5314 },
    { 27, 12, 5320 },
    { 20,  3, 5408 },
    {  3,  4, 5440 },
    {  5,  5, 5476 },
    {  4,  4, 5528 },
    { 11,  5, 5579 },
    { 12,  1, 5599 },
    { 22,  1, 5663 },
    { 19,  5, 5689 },
    {  8,  7, 5702 },
    { 14,  1, 5703 },
    {  8,  7, 5704 },
    { 12, 13, 5752 },
    {  5, 12, 5756 },
    { 12,  8, 5799 },
    {  5,  5, 5854 }
};


void TestHebrew::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("h");
    string name("Hebrew");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestHebrew::testValues()
{
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst; 
        tst << testHValues[i].year << " "
            << testHValues[i].month << " "
            << testHValues[i].day;
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( m_sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string jbstr = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( value, jbstr );
    }
}

void TestHebrew::testRanges()
{
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst; 
        tst << testHValues[i].year << " "
            << testHValues[i].month << " "
            << testHValues[i].day << " .. "
            << testHValues[i+1].year << " "
            << testHValues[i+1].month << " "
            << testHValues[i+1].day;
        string value = tst.str();
        Range rng = m_cal->str_to_range( m_sid, value/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( m_sid, rng );
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
}

// End of src/test/testhebrew.cpp file
