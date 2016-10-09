/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/unit/def_fr.cpp
 * Project:     Cal Unit test: Unit Tests for Cal library API.
 * Purpose:     CppUnit for default French Republican (fr) calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     24th December 2014
 * Copyright:   Copyright (c) 2014 ~ 2016, Nick Matthews.
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

class TestDef_fr : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TestDef_fr );
    CPPUNIT_TEST( testCreation );
    CPPUNIT_TEST( testStrTable_dmy );
    CPPUNIT_TEST( testStrTable_cdmy );
    CPPUNIT_TEST( testStrTable_dcdmy );
    CPPUNIT_TEST( testStringInput );
    CPPUNIT_TEST_SUITE_END();

    Cal::Calendars* m_cal;
    Cal::SHandle    m_sid; // Scheme handle

public:
    void setUp();
    void tearDown();

    void testCreation();
    void testStrTable_dmy();
    void testStrTable_cdmy();
    void testStrTable_dcdmy();
    void testStringInput();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestDef_fr );

static const char* test_strs[MaxSample][3] = {
    //     dmy              cdmy                dcdmy
    { "5 Ther -2378",  "5 Ther -2378",  "Quin 5 Ther -2378"  },
    { "14 Frim -1959", "14 Frim -1959", "Quar 14 Frim -1959" },
    { "2 Vend -1721",  "2 Vend -1721",  "Duo 2 Vend -1721"   },
    { "10 Vend -1656", "10 Vend -1656", "Dec 10 Vend -1656"  },
    { "19 Nivo -1322", "19 Nivo -1322", "Non 19 Nivo -1322"  },
    { "1 Prai -1216",  "1 Prai -1216",  "Prim 1 Prai -1216"  },
    { "19 Brum -1097", "19 Brum -1097", "Non 19 Brum -1097"  },
    { "5 Flor -779",   "5 Flor -779",   "Quin 5 Flor -779"   },
    { "5 Prai -696",   "5 Prai -696",   "Quin 5 Prai -696"   },
    { "2 Germ -602",   "2 Germ -602",   "Duo 2 Germ -602"    },
    { "20 Vent -552",  "20 Vent -552",  "Dec 20 Vent -552"   },
    { "13 Germ -504",  "13 Germ -504",  "Tri 13 Germ -504"   },
    { "8 Flor -494",   "8 Flor -494",   "Oct 8 Flor -494"    },
    { "23 Prai -401",  "23 Prai -401",  "Tri 23 Prai -401"   },
    { "14 Pluv -356",  "14 Pluv -356",  "Quar 14 Pluv -356"  },
    { "20 Germ -300",  "20 Germ -300",  "Dec 20 Germ -300"   },
    { "2 Comp -239",   "Gen -239",      "Gen -239"           },
    { "15 Vent -232",  "15 Vent -232",  "Quin 15 Vent -232"  },
    { "22 Prai -144",  "22 Prai -144",  "Duo 22 Prai -144"   },
    { "12 Mess -112",  "12 Mess -112",  "Duo 12 Mess -112"   },
    { "6 Ther -76",    "6 Ther -76",    "Sext 6 Ther -76"    },
    { "1 Mess -24",    "1 Mess -24",    "Prim 1 Mess -24"    },
    { "14 Ther 27",    "14 Ther 27",    "Quar 14 Ther 27"    },
    { "6 Germ 47",     "6 Germ 47",     "Sext 6 Germ 47"     },
    { "28 Germ 111",   "28 Germ 111",   "Oct 28 Germ 111"    },
    { "7 Fruc 137",    "7 Fruc 137",    "Sept 7 Fruc 137"    },
    { "7 Vend 150",    "7 Vend 150",    "Sept 7 Vend 150"    },
    { "29 Germ 151",   "29 Germ 151",   "Non 29 Germ 151"    },
    { "15 Vend 152",   "15 Vend 152",   "Quin 15 Vend 152"   },
    { "27 Vent 200",   "27 Vent 200",   "Sept 27 Vent 200"   },
    { "6 Vent 204",    "6 Vent 204",    "Sext 6 Vent 204"    },
    { "20 Brum 247",   "20 Brum 247",   "Dec 20 Brum 247"    },
    { "30 Mess 302",   "30 Mess 302",   "Dec 30 Mess 302"    }
};

void TestDef_fr::setUp()
{
    m_cal = new Calendars(Init_script_default);
    m_sid = m_cal->get_scheme( "fr" );
}

void TestDef_fr::tearDown()
{
    delete m_cal;
}

