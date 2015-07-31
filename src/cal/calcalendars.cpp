/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calcalendars.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Main library API.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     19th September 2013
 * Copyright:   Copyright (c) 2013-2015, Nick Matthews.
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

#include "cal/calendars.h"

#include "calbase.h"
#include "caldefscripts.h"
#include "calformat.h"
#include "calgrammar.h"
#include "calmark.h"
#include "calparse.h"
#include "calrecord.h"
#include "calscheme.h"
#include "calscript.h"
#include "calscriptstore.h"
#include "calversion.h"
#include "calvocab.h"

#include <cassert>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Cal;


Calendars::Calendars( Init_schemes init )
{
    Mark* mark = new Mark( "" );
    m_marks.push_back( mark );

    m_store = new ScriptStore;
    switch( init )
    {
    case Init_script_none:
        break;
    case Init_script_default:
        for( size_t i = 0 ; i < cal_default_scripts_size ; i++ ) {
            run_script( cal_default_scripts[i] );
        }
        break;
    }
}

Calendars::~Calendars()
{
    for( size_t i = 0 ; i < m_marks.size() ; i++ ) {
        delete m_marks[i];
    }
    delete m_store;
}

const char* Calendars::version()
{
    return cal_version;
}

string Calendars::run_script( const string& script )
{
    istringstream iss( script );
    ostringstream oss;
    Script scr( this, iss, oss );
    scr.run();
    return oss.str();
}

string Calendars::run_script_file( const string& filename )
{
    ifstream ifs( filename.c_str() );
    ostringstream oss;
    Script scr( this, ifs, oss );
    scr.run();
    return oss.str();
}

SHandle Calendars::get_scheme( const string& code ) const
{
    if( m_shandles.count( code ) > 0 ) {
        return m_shandles.find( code )->second;
    }
    return NULL;
}

SchemeList Calendars::get_scheme_list() const
{
    SchemeList slist;
    SchemeData sdata;
    SHandleMap::const_iterator it;
    for( it = m_shandles.begin() ; it != m_shandles.end() ; it++ ) {
        if( it->second->get_style() == SCH_STYLE_Hide ) {
            continue;
        }
        sdata.code = it->first;
        sdata.handle = it->second;
        sdata.name = sdata.handle->get_name();
        slist.push_back( sdata );
    }
    return slist;
}

void Calendars::get_scheme_info( Scheme_info* info, SHandle scheme ) const
{
    if( scheme ) {
        scheme->get_info( info );
    }
}

void Calendars::get_scheme_input( SchemeFormats* info, SHandle scheme ) const
{
    if( scheme ) {
        scheme->get_input( info );
    }
}

void Calendars::get_scheme_output( SchemeFormats* info, SHandle scheme ) const
{
    if( scheme ) {
        scheme->get_output( info );
    }
}

bool Calendars::get_vocab_info( Vocab_info* info, const string& code ) const
{
    Vocab* voc = get_vocab( code );
    if( voc == NULL ) {
        return false;
    }
    voc->get_info( info );
    return true;
}

void Calendars::set_input_format( SHandle scheme, const std::string& code )
{
    if( scheme ) {
        scheme->set_input_format( code );
    }
}

void Calendars::set_output_format( SHandle scheme, const std::string& code )
{
    if( scheme ) {
        scheme->set_output_format( code );
    }
}

Field Calendars::fieldvec_to_jdn( SHandle scheme, const FieldVec& fieldv )
{
    if( scheme ) {
        return scheme->fieldvec_to_jdn( fieldv );
    }
    return f_invalid;
}

FieldVec Calendars::jdn_to_fieldvec( SHandle scheme, Field jdn )
{
    if( scheme ) {
        return scheme->jdn_to_fieldvec( jdn );
    }
    return FieldVec();
}

string Calendars::fieldvec_to_str( SHandle scheme, const FieldVec& fieldv )
{
    if( scheme ) {
        Record rec( scheme->get_base(), &fieldv[0], fieldv.size() );
        return rec.get_str();
    }
    return "";
}

