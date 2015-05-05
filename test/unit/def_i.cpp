/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_i.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Islamic Arithmetical (i) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th January 2015
 * Copyright:   Copyright (c) 2015, Nick Matthews.
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

using namespace Cal;
using std::string;

class TestDef_i : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_i );
    CPPUNIT_TEST( testCreation );
    CPPUNIT_TEST( testStrTable );
    CPPUNIT_TEST( testStringInput );
    CPPUNIT_TEST( test_str_to_jdn );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testCreation();
    void testStrTable();
    void testStringInput();
    void test_str_to_jdn();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_i );

static const char* test_strs[MaxSample][2] = {
    //     dmy                       wdmy
    { "9 Dhu al-Hijjah -1245", "Ahad 9 Dhu al-Hijjah -1245", },
    { "23 Safar -813",         "Arbi 23 Safar -813",         },
    { "1 Rabi' II -568",       "Arbi 1 Rabi' II -568",       },
    { "6 Rabi' II -501",       "Ahad 6 Rabi' II -501",       },
    { "17 Shawwal -157",       "Arbi 17 Shawwal -157",       },
    { "3 Jumada II -47",       "Ithn 3 Jumada II -47",       },
    { "13 Rajab 75",           "Sabt 13 Rajab 75",           },
    { "5 Shawwal 403",         "Ahad 5 Shawwal 403",         },
    { "22 Jumada I 489",       "Ahad 22 Jumada I 489",       },
    { "7 Safar 586",           "Jumu 7 Safar 586",           },
    { "7 Sha'aban 637",        "Sabt 7 Sha'aban 637",        },
    { "20 Safar 687",          "Jumu 20 Safar 687",          },
    { "7 Rajab 697",           "Ahad 7 Rajab 697",           },
    { "1 Rajab 793",           "Ahad 1 Rajab 793",           },
    { "6 Rajab 839",           "Arbi 6 Rajab 839",           },
    { "1 Jumada II 897",       "Sabt 1 Jumada II 897",       },
    { "30 Ramadan 960",        "Sabt 30 Ramadan 960",        },
    { "27 Jumada I 967",       "Sabt 27 Jumada I 967",       },
    { "18 Jumada I 1058",      "Arbi 18 Jumada I 1058",      },
    { "2 Jumada II 1091",      "Ahad 2 Jumada II 1091",      },
    { "4 Sha'aban 1128",       "Jumu 4 Sha'aban 1128",       },
    { "3 Safar 1182",          "Ahad 3 Safar 1182",          },
    { "10 Shawwal 1234",       "Ithn 10 Shawwal 1234",       },
    { "11 Muharram 1255",      "Arbi 11 Muharram 1255",      },
    { "21 Muharram 1321",      "Ahad 21 Muharram 1321",      },
    { "19 Rabi' I 1348",       "Ahad 19 Rabi' I 1348",       },
    { "8 Ramadan 1360",        "Ithn 8 Ramadan 1360",        },
    { "13 Rabi' II 1362",      "Ithn 13 Rabi' II 1362",      },
    { "7 Shawwal 1362",        "Kham 7 Shawwal 1362",        },
    { "13 Ramadan 1412",       "Thul 13 Ramadan 1412",       },
    { "5 Shawwal 1416",        "Ahad 5 Shawwal 1416",        },
    { "12 Shawwal 1460",       "Arbi 12 Shawwal 1460",       },
    { "5 Rabi' I 1518",        "Ahad 5 Rabi' I 1518",        }
};

void TestDef_i::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "i" );
}

void TestDef_i::tearDown()
{
    delete m_cal;
}

void TestDef_i::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Islamic Tabular";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "i";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "i";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "im" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "iw" ) {
            str = "Weekday names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_i::testStrTable()
{
    m_cal->set_input_format( m_sid, "dmy" );
    m_cal->set_output_format( m_sid, "dmy.a" );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string sample( test_strs[i][0] );
        Field jdn = m_cal->str_to_jdn( m_sid, sample );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( m_sid, jdn );
        CPPUNIT_ASSERT_EQUAL( sample, str );
    }
    m_cal->set_input_format( m_sid, "wdmy" );
    m_cal->set_output_format( m_sid, "wdmy.a" );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string sample( test_strs[i][1] );
        Field jdn = m_cal->str_to_jdn( m_sid, sample );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( m_sid, jdn );
        CPPUNIT_ASSERT_EQUAL( sample, str );
    }
}

void TestDef_i::testStringInput()
{
    struct data { string in; string fmt; string out; Field beg; Field end; } t[] = {
        { "1367", "dmy", "1367", 2432505, 2432858 },
        { "dhual-qi'dah1367", "dmy.a", "Dhu al-Qi'dah 1367", 2432800, 2432829 },
        { "ahad15dhual-qi'dah1367", "wdmy.a", "Ahad 15 Dhu al-Qi'dah 1367", 2432814, 2432814 },
        { "15dhual-qi'dah1367", "dmy.a", "15 Dhu al-Qi'dah 1367", 2432814, 2432814 },
        { "15dhual-qi'dah-10", "dmy.a", "15 Dhu al-Qi'dah -10", 1944851, 1944851 },
    };
    size_t count = sizeof(t) / sizeof(data);

    for( size_t i = 0 ; i < count ; i++ ) {
        m_cal->set_input_format( m_sid, t[i].fmt );
        m_cal->set_output_format( m_sid, t[i].fmt );
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

void TestDef_i::test_str_to_jdn()
{
    Field jdn = m_cal->str_to_jdn( m_sid, "" );
    CPPUNIT_ASSERT_EQUAL( f_invalid, jdn );
}

// End of src/test/def_i.cpp file