void TestDef_fr::testCreation()
{
    CPPUNIT_ASSERT( m_sid != NULL );
    Scheme_info info;
    m_cal->get_scheme_info( &info, m_sid );
    string str = "French Republic";
    CPPUNIT_ASSERT_EQUAL( str, info.name );
    str = "fr";
    CPPUNIT_ASSERT_EQUAL( str, info.code );
    str = "fr";
    CPPUNIT_ASSERT_EQUAL( str, info.grammar_code );
    CPPUNIT_ASSERT( info.vocab_codes.size() == info.vocab_names.size() );
    for( size_t i = 0 ; i < info.vocab_codes.size() ; i++ ) {
        str.clear();
        if( info.vocab_codes[i] == "frm" ) {
            str = "Month names";
        }
        if( info.vocab_codes[i] == "frcomp" ) {
            str = "Complementary Days";
        }
        if( info.vocab_codes[i] == "frdd" ) {
            str = "Decade Day";
        }
        if( info.vocab_codes[i] == "w" ) {
            str = "Weekday names";
        }
        CPPUNIT_ASSERT( str != "" );
        CPPUNIT_ASSERT_EQUAL( str, info.vocab_names[i] );
    }
    SchemeFormatInfo formats;
    m_cal->get_input_info( &formats, m_sid );
    for( size_t i = 0 ; i < formats.descs.size() ; i++ ) {
        for( size_t j = 0 ; j < formats.descs[i].codes.size() ; j++ ) {
            string fcode = formats.descs[i].codes[j].code;
            if( fcode == "dmy" || fcode == "dmy+" || fcode == "cdmy" ||
                fcode == "out" || fcode == "dcdmy" || fcode == "wcdmy" ||
                fcode == "cdmy-" || fcode == "dcdmy-" || fcode == "wcdmy-" )
            {
                str = "Day Month Year";
            } else if( fcode == "ymd" ) {
                str = "Year Month Day";
            } else {
                CPPUNIT_ASSERT( false ); // Shouldn't be here.
            }
            CPPUNIT_ASSERT_EQUAL( str, formats.descs[i].desc );
        }
    }
    m_cal->get_output_info( &formats, m_sid );
    for( size_t i = 0 ; i < formats.descs.size() ; i++ ) {
        for( size_t j = 0 ; j < formats.descs[i].codes.size() ; j++ ) {
            string fcode = formats.descs[i].codes[j].code;
            if( fcode == "cdmy" ) {
                str = "CompDay dd Month yyyy";
            } else if( fcode == "cdmy-" ) {
                str = "CDay dd Mon yyyy";
            } else if( fcode == "dcdmy" ) {
                str = "DecDay CompDay dd Month yyyy";
            } else if( fcode == "dcdmy-" ) {
                str = "DDay CDay dd Mon yyyy";
            } else if( fcode == "dmy" ) {
                str = "dd Mon yyyy";
            } else if( fcode == "dmy+" ) {
                str = "dd Month yyyy";
            } else if( fcode == "out" ) {
                str = "CompDay dd Month an yyyy[X]";
            } else if( fcode == "wcdmy" ) {
                str = "Weekday, CompDay dd Month yyyy";
            } else if( fcode == "wcdmy-" ) {
                str = "WDay, CDay dd Mon yyyy";
            } else if( fcode == "ymd" ) {
                str = "yyyy,Mon,dd";
            } else {
                CPPUNIT_ASSERT( false ); // Shouldn't be here.
            }
            CPPUNIT_ASSERT_EQUAL( str, formats.descs[i].desc );
        }
    }
}

void TestDef_fr::testStrTable_dmy()
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
}

void TestDef_fr::testStrTable_cdmy()
{
    m_cal->set_input_format( m_sid, "cdmy" );
    m_cal->set_output_format( m_sid, "cdmy-" );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string sample( test_strs[i][1] ); 
        Field jdn = m_cal->str_to_jdn( m_sid, sample );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( m_sid, jdn );
        CPPUNIT_ASSERT_EQUAL( sample, str );
    }
}

void TestDef_fr::testStrTable_dcdmy()
{
    m_cal->set_input_format( m_sid, "dcdmy" );
    m_cal->set_output_format( m_sid, "dcdmy-" );
    for( size_t i = 0 ; i < MaxSample ; i++ ) {
        string sample( test_strs[i][2] ); 
        Field jdn = m_cal->str_to_jdn( m_sid, sample );
        CPPUNIT_ASSERT_EQUAL( testJdnValues[i], jdn );
        string str = m_cal->jdn_to_str( m_sid, jdn );
        CPPUNIT_ASSERT_EQUAL( sample, str );
    }
}

void TestDef_fr::testStringInput()
{
    struct data { string in; string out; string o_dmy; Field beg; Field end; } t[] = {
        { "18Vent\303\264se7", "18 Vent\303\264se 7", "18 Vent 7", 2378198, 2378198 },
        { "Frim6", "Frimaire 6", "Frim 6", 2377726, 2377755 },
        { "7", "7", "7", 2378031, 2378396 },
        { "trav8", "F\303\252te du Travail 8", "3 Comp 8", 2378759, 2378759 },
    };
    size_t count = sizeof(t) / sizeof(data);

    m_cal->set_input_format( m_sid, "cdmy" );
    m_cal->set_output_format( m_sid, "cdmy" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].out, str );
        if( rl.size() ) {
            CPPUNIT_ASSERT_EQUAL( t[i].beg, rl[0].jdn1 );
            size_t j = rl.size() - 1;
            CPPUNIT_ASSERT_EQUAL( t[i].end, rl[j].jdn2 );
        }
    }

    m_cal->set_input_format( m_sid, "dmy" );
    m_cal->set_output_format( m_sid, "dmy" );
    for( size_t i = 0 ; i < count ; i++ ) {
        RangeList rl = m_cal->str_to_rangelist( m_sid, t[i].in );
        string str = m_cal->rangelist_to_str( m_sid, rl );
        CPPUNIT_ASSERT_EQUAL( t[i].o_dmy, str );
        if( rl.size() ) {
            CPPUNIT_ASSERT_EQUAL( t[i].beg, rl[0].jdn1 );
            size_t j = rl.size() - 1;
            CPPUNIT_ASSERT_EQUAL( t[i].end, rl[j].jdn2 );
        }
    }
}

// End of src/test/def_fr.cpp file