FieldVec Calendars::str_to_fieldvec( SHandle scheme, const string& str )
{
    string scode, fcode, dstr;
    split_code_date( &scode, &fcode, &dstr, str );
    if( scode.size() ) {
        scheme = get_scheme( scode );
        if( scheme && fcode.empty() ) {
            fcode = scheme->get_pref_input_format();
        }
    }
    if( scheme == NULL ) {
        return FieldVec(0);
    }
    Record rec( scheme->get_base(), dstr, fcode );
    return rec.get_fieldvec();
}

Field Calendars::str_to_jdn( SHandle scheme, const string& str )
{
    string scode, fcode, dstr;
    split_code_date( &scode, &fcode, &dstr, str );
    if( scode.size() ) {
        scheme = get_scheme( scode );
        if( scheme && fcode.empty() ) {
            fcode = scheme->get_pref_input_format();
        }
    }
    if( scheme == NULL ) {
        return f_invalid;
    }
    return scheme->str_to_jdn( dstr, fcode );
}

string Calendars::jdn_to_str( SHandle scheme, Field jdn )
{
    if( scheme ) {
        return scheme->jdn_to_str( jdn );
    }
    return "";
}

Range Calendars::str_to_range( SHandle scheme, const string& str )
{
    RangeList rlist = range_str_to_rangelist( scheme, str );
    switch( rlist.size() )
    {
    case 0:
        return Range( f_invalid, f_invalid );
    case 1:
        return rlist[0];
    }
    return Range( rlist[0].jdn1, rlist[rlist.size()-1].jdn2 );
}

string Calendars::range_to_str( SHandle scheme, Range range )
{
    if( scheme ) {
        return scheme->range_to_str( range );
    }
    return "";
}

RangeList Calendars::str_to_rangelist( SHandle scheme, const string& input )
{
    RangeList rlist;
    string str;
    size_t pos, cpos = 0;
    do {
        pos = input.find( range_div, cpos );
        if( pos != string::npos ) {
            str = input.substr( cpos, pos - cpos );
            cpos = pos + 1;
        } else {
            str = input.substr( cpos );
        }
        RangeList rl = range_str_to_rangelist( scheme, str );
        if( rl.size() ) {
            rlist.insert( rlist.end(), rl.begin(), rl.end() );
        }
    } while( pos != string::npos );
    return rlist;
}

RangeList Calendars::expr_str_to_rangelist( SHandle scheme, const string& str )
{
    RangeList rlist;
    if( scheme == NULL ) {
        return rlist;
    }
    string script;
    if( scheme ) {
        script = "set input \"" + scheme->get_code() + "\";\n";
    }
    script += "let result = " + parse_date_expr( str ) + ";";

    istringstream iss( script );
    ostringstream oss;
    Script scr( this, iss, oss );
    scr.run();

    SValueMap table = get_store()->table;
    if( table.count( "result" ) > 0 ) {
        table.find( "result" )->second.get_rlist( rlist );
    }
    return rlist;
}

string Calendars::rangelist_to_str( SHandle scheme, const RangeList& ranges )
{
    return scheme->rangelist_to_str( ranges );
}

Field Calendars::add_to_jdn( SHandle scheme, Field jdn, Field value, Unit unit, Norm norm )
{
    if( scheme ) {
        return scheme->add_to_jdn( jdn, value, unit, norm );
    }
    return f_invalid;
}

bool Calendars::str_to_rel_info( SHandle scheme, const string& str, Rel_info* info ) const
{
    if( scheme ) {
        return scheme->str_to_rel_info( str, info );
    }
    return false;
}

RangeList Calendars::rel_rangelist( SHandle scheme, const RangeList& ranges, Rel_info* info )
{
    if( scheme ) {
        return scheme->rel_rangelist( ranges, info );
    }
    return RangeList();
}

bool Calendars::add_scheme( SHandle sch, const string& code )
{
    if( sch == NULL || sch->is_ok() == false  // Only add initialised schemes
        || m_shandles.count( code )           // that are not aready there. 
    ) {
        delete sch;
        return false;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size()-1]->add_scheme( sch );
    m_shandles[code] = sch;
    return true;
}

Grammar* Calendars::create_grammar( const string& code )
{
    Grammar* gmr = new Grammar( code );
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size()-1]->add_grammar( gmr );
    m_grammars[code] = gmr;
    return gmr;
}

