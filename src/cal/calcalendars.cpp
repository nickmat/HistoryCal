/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calcalendars.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Main library API.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     19th September 2013
 * Copyright:   Copyright (c) 2013 ~ 2020, Nick Matthews.
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
#include "calformatiso.h"
#include "calformattext.h"
#include "calformatunit.h"
#include "calfunction.h"
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

using namespace Cal;
using std::string;


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
            string error = run_script( cal_default_scripts[i].script );
            if ( !error.empty() ) {
                m_init_error += "Module: \"" +
                    string(cal_default_scripts[i].module) + "\"\n" + error;
            }
        }
        break;
    }
}

Calendars::~Calendars()
{
    for( size_t i = m_marks.size() ; i > 0 ; --i ) {
        delete m_marks[i-1];
    }
    while ( pop_store() );
    delete m_store;
}

const char* Calendars::version()
{
    return cal_version;
}

string Calendars::get_init_error()
{
    return m_init_error;
}

string Calendars::get_last_error()
{
    return m_last_error;
}

string Calendars::run_script( const string& script )
{
    std::istringstream iss( script );
    std::ostringstream oss;
    Script scr( this, iss, oss );
    scr.run();
    return oss.str();
}

string Calendars::run_script_file( const string& filename )
{
    std::ifstream ifs( filename.c_str() );
    std::ostringstream oss;
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

void Calendars::get_input_info( SchemeFormatInfo* info, SHandle scheme ) const
{
    if( scheme ) {
        const Base* base = scheme->get_base();
        string inputcode = scheme->get_input_fcode();
        base->get_input_info( info, inputcode );
    }
}

void Calendars::get_output_info( SchemeFormatInfo* info, SHandle scheme ) const
{
    if( scheme ) {
        const Base* base = scheme->get_base();
        string outcode = scheme->get_output_fcode();
        base->get_output_info( info, outcode );
    }
}

void Calendars::get_format_info( Format_info* info, SHandle scheme, const string& fcode ) const
{
    if( scheme ) {
        const Base* base = scheme->get_base();
        Format* fmt = base->get_format( fcode );
        if( fmt ) {
            fmt->get_info( info );
        }
    }
}

void Calendars::get_format_text_info( FormatText_info* info, SHandle scheme, const string& fcode ) const
{
    if( scheme ) {
        const Base* base = scheme->get_base();
        FormatText* fmt = dynamic_cast<FormatText*>( base->get_format( fcode ) );
        if( fmt ) {
            fmt->get_info( &info->info );
            info->control_str = fmt->get_control();
        }
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

void Calendars::set_input_format( SHandle scheme, const string& code )
{
    if( scheme ) {
        scheme->set_input_format( code );
    }
}

void Calendars::set_output_format( SHandle scheme, const string& code )
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

RangeList Calendars::fieldvec_to_rlist( SHandle scheme, const FieldVec& fieldv )
{
    if( scheme ) {
        return scheme->fieldvec_to_rlist( fieldv );
    }
    return RangeList();
}

FieldVec Calendars::jdn_to_fieldvec( SHandle scheme, Field jdn )
{
    if( scheme ) {
        return scheme->jdn_to_fieldvec( jdn );
    }
    return FieldVec();
}

string Calendars::fieldvec_to_str( SHandle scheme, const FieldVec& fieldv, const string& fcode )
{
    if( scheme ) {
        Record rec( scheme->get_base(), &fieldv[0], fieldv.size() );
        return rec.get_str( fcode );
    }
    return "";
}

FieldVec Calendars::str_to_fieldvec( SHandle scheme, const string& str, const string& fcode )
{
    if( scheme == NULL ) {
        return FieldVec(0);
    }
    string fmtcode;
    if( fcode.empty() ) {
        fmtcode = scheme->get_pref_input_format();
    } else {
        fmtcode = fcode;
    }
    Record rec( scheme->get_base(), str, fmtcode, RB_none );
    return rec.get_fieldvec();
}

Field Calendars::str_to_jdn( SHandle scheme, const string& str, const string& fcode )
{
    RangeList rlist = str_to_rangelist( scheme, str, fcode );
    if ( rlist.size() == 1 && rlist[0].jdn1 == rlist[0].jdn2 ) {
        return rlist[0].jdn1;
    }
    return f_invalid;
}

string Calendars::jdn_to_str( SHandle scheme, Field jdn, const string& fcode )
{
    if( scheme ) {
        return scheme->jdn_to_str( jdn, fcode );
    }
    return "";
}

Range Calendars::str_to_range( SHandle scheme, const string& str, const string& fcode )
{
    RangeList rlist = str_to_rangelist( scheme, str, fcode );
    switch( rlist.size() )
    {
    case 0:
        return Range( f_invalid, f_invalid );
    case 1:
        return rlist[0];
    }
    return Range( rlist[0].jdn1, rlist[rlist.size()-1].jdn2 );
}

string Calendars::range_to_str( SHandle scheme, Range range, const string& fcode )
{
    if( scheme ) {
        return scheme->range_to_str( range, fcode );
    }
    return "";
}

RangeList Calendars::str_to_rangelist(
    SHandle scheme, const string& input, const string& fcode )
{
    if( scheme == nullptr ) {
        return RangeList(0);
    }
    const Base* base = scheme->get_base();
    assert( base != nullptr );

    string fc = fcode.empty() ? scheme->get_input_fcode() : fcode;
    Format* fmt = base->get_format( fc );
    if( fmt == nullptr ) {
        return RangeList(0);
    }
    return fmt->string_to_rlist( base, input );
}

RangeList Calendars::expr_str_to_rangelist( SHandle scheme, const string& str )
{
    RangeList rlist;
    string script;
    if( scheme ) {
        script = "set input \"" + scheme->get_code() + "\";\n";
    }
    script += "let result = " + parse_date_expr( str ) + ";";

    std::istringstream iss( script );
    std::ostringstream oss;
    Script scr( this, iss, oss );
    scr.run();

    SValue value;
    if ( get_store()->get( &value, "result" ) ) {
        value.get_rlist( rlist );
    }
    return rlist;
}

string Calendars::rangelist_to_str( SHandle scheme, const RangeList& ranges, const string& fcode )
{
    if ( scheme ) {
        return scheme->rangelist_to_str( ranges, fcode );
    }
    return "";
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
    if ( sch == NULL || sch->is_ok() == false  // Only add initialised schemes
        || m_shandles.count( code )           // that are not aready there. 
        ) {
        delete sch;
        return false;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_scheme( sch );
    m_shandles[code] = sch;
    return true;
}

bool Calendars::add_temp_scheme( SHandle sch, const string& code )
{
    if ( sch == nullptr || sch->is_ok() == false  // Only add initialised schemes
        || m_shandles.count( code )               // that are not aready there. 
        ) {
        return false;
    }
    m_shandles[code] = sch;
    return true;
}

void Calendars::remove_temp_scheme( const string& code )
{
    m_shandles.erase( code );
}

bool Calendars::add_grammar( Grammar* gmr, const string& code )
{
    if ( gmr == nullptr || gmr->is_ok() == false  // Only add initialised schemes
        || m_grammars.count( code )               // that are not aready there. 
        ) {
        delete gmr;
        return false;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_grammar( gmr );
    m_grammars[code] = gmr;
    return true;
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

FormatText* Calendars::create_format_text( const string& code, Grammar* gmr )
{
    size_t pos = code.find( ':' );
    if( pos == string::npos ) {
        if( gmr == NULL ) {
            return NULL;
        }
        return gmr->create_format_text( code );
    }
    if( gmr != NULL ) {
        return NULL;
    }
    string gcode = code.substr( 0, pos );
    string fcode = code.substr( pos + 1 );
    gmr = get_grammar( gcode );
    if( gmr == NULL || gmr->get_format( fcode ) != NULL ) {
        return NULL;
    }
    FormatText* fmt = new FormatText( fcode, gmr );
    if( !gmr->add_format( fmt ) ) {
        delete fmt;
        return NULL;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size()-1]->add_format( fmt );
    return fmt;
}

FormatIso* Calendars::create_format_iso( const string& code, Grammar* gmr, const StringVec& rules )
{
    size_t pos = code.find( ':' );
    if( pos == string::npos ) {
        if( gmr == NULL ) {
            return NULL;
        }
        return gmr->create_format_iso( code, rules );
    }
    if( gmr != NULL ) {
        return NULL;
    }
    string gcode = code.substr( 0, pos );
    string fcode = code.substr( pos + 1 );
    gmr = get_grammar( gcode );
    if( gmr == NULL || gmr->get_format( fcode ) != NULL ) {
        return NULL;
    }
    FormatIso* fmt = new FormatIso( fcode, gmr, rules );
    if( !gmr->add_format( fmt ) ) {
        delete fmt;
        return NULL;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size()-1]->add_format( fmt );
    return fmt;
}

FormatUnit* Calendars::create_format_unit( const string& code, Grammar* gmr )
{
    size_t pos = code.find( ':' );
    if ( pos == string::npos ) {
        if ( gmr == nullptr ) {
            return nullptr;
        }
        return gmr->create_format_unit( code );
    }
    if ( gmr != nullptr ) {
        return nullptr;
    }
    string gcode = code.substr( 0, pos );
    string fcode = code.substr( pos + 1 );
    gmr = get_grammar( gcode );
    if ( gmr == nullptr || gmr->get_format( fcode ) != nullptr ) {
        return nullptr;
    }
    FormatUnit* fmt = new FormatUnit( fcode, gmr );
    if ( !gmr->add_format( fmt ) ) {
        delete fmt;
        return nullptr;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_format( fmt );
    return fmt;
    return nullptr;
}

Function* Calendars::create_function( const string & code )
{
    Function* fun = new Function( code );
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_function( fun );
    m_functions[code] = fun;
    return fun;
}

Function* Calendars::get_function( const string & code ) const
{
    if ( m_functions.count( code ) > 0 ) {
        return m_functions.find( code )->second;
    }
    return NULL;
}

void Calendars::add_or_replace_mark( const string& name )
{
    clear_mark( name );
    Mark* mark = new Mark( name );
    m_marks.push_back( mark );
}

bool Calendars::clear_mark( const string& name )
{
    assert( !m_marks.empty() );
    assert( !name.empty() );
    size_t end, pos;
    end = pos = m_marks.size() - 1;
    while( pos != 0 && name != m_marks[pos]->get_name() ) {
        --pos;
    }
    if( pos == 0 ) {
        return false; // Can't find mark name.
    }
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
        for ( ;;) {
            code = m_marks[i]->remove_next_vocab();
            if ( code.empty() ) {
                break;
            }
            m_vocabs.erase( code );
        }
        for ( ;;) {
            code = m_marks[i]->remove_next_function();
            if ( code.empty() ) {
                break;
            }
            m_functions.erase( code );
        }
        delete m_marks[i];
        m_marks.pop_back();
    }
    return true;
}

void Calendars::set_ischeme( Scheme* sch )
{
    int i = int( m_marks.size() ) - 1;
    if ( i >= 0 ) {
        m_marks[i]->set_ischeme( sch );
    }
}

void Calendars::set_oscheme( Scheme* sch )
{
    int i = int( m_marks.size() ) - 1;
    if ( i >= 0 ) {
        m_marks[i]->set_oscheme( sch );
    }
}

Scheme* Calendars::get_ischeme() const
{
    Scheme* sch = nullptr;
    for ( int i = int( m_marks.size() ) - 1; i >= 0 && sch == nullptr; --i ) {
        sch = m_marks[i]->get_ischeme();
    }
    return sch;
}

Scheme* Calendars::get_oscheme() const
{
    Scheme* sch = nullptr;
    for ( int i = int( m_marks.size() ) - 1; i >= 0 && sch == nullptr; --i ) {
        sch = m_marks[i]->get_oscheme();
    }
    return sch;
}

void Calendars::push_store()
{
    m_store = new ScriptStore( m_store );
}

bool Calendars::pop_store()
{
    ScriptStore* store = m_store->get_prev();
    if ( store ) {
        delete m_store;
        m_store = store;
        return true;
    }
    return false;
}

Field Calendars::evaluate_field(
    const string& expression, const Record& record, const BoolVec* reveal )
{
    std::istringstream iss( expression );
    std::ostringstream oss;
    Script scr( this, iss, oss );
    Field field = scr.evaluate_field( record, reveal );
    m_last_error = oss.str();
    return field;
}

void Calendars::evaluate_record(
    const string& expression, Record* record, const string& fname, Field field )
{
    std::istringstream iss( expression );
    std::ostringstream oss;
    Script scr( this, iss, oss );
    scr.evaluate_record( record, fname, field );
    m_last_error = oss.str();
}

// End of src/cal/calcalendars.cpp file
