/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calcalendars.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Main library API.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     19th September 2013
 * Copyright:   Copyright (c) 2013-2014, Nick Matthews.
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
#include "calgrammar.h"
#include "calparse.h"
#include "calrecord.h"
#include "calscheme.h"
#include "calscript.h"
#include "calscriptstore.h"
#include "calversion.h"
#include "calvocab.h"

#include <cassert>
#include <sstream>

using namespace std;
using namespace Cal;


Calendars::Calendars( Init_schemes init )
{
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
    for( SHandleMap::iterator it = m_shandles.begin() ; it != m_shandles.end() ; it++ ) {
        delete it->second;
    }
    for( GrammarMap::iterator it = m_grammars.begin() ; it != m_grammars.end() ; it++ ) {
        delete it->second;
    }
    for( VocabMap::iterator it = m_vocabs.begin() ; it != m_vocabs.end() ; it++ ) {
        delete it->second;
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
    if( ! scheme ) {
        return FieldVec();
    }
    string scode, fcode, mstr;
    split_code_date( &scode, &fcode, &mstr, str );
    if( scode.size() ) {
        scheme = get_scheme( scode );
    }
    if( scheme == NULL ) {
        return FieldVec(0);
    }
    Record rec( scheme->get_base(), mstr, fcode );
    return rec.get_fieldvec();
}

Field Calendars::str_to_jdn( SHandle scheme, const string& str )
{
    string sch, fmt, date;
    split_code_date( &sch, &fmt, &date, str );
    if( sch.size() ) {
        scheme = get_scheme( sch );
    }
    if( scheme == NULL ) {
        return f_invalid;
    }
    return scheme->str_to_jdn( date, fmt );
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

Grammar* Calendars::get_grammar( const string& code ) const
{
    if( m_grammars.count( code ) > 0 ) {
        return m_grammars.find( code )->second;
    }
    return NULL;
}

Vocab* Calendars::get_vocab( const string& code ) const
{
    if( m_vocabs.count( code ) > 0 ) {
        return m_vocabs.find( code )->second;
    }
    return NULL;
}

bool Calendars::add_scheme( SHandle sch, const string& code )
{
    if( sch == NULL || sch->is_ok() == false ) {
        delete sch;
        return false;
    }
    m_shandles[code] = sch;
    return true;
}

Vocab* Calendars::create_vocab( const string& code )
{
    Vocab* voc = new Vocab( code );
    m_vocabs[code] = voc;
    return voc;
}

Grammar* Calendars::create_grammar( const string& code )
{
    Grammar* gmr = new Grammar( code );
    m_grammars[code] = gmr;
    return gmr;
}

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
            base1 = base2 = sch->get_base();
        }
        if( fcode1.empty() && base1 != NULL ) {
            fcode1 = base1->get_input_format();
        }
        fcode2 = fcode1;
        str2 = str1;
    } else {
        // Two date strings.
        string temp = str.substr( 0, pos );
        split_code_date( &scode, &fcode1, &str1, temp );
        if( scode.size() ) {
            SHandle sch = get_scheme( scode );
            base1 = sch->get_base();
        }
        if( fcode1.empty() && base1 != NULL ) {
            fcode1 = base1->get_input_format();
        }
        temp = str.substr( pos + 1 );
        scode.clear();
        split_code_date( &scode, &fcode2, &str2, temp );
        if( scode.size() ) {
            SHandle sch = get_scheme( scode );
            base2 = sch->get_base();
        }
        if( fcode2.empty() && base2 != NULL ) {
            fcode2 = base2->get_input_format();
        }
    }
    RangeList ranges;
    if( base1 == NULL || base2 == NULL ) {
        return ranges;
    }

    Record mask1( base1, str1, fcode1 ), mask2( base2, str2, fcode2 );
    Record rec1(base1), rec2(base2);
    bool ret1 = rec1.set_fields_as_begin_first( mask1.get_field_ptr() );
    bool ret2 = rec2.set_fields_as_begin_last( mask2.get_field_ptr() );
    while( ret1 && ret2 ) {
        Range range;
        range.jdn1 = rec1.get_jdn();
        range.jdn2 = rec2.get_jdn();
        if( range.jdn1 > range.jdn2 ) {
            break;
        }
        ranges.push_back( range );
        ret1 = rec1.set_fields_as_next_first( mask1.get_field_ptr() );
        ret2 = rec2.set_fields_as_next_last( mask2.get_field_ptr() );
    }
    return ranges;
}

// End of src/cal/calcalendars.cpp file
