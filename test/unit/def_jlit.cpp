/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_jlit.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default Julian Liturgical (jlit) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th August 2015
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

class TestDef_jlit : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_jlit );
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
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_jlit );

static const char* test_strs[MaxSample][2] = {
    //             wdmyl                       wly
    { "Sun 30 Jul -586 Trinity IV",   "Sun, Trinity IV -586",   },
    { "Wed 8 Dec -168 Advent II",     "Wed, Advent II -168",    },
    { "Wed 26 Sep 70 Trinity XV",     "Wed, Trinity XV 70",     },
    { "Sun 3 Oct 135 Trinity XVI",    "Sun, Trinity XVI 135",   },
    { "Wed 7 Jan 470 Xmas II",        "Wed, Xmas II 470",       }, // Checked [1]
    { "Mon 18 May 576 Ascen I",       "Mon, Ascen I 576",       }, // Checked [1]
    { "Sat 7 Nov 694 Trinity XX",     "Sat, Trinity XX 694",    }, // Checked [1]
    { "Sun 19 Apr 1013 Easter II",    "Sun, Easter II 1013",    }, // Checked [1]
    { "Sun 18 May 1096 Rogation",     "Sun, Rogation 1096",     }, // Checked [1]
    { "Fri 16 Mar 1190 Passion",      "Fri, Passion 1190",      }, // Checked [1]
    { "Sat 3 Mar 1240 Quinquag",      "Sat, Quinquag 1240",     }, // Checked [1]
    { "Fri 26 Mar 1288 Holy",         "Fri, Holy 1288",         }, // Checked [1]
    { "Sun 20 Apr 1298 Easter II",    "Sun, Easter II 1298",    }, // Checked [1]
    { "Sun 4 Jun 1391 Trinity II",    "Sun, Trinity II 1391",   }, // Checked [1]
    { "Wed 25 Jan 1436 Epiph III",    "Wed, Epiph III 1436",    }, // Checked [1]
    { "Sat 31 Mar 1492 Lent III",     "Sat, Lent III 1492",     }, // Checked [1]
    { "Sat 9 Sep 1553 Trinity XIV",   "Sat, Trinity XIV 1553",  }, // Checked [1]
    { "Sat 24 Feb 1560 Sexages",      "Sat, Sexages 1560",      }, // Checked [1]
    { "Wed 31 May 1648 Trinity",      "Wed, Trinity 1648",      }, // Checked [1]
    { "Sun 20 Jun 1680 Trinity II",   "Sun, Trinity II 1680",   }, // Checked [1]
    { "Fri 13 Jul 1716 Trinity VI",   "Fri, Trinity VI 1716",   }, // Checked [1]
    { "Sun 8 Jun 1768 Trinity II",    "Sun, Trinity II 1768",   },
    { "Mon 21 Jul 1819 Trinity VII",  "Mon, Trinity VII 1819",  },
    { "Wed 15 Mar 1839 Passion",      "Wed, Passion 1839",      },
    { "Sun 6 Apr 1903 Easter",        "Sun, Easter 1903",       },
    { "Sun 12 Aug 1929 Trinity VIII", "Sun, Trinity VIII 1929", },
    { "Mon 16 Sep 1941 Trinity XV",   "Mon, Trinity XV 1941",   },
    { "Mon 6 Apr 1943 Holy",          "Mon, Holy 1943",         },
    { "Thur 24 Sep 1943 Trinity XV",  "Thur, Trinity XV 1943",  },
    { "Tue 4 Mar 1992 Quadrag",       "Tue, Quadrag 1992",      },
    { "Sun 12 Feb 1996 Quinquag",     "Sun, Quinquag 1996",     },
    { "Wed 28 Oct 2038 Trinity XX",   "Wed, Trinity XX 2038",   },
    { "Sun 5 Jul 2094 Trinity VI",    "Sun, Trinity VI 2094",   }
};
// Checked [1] - A Handbook of Dates, C R Cheney revised Micheal Jones 

void TestDef_jlit::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "jlit" );
}

void TestDef_jlit::tearDown()
{
    delete m_cal;
}

void TestDef_jlit::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "Julian Liturgical";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "jlit";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "lit";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str = "";
        if( info.vocab_codes[i] == "m" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "ws" ) {
            str = "Weekday names";
        }
        if( info.vocab_codes[i] == "lit" ) {
            str = "Liturgy Weeks";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
}

void TestDef_jlit::testStrTable()
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

void TestDef_jlit::testStringInput()
{
    struct data { string fmt; string in; string out; Field beg; Field end; } t[] = {
        { "wly", "wed,xmasii470", "Wed, Xmas II 470", 1892732, 1892732 },
        { "wly", "mon,trinityxv1748", "Mon, Trinity XV 1748", 2359777, 2359777 },
        { "wdmyl", "mon19sep1748trinityxv", "Mon 19 Sep 1748 Trinity XV", 2359777, 2359777 },
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

// End of src/test/def_jlit.cpp file
