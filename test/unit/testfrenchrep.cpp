/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/testfrenchrep.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit tests for the French Republican calendar.
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

class TestFrenchRep : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestFrenchRep );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestFrenchRep );


void TestFrenchRep::setUp()
{
    m_sid = NULL;
    m_cal = new Calendars;
    m_cal->run_script( "scheme \"fr\" {name \"French Republican\"; base french;}" );
    m_sid = m_cal->get_scheme( "fr" );
}

void TestFrenchRep::tearDown()
{
    delete m_cal;
}


DMYDate testFRValues[MaxSample] = {
//    day mon  year
    {  5, 11, -2378 },
    { 14,  3, -1959 },
    {  2,  1, -1721 },
    { 10,  1, -1656 },
    { 19,  4, -1322 },
    {  1,  9, -1216 },
    { 19,  2, -1097 },
    {  5,  8,  -779 },
    {  5,  9,  -696 },
    {  2,  7,  -602 },
    { 20,  6,  -552 },
    { 13,  7,  -504 },
    {  8,  8,  -494 },
    { 23,  9,  -401 },
    { 14,  5,  -356 },
    { 20,  7,  -300 },
    {  2, 13,  -239 },
    { 15,  6,  -232 },
    { 22,  9,  -144 },
    { 12, 10,  -112 },
    {  6, 11,   -76 },
    {  1, 10,   -24 },
    { 14, 11,    27 },
    {  6,  7,    47 },
    { 28,  7,   111 },
    {  7, 12,   137 },
    {  7,  1,   150 },
    { 29,  7,   151 },
    { 15,  1,   152 },
    { 27,  6,   200 },
    {  6,  6,   204 },
    { 20,  2,   247 },
    { 30, 10,   302 }
};


void TestFrenchRep::testScript()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    string code("fr");
    string name("French Republican");
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    CPPUNIT_ASSERT_EQUAL( code, info.code );
    CPPUNIT_ASSERT_EQUAL( name, info.name );
}

void TestFrenchRep::testValues()
{
    for( int i = 0 ; i < MaxSample ; i++ ) {
        stringstream tst; 
        tst << testFRValues[i].year << " "
            << testFRValues[i].month << " "
            << testFRValues[i].day;
        string value = tst.str();
        Field jdn = m_cal->str_to_jdn( m_sid, value );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string jbstr = m_cal->jdn_to_str( m_sid, testJdnValues[i] );
        CPPUNIT_ASSERT_EQUAL( value, jbstr );
    }
}

void TestFrenchRep::testRanges()
{
    for( int i = 0 ; i < MaxSample-1 ; i++ ) {
        stringstream tst; 
        tst << testFRValues[i].year << " "
            << testFRValues[i].month << " "
            << testFRValues[i].day << " .. "
            << testFRValues[i+1].year << " "
            << testFRValues[i+1].month << " "
            << testFRValues[i+1].day;
        string value = tst.str();
        Range rng = m_cal->str_to_range( m_sid, value/*, 0*/ );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], rng.jdn1 );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i+1], rng.jdn2 );
        string rngstr = m_cal->range_to_str( m_sid, rng );
        CPPUNIT_ASSERT_EQUAL( value, rngstr );
    }
}

// End of src/test/testfrenchrep.cpp file