Grammar* Calendars::get_grammar( const string& code ) const
{
    if( m_grammars.count( code ) > 0 ) {
        return m_grammars.find( code )->second;
    }
    return NULL;
}

Vocab* Calendars::create_vocab( const string& code )
{
    Vocab* voc = new Vocab( code );
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size()-1]->add_vocab( voc );
    m_vocabs[code] = voc;
    return voc;
}

Vocab* Calendars::get_vocab( const string& code ) const
{
    if( m_vocabs.count( code ) > 0 ) {
        return m_vocabs.find( code )->second;
    }
    return NULL;
}

Format* Calendars::create_format( const string& code )
{
    size_t pos = code.find( ':' );
    if( pos == string::npos ) {
        return NULL;
    }
    string gcode = code.substr( 0, pos );
    string fcode = code.substr( pos + 1 );
    Grammar* gmr = get_grammar( gcode );
    if( gmr == NULL || gmr->get_format( fcode ) != NULL ) {
        return NULL;
    }
    Format* fmt = new Format( fcode, gmr );
    if( !gmr->add_format( fmt ) ) {
        delete fmt;
        return NULL;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size()-1]->add_format( fmt );
    return fmt;
}

void Calendars::add_or_replace_mark( std::string& name )
{
    assert( m_marks.size() > 0 );
    assert( name.size() > 0 );
    size_t end, pos;
    end = pos = m_marks.size() - 1;
    while( pos != 0 && name != m_marks[pos]->get_name() ) {
        --pos;
    }
    if( pos > 0 ) {
        for( size_t i = end ; i >= pos ; --i ) {
            string code;
            // We must remove the Format's before removing the Grammar's 
            for(;;) { 
                code = m_marks[i]->remove_next_format();                
                if( code.empty() ) {
                    break;
                }
            }
            for(;;) { 
                code = m_marks[i]->remove_next_scheme();                
                if( code.empty() ) {
                    break;
                }
                m_shandles.erase( code );
            }
            for(;;) { 
                code = m_marks[i]->remove_next_grammar();                
                if( code.empty() ) {
                    break;
                }
                m_grammars.erase( code );
            }
            for(;;) { 
                code = m_marks[i]->remove_next_vocab();                
                if( code.empty() ) {
                    break;
                }
                m_vocabs.erase( code );
            }
            delete m_marks[i];
            m_marks.pop_back();
        }
    }
    Mark* mark = new Mark( name );
    m_marks.push_back( mark );
}

namespace {

    // Sets range and returns true if the masks can create a valid range.
    // Sets up range but returns false if the masks can create a valid range only
    // by ignoring optional fields.
    // Sets range to invalid and returns false if the masks cannot create a valid
    // range.
    bool set_range_as_begin( Range* range, const Record& mask1, const Record& mask2 )
    {
        Record rec1( mask1.get_base() );
        Record rec2( mask2.get_base() );
        bool ret1 = rec1.set_fields_as_begin_first( mask1.get_field_ptr(), false );
        bool ret2 = rec2.set_fields_as_begin_last( mask2.get_field_ptr(), false );
        if( !ret1 || !ret2 ) {
            range->jdn1 = f_invalid;
            return false;
        }
        range->jdn1 = rec1.get_jdn();
        range->jdn2 = rec2.get_jdn();
        if( range->jdn1 == f_invalid || range->jdn2 == f_invalid ) {
            range->jdn1 = f_invalid;
            return false;
        }
        ret1 = rec1.set_fields_as_begin_first( mask1.get_field_ptr(), true );
        ret2 = rec2.set_fields_as_begin_last( mask2.get_field_ptr(), true );
        if( ret1 && ret2 ) {
            Range r;
            r.jdn1 = rec1.get_jdn();
            r.jdn2 = rec2.get_jdn();
            if( r.jdn1 != range->jdn1 || r.jdn2 != range->jdn2 ) {
                if( r.jdn1 > r.jdn2 ) {
                    return false;
                }
                *range = r;
                return true;
            }
        }
        return ret1 && ret2;
    }

