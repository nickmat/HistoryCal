/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_h.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Hebrew (h) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     4th January 2015
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

class TestDef_h : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_h );
    CPPUNIT_TEST( testCreation );
    CPPUNIT_TEST( testStrTable );
    CPPUNIT_TEST( testStringInput );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testCreation();
    void testStrTable();
    void testStringInput();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_h );

static const char* test_strs[MaxSample][2] = {
    //     dmy                       wdmy
    { "10 Av 3174",         "Yom Rishon 10 Av 3174"          },
    { "25 Kislev 3593",     "Yom Revi'i 25 Kislev 3593"      },
    { "3 Tishri 3831",      "Yom Revi'i 3 Tishri 3831"       },
    { "9 Tishri 3896",      "Yom Rishon 9 Tishri 3896"       },
    { "18 Tevet 4230",      "Yom Revi'i 18 Tevet 4230"       },
    { "4 Sivan 4336",       "Yom Sheni 4 Sivan 4336"         },
    { "13 Marheshvan 4455", "Yom Shabbat 13 Marheshvan 4455" },
    { "6 Iyyar 4773",       "Yom Rishon 6 Iyyar 4773"        },
    { "23 Iyyar 4856",      "Yom Rishon 23 Iyyar 4856"       },
    { "7 Nisan 4950",       "Yom Shishi 7 Nisan 4950"        },
    { "8 Adar II 5000",     "Yom Shabbat 8 Adar II 5000"     },
    { "21 Nisan 5048",      "Yom Shishi 21 Nisan 5048"       },
    { "7 Iyyar 5058",       "Yom Rishon 7 Iyyar 5058"        },
    { "1 Tammuz 5151",      "Yom Rishon 1 Tammuz 5151"       },
    { "7 Shevat 5196",      "Yom Revi'i 7 Shevat 5196"       },
    { "3 Nisan 5252",       "Yom Shabbat 3 Nisan 5252"       },
    { "1 Tishri 5314",      "Yom Shabbat 1 Tishri 5314"      },
    { "27 Adar 5320",       "Yom Shabbat 27 Adar 5320"       },
    { "20 Sivan 5408",      "Yom Revi'i 20 Sivan 5408"       },
    { "3 Tammuz 5440",      "Yom Rishon 3 Tammuz 5440"       },
    { "5 Av 5476",          "Yom Shishi 5 Av 5476"           },
    { "4 Tammuz 5528",      "Yom Rishon 4 Tammuz 5528"       },
    { "11 Av 5579",         "Yom Sheni 11 Av 5579"           },
    { "12 Nisan 5599",      "Yom Revi'i 12 Nisan 5599"       },
    { "22 Nisan 5663",      "Yom Rishon 22 Nisan 5663"       },
    { "19 Av 5689",         "Yom Rishon 19 Av 5689"          },
    { "8 Tishri 5702",      "Yom Sheni 8 Tishri 5702"        },
    { "14 Nisan 5703",      "Yom Sheni 14 Nisan 5703"        },
    { "8 Tishri 5704",      "Yom Hamishi 8 Tishri 5704"      },
    { "12 Adar II 5752",    "Yom Shelishi 12 Adar II 5752"   },
    { "5 Adar 5756",        "Yom Rishon 5 Adar 5756"         },
    { "12 Marheshvan 5799", "Yom Revi'i 12 Marheshvan 5799"  },
    { "5 Av 5854",          "Yom Rishon 5 Av 5854"           }
};

void TestDef_h::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "h" );
}

void TestDef_h::tearDown()
{
    delete m_cal;
}

void TestDef_h::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Hebrew";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "h";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "h";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "hm" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "hw" ) {
            str = "Weekday names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_h::testStrTable()
{
    m_cal->set_input_format( m_sid, "dmy" );
    m_cal->set_output_format( m_sid, "dmy" );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string sample( test_strs[i][0] );
        Field jdn = m_cal->str_to_jdn( m_sid, sample );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( m_sid, jdn );
        CPPUNIT_ASSERT_EQUAL( sample, str );
    }
    m_cal->set_input_format( m_sid, "wdmy" );
    m_cal->set_output_format( m_sid, "wdmy" );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string sample( test_strs[i][1] );
        Field jdn = m_cal->str_to_jdn( m_sid, sample );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( m_sid, jdn );
        CPPUNIT_ASSERT_EQUAL( sample, str );
    }
}

void TestDef_h::testStringInput()
{
    struct data { string in; string fmt; string out; Field beg; Field end; } t[] = {
        { "5708", "dmy", "5708", 2432444, 2432828 },
        { "ell5708", "dmy", "Elul 5708", 2432800, 2432828 },
        { "Rish15ell5708", "wdmy", "Yom Rishon 15 Elul 5708", 2432814, 2432814 },
        { "15ell5708", "dmy", "15 Elul 5708", 2432814, 2432814 },
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

// End of src/test/def_h.cpp file
