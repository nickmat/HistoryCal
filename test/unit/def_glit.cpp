/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_glit.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Gregorian Liturgical (glit) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th August 2015
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

class TestDef_glit : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_glit );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_glit );

static const char* test_strs[MaxSample][2] = {
    //             wdmyl                       wly
    { "Sun 24 Jul -586 Trinity IV",   "Sun, Trinity IV -586",   },
    { "Wed 5 Dec -168 Advent I",      "Wed, Advent I -168",     },
    { "Wed 24 Sep 70 Trinity XV",     "Wed, Trinity XV 70",     },
    { "Sun 2 Oct 135 Trinity XVI",    "Sun, Trinity XVI 135",   },
    { "Wed 8 Jan 470 Xmas II",        "Wed, Xmas II 470",       },
    { "Mon 20 May 576 Ascen I",       "Mon, Ascen I 576",       },
    { "Sat 10 Nov 694 Trinity XX",    "Sat, Trinity XX 694",    },
    { "Sun 25 Apr 1013 Easter II",    "Sun, Easter II 1013",    },
    { "Sun 24 May 1096 Rogation",     "Sun, Rogation 1096",     },
    { "Fri 23 Mar 1190 Passion",      "Fri, Passion 1190",      },
    { "Sat 10 Mar 1240 Quinquag",     "Sat, Quinquag 1240",     },
    { "Fri 2 Apr 1288 Easter",        "Fri, Easter 1288",       },
    { "Sun 27 Apr 1298 Easter III",   "Sun, Easter III 1298",   },
    { "Sun 12 Jun 1391 Trinity II",   "Sun, Trinity II 1391",   },
    { "Wed 3 Feb 1436 Epiph IV",      "Wed, Epiph IV 1436",     },
    { "Sat 9 Apr 1492 Quasimodo",     "Sat, Quasimodo 1492",    },
    { "Sat 19 Sep 1553 Trinity XIV",  "Sat, Trinity XIV 1553",  },
    { "Sat 5 Mar 1560 Lent III",      "Sat, Lent III 1560",     },
    { "Wed 10 Jun 1648 Trinity",      "Wed, Trinity 1648",      }, // Checked [1]
    { "Sun 30 Jun 1680 Trinity II",   "Sun, Trinity II 1680",   }, // Checked [1]
    { "Fri 24 Jul 1716 Trinity VI",   "Fri, Trinity VI 1716",   }, // Checked [1]
    { "Sun 19 Jun 1768 Trinity III",  "Sun, Trinity III 1768",  }, // Checked [1]
    { "Mon 2 Aug 1819 Trinity VIII",  "Mon, Trinity VIII 1819", }, // Checked [1]
    { "Wed 27 Mar 1839 Holy",         "Wed, Holy 1839",         }, // Checked [1]
    { "Sun 19 Apr 1903 Quasimodo",    "Sun, Quasimodo 1903",    }, // Checked [1]
    { "Sun 25 Aug 1929 Trinity XIII", "Sun, Trinity XIII 1929", }, // Checked [1]
    { "Mon 29 Sep 1941 Trinity XVI",  "Mon, Trinity XVI 1941",  }, // Checked [1]
    { "Mon 19 Apr 1943 Holy",         "Mon, Holy 1943",         }, // Checked [1]
    { "Thur 7 Oct 1943 Trinity XV",   "Thur, Trinity XV 1943",  }, // Checked [1]
    { "Tue 17 Mar 1992 Lent II",      "Tue, Lent II 1992",      }, // Checked [1]
    { "Sun 25 Feb 1996 Quadrag",      "Sun, Quadrag 1996",      }, // Checked [1]
    { "Wed 10 Nov 2038 Trinity XX",   "Wed, Trinity XX 2038",   }, // Checked [1]
    { "Sun 18 Jul 2094 Trinity VII",  "Sun, Trinity VII 2094",  }  // Checked [1]
};
// Checked [1] - A Handbook of Dates, C R Cheney revised Micheal Jones 

void TestDef_glit::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "glit" );
}

void TestDef_glit::tearDown()
{
    delete m_cal;
}

void TestDef_glit::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Gregorian Liturgical";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "glit";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "lit";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "m" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "w" ) {
            str = "Weekday names";
        }
        if( info.vocab_codes[i] == "lit" ) {
            str = "Liturgy Weeks";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_glit::testStrTable()
{
    m_cal->set_input_format( m_sid, "wdmyl" );
    m_cal->set_output_format( m_sid, "wdmyl" );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string sample( test_strs[i][0] );
        Field jdn = m_cal->str_to_jdn( m_sid, sample );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( m_sid, jdn );
        CPPUNIT_ASSERT_EQUAL( sample, str );
    }
    m_cal->set_input_format( m_sid, "wly" );
    m_cal->set_output_format( m_sid, "wly" );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string sample( test_strs[i][1] );
        Field jdn = m_cal->str_to_jdn( m_sid, sample );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( m_sid, jdn );
        CPPUNIT_ASSERT_EQUAL( sample, str );
    }
}

void TestDef_glit::testStringInput()
{
    struct data { string fmt; string in; string out; Field beg; Field end; } t[] = {
        { "wdmyl", "sun19sep1948trinityxvii", "Sun 19 Sep 1948 Trinity XVII", 2432814, 2432814 },
        { "wly", "sun,trinityxvii1948", "Sun, Trinity XVII 1948", 2432814, 2432814 },
        { "wly", "trinityxvii1948", "Sun, Trinity XVII 1948 ~ Sat, Trinity XVII 1948", 2432814, 2432820 },
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

// End of src/test/def_i.cpp file