    // Using the initial value of range, attempts to adjust the value of range
    // to the next value.
    // If it fails, it sets the range invalid and returns false.
    // If it succeeds, it will attempt to correct for optional fields - if this
    // fails, the uncorrected range is set and the the function returns false.
    bool set_range_as_next( Range* range, const Record& mask1, const Record& mask2 )
    {
        Record rec1( mask1.get_base(), range->jdn1 );
        Record rec2( mask2.get_base(), range->jdn2 );
        bool ret1 = rec1.set_fields_as_next_first( mask1.get_field_ptr() );
        bool ret2 = rec2.set_fields_as_next_last( mask2.get_field_ptr() );


        if( !ret1 || !ret2 ) {
            range->jdn1 = f_invalid;
            return false;
        }
        range->jdn1 = rec1.get_jdn();
        range->jdn2 = rec2.get_jdn();
        if( range->jdn1 > range->jdn2 ||
            range->jdn1 == f_invalid || range->jdn2 == f_invalid )
        {
            range->jdn1 = f_invalid;
            return false;
        }
        ret1 = rec1.correct_fields_as_first( mask1.get_field_ptr() );
        ret2 = rec2.correct_fields_as_last( mask2.get_field_ptr() );
        if( ret1 && ret2 ) {
            Range r;
            r.jdn1 = rec1.get_jdn();
            r.jdn2 = rec2.get_jdn();
            if( r.jdn1 != range->jdn1 || r.jdn2 != range->jdn2 ) {
                if( r.jdn1 > r.jdn2 ) {
                    return false;
                }
                *range = r;
                return true;
            }
        }
        return ret1 && ret2;
    }

} // namespace

// Convert a string written as a shorthand or single longhand range
// to a rangelist. Handles scheme:format# prefix. Scheme may be NULL;
RangeList Calendars::range_str_to_rangelist( SHandle scheme, const string& str )
{
    Base* base1 = NULL;
    Base* base2 = NULL;
    if( scheme ) {
        base1 = base2 = scheme->get_base();
    }
    string str1, str2, scode, fcode1, fcode2;
    size_t pos = str.find( range_sep );
    if( pos == string::npos ) {
        // Single, poss shorthand, date string.
        split_code_date( &scode, &fcode1, &str1, str );
        if( scode.size() ) {
            SHandle sch = get_scheme( scode );
            if( sch ) {
                base1 = base2 = sch->get_base();
                if( fcode1.empty() ) {
                    fcode1 = sch->get_pref_input_format();
                }
            }
        }
        if( fcode1.empty() && base1 != NULL ) {
            fcode1 = base1->get_input_fcode();
        }
        fcode2 = fcode1;
        str2 = str1;
    } else {
        // Two date strings.
        string temp = str.substr( 0, pos );
        split_code_date( &scode, &fcode1, &str1, temp );
        if( scode.size() ) {
            SHandle sch = get_scheme( scode );
            if( sch ) {
                base1 = sch->get_base();
                if( fcode1.empty() ) {
                    fcode1 = sch->get_pref_input_format();
                }
            }
        }
        if( fcode1.empty() && base1 != NULL ) {
            fcode1 = base1->get_input_fcode();
        }
        temp = str.substr( pos + 1 );
        scode.clear();
        split_code_date( &scode, &fcode2, &str2, temp );
        if( scode.size() ) {
            SHandle sch = get_scheme( scode );
            if( sch ) {
                base2 = sch->get_base();
                if( fcode2.empty() ) {
                    fcode2 = sch->get_pref_input_format();
                }
            }
        }
        if( fcode2.empty() && base2 != NULL ) {
            fcode2 = base2->get_input_fcode();
        }
    }
    RangeList ranges;
    if( base1 == NULL || base2 == NULL ) {
        return ranges;
    }

    Record mask1( base1, str1, fcode1 ), mask2( base2, str2, fcode2 );
    Range range;
    bool ret = set_range_as_begin( &range, mask1, mask2 );
    while( range.jdn1 != f_invalid ) {
        if( ret ) {
            if( ranges.size() && ranges[ranges.size()-1].jdn2+1 >= range.jdn1 ) {
                ranges[ranges.size()-1].jdn2 = range.jdn2;
            } else {
                ranges.push_back( range );
            }
        }
        ret = set_range_as_next( &range, mask1, mask2 );
    }
    return ranges;
}

// End of src/cal/calcalendars.cpp file
