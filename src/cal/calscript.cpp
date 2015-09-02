/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscript.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Run script.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th May 2014
 * Copyright:   Copyright (c) 2014 - 2015, Nick Matthews.
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

#include "calscript.h"

#include "cal/calendars.h"
#include "calformat.h"
#include "calgrammar.h"
#include "calgregorian.h"
#include "calparse.h"
#include "calregnal.h"
#include "calscheme.h"
#include "calvocab.h"

#include <cassert>
#include <sstream>

using namespace Cal;
using std::string;
using std::vector;


Script::Script( Calendars* cals, std::istream& in, std::ostream& out ) 
    : m_cals(cals), m_ts(in,out), m_out(&out), m_err(&out)
{
    assert( cals != NULL );
}

bool Script::run()
{
    bool ret = false;
    STokenStream* prev_ts = SValue::set_token_stream( &m_ts );
    for(;;) {
        m_ts.next();
        if( m_ts.current().type() == SToken::STT_End ) {
            ret = true;
            break;
        }
        if( statement() == false ) {
            ret = ( m_ts.errors() == 0 );
            break;
        }
    }
    SValue::set_token_stream( prev_ts );
    return ret;
}

ScriptStore* Script::store() const
{
    return m_cals->get_store();
}

bool Script::statement()
{
    SToken token = m_ts.current();

    if( token.type() == SToken::STT_Name ) {
        string name = token.get_str();
        if( name == "end" ) return false;
        if( name == "clear" ) return do_clear();
        if( name == "if" ) return do_if();
        if( name == "do" ) return do_do();
        if( name == "set" ) return do_set();
        if( name == "let" ) return do_let();
        if( name == "write" ) return do_write();
        if( name == "writeln" ) return do_writeln();
        if( name == "mark" ) return do_mark();
        if( name == "scheme" ) return do_scheme();
        if( name == "vocab" ) return do_vocab();
        if( name == "grammar" ) return do_grammar();
        if( name == "format" ) return do_format( NULL );
        if( store()->exists( name ) ) return do_assign( name );
    } else if( token.type() == SToken::STT_Semicolon ) {
        return true; // Empty statement
    }
    // Assume error
    return error( "Unrecognised statement." ); 
}

bool Script::do_clear()
{
    store()->table.clear();
    return true;
}

bool Script::do_if()
{
    bool done = false, result = false;
    for(;;) {
        if( !result && !done ) {
            bool ok = expr( true ).get( result );
            if( !ok ) {
                error( "Boolean expression expected." );
                return false;
            }
        }
        SToken token = m_ts.current();
        if( result ) {
            // Run the statements
            for(;;) {
                if( token.type() == SToken::STT_End ) {
                    error( "Unexpected end." );
                    return false;
                }
                if( token.type() == SToken::STT_Name ) {
                    string name = token.get_str();
                    if( name == "endif" ) {
                        return true;
                    }
                    if( name == "else" || name == "elseif" ) {
                        break;
                    }
                }
                if( statement() == false ) {
                    return false;
                }
                token = m_ts.next();
            }
            done = true;
            result = false;
        } else {
            // move on to the next "elseif" or "else"
            for(;;) {
                if( token.type() == SToken::STT_End ) {
                    error( "Unexpected end." );
                    return false;
                }
                if( token.type() == SToken::STT_Name ) {
                    string name = token.get_str();
                    if( name == "endif" ) {
                        return true;
                    }
                    if( name == "else" ) {
                        m_ts.next();
                        result = true;
                        break;
                    }
                    if( name == "elseif" ) {
                        break;
                    }
                }
                token = m_ts.next();
            }
        }
        if( done ) {
            // Skip to "endif"
            for(;;) {
                token = m_ts.next();
                if( token.type() == SToken::STT_Name && token.get_str() == "endif" ) {
                    return true;
                }
                if( token.type() == SToken::STT_End ) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Script::do_do()
{
    int start_line = m_ts.get_line();
    string code = m_ts.read_until( "loop", "do" );
    if( code.empty() ) {
        return false;
    }
    int end_line = m_ts.get_line();
    std::istringstream iss( code );
    std::istream* prev_iss = m_ts.reset_in( &iss );
    for( size_t i = 1000 ; i > 0 ; --i ) {  // We have a limit of 1000 reiterations
        m_ts.set_line( start_line );
        bool exit = false;
        SToken token = m_ts.next();
        for(;;) {
            if( token.type() == SToken::STT_End ) {
                exit = true;
                break;
            }
            if( token.type() == SToken::STT_Name ) {
                string name = token.get_str();
                if( name == "until" || name == "while" ) {
                    bool ok = expr( true ).get( exit );
                    if( !ok ) {
                        error( "Boolean expression expected." );
                        break;
                    }
                    if( name == "while" ) {
                        exit = !exit;
                    }
                    if( exit ) {
                        break;
                    }
                    token = m_ts.current();
                    continue;
                }
                if( name == "loop" ) {
                    break;
                }
            }
            if( statement() == false ) {
                exit = true;
                break;
            }
            token = m_ts.next();
        }
        if( exit ) {
            break;
        }
        iss.clear();
        iss.seekg( 0 );
    }
    m_ts.reset_in( prev_iss );
    m_ts.set_line( end_line );
    return true;
}

bool Script::do_set()
{
    string prop;
    SToken token = m_ts.next();
    if( token.type() == SToken::STT_Name ) {
        prop = token.get_str();
        token = m_ts.next();
    }
    string value;
    if( token.type() == SToken::STT_Name || token.type() == SToken::STT_String ) {
        value = token.get_str();
        token = m_ts.next();
    }
    if( token.type() != SToken::STT_Semicolon ) {
        error( "set statement is \"set propery value;\"." );
        return false;
    }
    string scheme, format;
    split_code( &scheme, &format, value );
    SHandle sch = m_cals->get_scheme( scheme );
    if( sch ) {
        if( prop == "input" ) {
            store()->ischeme = sch;
            if( format.size() ) {
                m_cals->set_input_format( sch, format );
            }
        } else if( prop == "output" ) {
            store()->oscheme = sch;
            if( format.size() ) {
                m_cals->set_output_format( sch, format );
            }
        } else {
            error( "Set property \"" + prop + "\" not recognised." );
        }
    } else {
        error( "Set value \"" + value + "\" not recognised." );
    }
    return true;
}

bool Script::do_let()
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_Name ) {
        error( "Variable name expected." );
        return false;
    }
    return do_assign( token.get_str() );
}

bool Script::do_assign( const std::string& name )
{
    SToken token = m_ts.next();
    SValue value;
    if( token.type() == SToken::STT_Equal ) {
        value = expr( true );
    } else if( store()->exists( name ) ) {
        value = store()->table[name];
        switch( token.type() )
        {
        case SToken::STT_PlusEq:
            value.plus( expr( true ) );
            break;
        case SToken::STT_MinusEq:
            value.minus( expr( true ) );
            break;
        case SToken::STT_DivideEq:
            value.divide( expr( true ) );
            break;
        case SToken::STT_StarEq:
            value.multiply( expr( true ) );
            break;
        default:
            error( "Assign operator expected." );
            return false;
        }
    } else {
        error( "'=' expected." );
        return false;
    }
    store()->table[name] = value;
    token = m_ts.current();
    if( token.type() != SToken::STT_Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool Script::do_write()
{
    SValue value = expr( true );
    string str;
    if( value.get( str ) ) {
        *m_out << str;
    } else {
        error( "Unable to output string" );
        return false;
    }
    if( m_ts.current().type() != SToken::STT_Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool Script::do_writeln()
{
    bool ret = do_write();
    *m_out << string("\n");
    return ret;
}

bool Script::do_mark()
{
    string mark;
    expr( true ).get( mark );
    if( mark.empty() ) {
        error( "Mark name string expected." );
        return false;
    }
    m_cals->add_or_replace_mark( mark );
    return true;
}

bool Script::do_scheme()
{
    string code;
    expr( true ).get( code );
    if( code.empty() ) {
        error( "Scheme code missing." );
        return false;
    }
    if( m_cals->get_scheme( code ) != NULL ) {
        error( "Scheme \"" + code + "\" already exists." );
        return false;
    }
    SHandle sch = do_create_scheme( code );
    return m_cals->add_scheme( sch, code );
}

SHandle Script::do_create_scheme( const std::string& code )
{
    if( m_ts.current().type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return NULL;
    }
    Base* base = NULL;
    string name, gmr_code;
    StringVec optfields;
    Scheme_style style = SCH_STYLE_Default;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End ) {
            break;
        }
        if( token.type() == SToken::STT_Semicolon ) {
            continue;
        }
        if( token.type() == SToken::STT_Name ) {
            if( token.get_str() == "name" ) {
                expr( true ).get( name );
            } else if( token.get_str() == "base" ) {
                base = do_base();
            } else if( token.get_str() == "shift" ) {
                base = do_base_shift();
            } else if( token.get_str() == "hybrid" ) {
                base = do_base_hybrid();
            } else if( token.get_str() == "regnal" ) {
                base = do_base_regnal();
            } else if( token.get_str() == "optional" ) {
                optfields = do_string_list();
            } else if( token.get_str() == "grammar" ) {
                expr( true ).get( gmr_code );
            } else if( token.get_str() == "style" ) {
                token = m_ts.next();
                if( token.type() != SToken::STT_Name ) {
                    error( "Style name expected." );
                    continue;
                }
                if( token.get_str() == "hide" ) {
                    style = SCH_STYLE_Hide;
                }
            }
        }
    }
    if( base == NULL ) {
        return NULL;
    }
    for( size_t i = 0 ; i < optfields.size() ; i++ ) {
        base->add_opt_field( optfields[i] );
    }
    base->set_grammar( m_cals->get_grammar( gmr_code ) );
    SHandle sch = new Scheme( name, base );
    sch->set_style( style );
    sch->set_code( code );
    return sch;
}

Base* Script::do_base()
{
    Scheme::BaseScheme bs = Scheme::BS_NULL;
    SToken token = m_ts.next();
    if( token.type() == SToken::STT_Name ) {
        if( token.get_str() == "jdn" ) {
            bs = Scheme::BS_jdn;
        } else if( token.get_str() == "julian" ) {
            bs = Scheme::BS_julian;
        } else if( token.get_str() == "gregorian" ) {
            bs = Scheme::BS_gregorian;
        } else if( token.get_str() == "french" ) {
            bs = Scheme::BS_french;
        } else if( token.get_str() == "hebrew" ) {
            bs = Scheme::BS_hebrew;
        } else if( token.get_str() == "islamic" ) {
            token = m_ts.next();
            bs = Scheme::BS_islamic;
            if( token.type() == SToken::STT_Name ) {
                return Scheme::create_base( bs, token.get_str() );
            } 
        } else {
            error( "Base scheme not recognised." );
        }
    } else {
        error( "Base name expected." );
    }
    return Scheme::create_base( bs );
}

Base* Script::do_base_shift()
{
    string scode;
    expr( true ).get( scode );
    SHandle sch = m_cals->get_scheme( scode );
    Field era = f_invalid;
    if( sch && m_ts.current().type() == SToken::STT_Comma ) {
        era = expr( true ).get_field();
    }
    Base* sbase = NULL;
    if( sch ) {
        sbase = sch->get_base();
    }
    return Scheme::create_base_shift( sbase, era );
}

Base* Script::do_base_hybrid()
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return NULL;
    }
    StringVec fieldnames, ext_fieldnames;
    HybridData data;
    data.start = f_minimum;
    vector<HybridData> data_vec;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End ) {
            break;
        }
        if( token.type() == SToken::STT_Semicolon ) {
            continue;
        }
        SValue value;
        if( token.type() == SToken::STT_Name ) {
            if( token.get_str() == "fields" ) {
                fieldnames = do_string_list();
            } else if( token.get_str() == "extended" ) {
                ext_fieldnames = do_string_list();
            } else if( token.get_str() == "scheme" ) {
                string scode;
                expr( true ).get( scode );
                Scheme* sch;
                if( m_ts.current().type() == SToken::STT_Semicolon ) {
                    sch = m_cals->get_scheme( scode );
                } else {
                    // Create new scheme 
                    sch = do_create_scheme( scode );
                    data.scheme = sch;
                }
                if( sch == NULL ) {
                    error( "Can not find/create scheme " + scode );
                    continue;
                }
                data.base = sch->get_base();
                if( data.ok() ) {
                    data_vec.push_back( data );
                    data = HybridData();
                } else {
                    delete data.scheme;
                    error( "Hybrid data not complete." );
                }
            } else if( token.get_str() == "change" ) {
                data.start = expr(true).get_field();
            }
        }
    }
    return Scheme::create_base_hybrid( fieldnames, ext_fieldnames, data_vec );
}

Base* Script::do_base_regnal()
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return NULL;
    }
    StringVec fieldnames;
    StringVec ext_fieldnames;
    FieldVec fixedfields;
    vector<RegnalEra> eras;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End ) {
            break;
        }
        if( token.type() == SToken::STT_Semicolon ) {
            continue;
        }
        SValue value;
        if( token.type() == SToken::STT_Name ) {
            if( token.get_str() == "fields" ) {
                fieldnames = do_string_list();
            } else if( token.get_str() == "extended" ) {
                ext_fieldnames = do_string_list();
            } else if( token.get_str() == "fixed" ) {
                fixedfields = do_fixed_fields( fieldnames );
            } else if( token.get_str() == "era" ) {
                RegnalEra era;
                StringVec fnames(fieldnames);
                fnames.insert( fnames.end(), ext_fieldnames.begin(), ext_fieldnames.end() );
                if( do_regnal_era( era, fnames ) ) {
                    eras.push_back( era );
                }
            }
        }
    }
    return Scheme::create_base_regnal( fieldnames, ext_fieldnames, fixedfields, eras );
}

bool Script::do_regnal_era( RegnalEra& era, StringVec& fieldnames )
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return NULL;
    }
    Range range( f_minimum, f_maximum );
    StringVec matchs;
    SHandle sch;
    bool local = true;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End ) {
            break;
        }
        if( token.type() == SToken::STT_Semicolon ) {
            continue;
        }
        SValue value;
        if( token.type() == SToken::STT_Name ) {
            if( token.get_str() == "range" ) {
                SValue value = expr( true );
                range = value.get_range();
            } else if( token.get_str() == "scheme" ) {
                SValue value = expr( true );
                string scode;
                value.get( scode );
                if( m_ts.current().type() == SToken::STT_Semicolon ) {
                    sch = m_cals->get_scheme( scode );
                    local = false;
                } else {
                    // Create new scheme 
                    sch = do_create_scheme( scode );
                }
            }
        } else if( token.type() == SToken::STT_match ) { // "match" is a keyword
            matchs = do_string_list();
            assert( matchs.size() % 2 == 0 ); // must be even number
        }
    }
    era.begin = range.jdn1;
    era.end = range.jdn2;
    era.base = sch->get_base();
    era.scheme = sch;
    era.local = local;
    era.xref.clear();
    for( size_t i = 0 ; i < fieldnames.size() ; i++ ) {
        string fieldname = fieldnames[i];
        for( size_t j = 0 ; j < matchs.size() ; j += 2 ) {
            if( fieldname == matchs[j+1] ) {
                fieldname = matchs[j];
                break;
            }
        }
        int index = era.base->get_fieldname_index( fieldname );
        era.xref.push_back( index );
    }
    return true;
}

bool Script::do_vocab()
{
    string code;
    expr( true ).get( code );
    if( code.empty() ) {
        error( "Vocab code missing." );
        return false;
    }
    if( m_cals->get_vocab( code ) != NULL ) {
        error( "vocab \"" + code + "\" already exists." );
        return false;
    }
    Vocab* voc = m_cals->create_vocab( code );
    string str;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End ) {
            break; // All done.
        } else if( token.type() == SToken::STT_Name ) {
            string name = token.get_str();
            if( name == "name" ) {
                str = expr( true ).get_str();
                voc->set_name( str );
            } else if( name == "fieldname" ) {
                str = expr( true ).get_str();
                voc->set_fieldname( str );
            } else if( name == "lang" ) {
                str = expr( true ).get_str();
                voc->set_lang( str );
            } else if( name == "stylename" ) {
                str = expr( true ).get_str();
                voc->set_full_style_name( str );
                if( m_ts.current().type() == SToken::STT_Comma ) {
                    str = expr( true ).get_str();
                }
                voc->set_abbrev_style_name( str );
            } else if( name == "tokens" ) {
                do_vocab_tokens( voc );
            } else {
                error( "Unknown vocab subcommand." );
            }
        }
    }
    return true;
}

bool Script::do_vocab_tokens( Vocab* voc )
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return false;
    }
    for(;;) {
        // Look ahead for '}'
        if( m_ts.next().type() == SToken::STT_RCbracket ||
            m_ts.current().type() == SToken::STT_End ) {
            break; // All done.
        }
        SValue value = expr( false );
        if( value.type() != SValue::SVT_Field ) {
            error( "Number expected." );
            return false;
        }
        Field number = value.get_field();
        if( m_ts.current().type() != SToken::STT_Comma ) {
            error( "',' expected." );
            return false;
        }
        string name;
        expr( true ).get( name );
        string abbrev;
        if( m_ts.current().type() == SToken::STT_Comma ) {
            expr( true ).get( abbrev );
        }
        if( m_ts.current().type() != SToken::STT_Semicolon ) {
            error( "';' expected." );
            return false;
        }
        voc->add_token( number, name, abbrev );
    }
    return true;
}

bool Script::do_grammar()
{
    string code;
    expr( true ).get( code );
    if( code.empty() ) {
        error( "Grammar code missing." );
        return false;
    }
    if( m_cals->get_grammar( code ) != NULL ) {
        error( "grammar \"" + code + "\" already exists." );
        return false;
    }
    Grammar* gmr = m_cals->create_grammar( code );
    string str;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_LCbracket ) {
            continue; 
        } else if( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End ) {
            break; // All done.
        } else if( token.type() == SToken::STT_Name ) {
            string name = token.get_str();
            if( name == "vocabs" ) {
                do_grammar_vocabs( gmr );
            } else if( name == "format" ) {
                do_format( gmr );
            } else if( name == "pref" ) {
                expr( true ).get( str );
                gmr->set_pref( str );
            } else if( name == "alias" ) {
                do_grammar_alias( gmr );
            } else if( name == "inherit" ) {
                do_grammar_inherit( gmr );
            } else if( name == "optional" ) {
                StringVec optfields = do_string_list();
                gmr->set_opt_fieldnames( optfields );
            } else if( name == "rank" ) {
                StringVec rankfields = do_string_list();
                gmr->set_rank_fieldnames( rankfields );
            }
        }
    }
    return true;
}

bool Script::do_grammar_vocabs( Grammar* gmr )
{
    StringVec vocabs = do_string_list();
    for( size_t i = 0 ; i < vocabs.size() ; i++ ) {
        Vocab* voc = m_cals->get_vocab( vocabs[i] );
        gmr->add_vocab( voc );
    }
    return true;
}

bool Script::do_grammar_alias( Grammar* gmr )
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_Name ) {
        error( "Alias type name expected." );
        return false;
    }
    string alias = token.get_str();
    token = m_ts.next();
    if( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return false;
    }
    string str1, str2;
    StringVec pairs;
    for(;;) {
        // Look ahead for '}'
        if( m_ts.next().type() == SToken::STT_RCbracket ||
            m_ts.current().type() == SToken::STT_End ) {
            break; // All done.
        }
        SValue value = expr( false );
        if( m_ts.current().type() != SToken::STT_Comma ) {
            error( "',' expected." );
            return false;
        }
        value.get( str1 );
        value = expr( true );
        if( m_ts.current().type() != SToken::STT_Semicolon ) {
            error( "';' expected." );
            return false;
        }
        value.get( str2 );
        if( str1.size() && str2.size() ) {
            pairs.push_back( str1 );
            pairs.push_back( str2 );
        }
    }
    gmr->add_alias( alias, pairs );
    return true;
}

void Script::do_grammar_inherit( Grammar* gmr )
{
    string code;
    expr( true ).get( code );
    gmr->set_inherit( m_cals, code );
}

// If parsing the format within a grammar then gmr is not NULL
// and the Format code just contains just the Format code, as in "fmt".
// If parsing the format in the global space then gmr is NULL and
// the Grammar to be used should be encoded into the Format code,
// as in "gmr:fmt"
bool Script::do_format( Grammar* gmr )
{
    string code, format, informat, separators;
    Format::Use usefor = Format::Use_inout;
    StringVec rankfields;
    expr( true ).get( code );
    if( code.empty() ) {
        error( "Format code missing." );
        return false;
    }
    if( m_ts.current().type() == SToken::STT_Comma ) {
        expr( true ).get( format );
        if( format.empty() ) {
            error( "Format missing." );
            return false;
        }
        if( m_ts.current().type() != SToken::STT_Semicolon ) {
            error( "';' expected." );
            return false;
        }
    } else if( m_ts.current().type() == SToken::STT_LCbracket ) {
        for(;;) {
            SToken token = m_ts.next();
            if( token.type() == SToken::STT_RCbracket ||
                token.type() == SToken::STT_End ) {
                break;
            }
            if( token.type() == SToken::STT_Semicolon ) {
                continue;
            }
            if( token.type() == SToken::STT_Name ) {
                string name = token.get_str();
                if( name == "output" ) {
                    usefor = Format::Use_output;
                } else if( name == "inout" ) {
                    usefor = Format::Use_inout;
                } else if( name == "strict" ) {
                    usefor = Format::Use_strict;
                } else if( name == "input" ) {
                    expr( true ).get( informat );
                    continue;
                } else if( name == "separators" ) {
                    expr( true ).get( separators );
                    continue;
                } else if( name == "rank" ) {
                    rankfields = do_string_list();
                    continue;
                } else {
                    error( "Expected format sub-statement." );
                    continue;
                }
                expr( true ).get( format );
            }
        }
    } else {
        error( "',' or '{' expected." );
        return false;
    }

    if( format.empty() ) {
        error( "Format string not found." );
        return false;
    }
    Format* fmt;
    if( gmr == NULL ) {
        fmt = m_cals->create_format( code );
        if( fmt == NULL ) {
            error( "Unable to create format." );
            return false;
        }
        gmr = fmt->get_owner();
        if( gmr == NULL ) {
            error( "Grammar not found." );
            return false;
        }
    } else {
        fmt = gmr->create_format( code );
        if( fmt == NULL ) {
            error( "Unable to create format." );
            return false;
        }
    }
    fmt->set_format( format, usefor );
    if( informat.size() ) {
        fmt->set_format( informat, Format::Use_input );
    }
    if( separators.size() ) {
        fmt->set_separators( separators );
    }
    if( rankfields.size() ) {
        fmt->set_rank_fieldnames( rankfields );
    }
    return true;
}

StringVec Script::do_string_list()
{
    StringVec vec;
    for(;;) {
        SValue value = expr( true );
        string str;
        if( ! value.get( str ) || str.empty() ) {
            break;
        }
        vec.push_back( str );
        if( m_ts.current().type() != SToken::STT_Comma ) {
            break;
        }
    }
    return vec;
}

FieldVec Script::do_fixed_fields( const StringVec& fieldnames )
{
    FieldVec vec(fieldnames.size()+1,f_invalid);
    for(;;) {
        SValue value = expr( true );
        string name;
        if( ! value.get( name ) || name.empty() ) {
            error( "Field name expected." );
            break;
        }
        if( m_ts.current().type() != SToken::STT_Comma ) {
            error( "Equal sign expected." );
            break;
        }
        value = expr( true );
        Field field;
        if( ! value.get( field ) ) {
            error( "Number expected." );
            break;
        }
        for( size_t i = 0 ; i < fieldnames.size() ; i++ ) {
            if( name == fieldnames[i] ) {
                vec[i+1] = field;
                break;
            }
        }
        if( m_ts.current().type() != SToken::STT_Comma ) {
            break;
        }
    }
    return vec;
}

SValue Script::expr( bool get )
{
    SValue left = compare( get );
    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_or:
            left.logical_or( compare( true ) );
            break;
        case SToken::STT_and:
            left.logical_and( compare( true ) );
        default:
            return left;
        }
    }
}

SValue Script::compare( bool get )
{
    SValue left = combine( get );
    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Equal:
            left.equal( combine( true ) );
            break;
        case SToken::STT_NotEqual:
            left.equal( combine( true ) );
            left.logical_not();
            break;
        case SToken::STT_GtThan:
            left.greater_than( combine( true ) );
            break;
        case SToken::STT_GtThanEq:
            left.less_than( combine( true ) );
            left.logical_not();
            break;
        case SToken::STT_LessThan:
            left.less_than( combine( true ) );
            break;
        case SToken::STT_LessThanEq:
            left.greater_than( combine( true ) );
            left.logical_not();
            break;
        default:
            return left;
        }
    }
}

SValue Script::combine( bool get )
{
    SValue left = range( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_UNION:
            left.rlist_union( range( true ) );
            break;
        case SToken::STT_INTERSECTION:
            left.intersection( range( true ) );
            break;
        case SToken::STT_REL_COMPLEMENT:
            left.rel_complement( range( true ) );
            break;
        case SToken::STT_SYM_DIFFERENCE:
            left.sym_difference( range( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::range( bool get )
{
    SValue left = sum( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Tilde:
            left.range_op( sum( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::sum( bool get )
{
    SValue left = term( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Plus:
            left.plus( term( true ) );
            break;
        case SToken::STT_Minus:
            left.minus( term( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::term( bool get )
{
    SValue left = subscript( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Star:
            left.multiply( subscript( true ) );
            break;
        case SToken::STT_Divide:
            left.divide( subscript( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::subscript( bool get )
{
    SValue left = primary( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_LSbracket:
            left.subscript_op( primary( true ) );
            if( m_ts.current().type() != SToken::STT_RSbracket ) {
                error( "']' expected." );
                break;
            }
            m_ts.next();
            break;
        default:
            return left;
        }
    }
}

SValue Script::primary( bool get )
{
    SValue value;
    SToken token = get ? m_ts.next() : m_ts.current();
    switch( token.type() )
    {
    case SToken::STT_Number:
        value.set_field( token.get_field() );
        m_ts.next();
        break;
    case SToken::STT_Bool:
        value.set_bool( token.get_bool() );
        m_ts.next();
        break;
    case SToken::STT_Qmark:
        value.set_field( f_invalid );
        m_ts.next();
        break;
    case SToken::STT_String:
        value.set_str( token.get_str() );
        m_ts.next();
        break;
    case SToken::STT_Name:
        value = get_value_var( token.get_str() );
        m_ts.next();
        break;
    case SToken::STT_Lbracket:
        value = expr( true );
        if( m_ts.current().type() != SToken::STT_Rbracket ) {
            error( "')' expected." );
            break;
        }
        m_ts.next();
        break;
    case SToken::STT_LCbracket:
        value = fields_expr( true );
        m_ts.next();
        break;
    case SToken::STT_date:
        {
            value = primary( true );
            SHandle sch = store()->ischeme;
            value.set( m_cals->str_to_rangelist( sch, value.get_str() ) );
        }
        break;
    case SToken::STT_mask:
        {
            value = primary( true );
            SHandle sch = store()->ischeme;
            value.set_fields( m_cals->str_to_fieldvec( sch, value.get_str() ) );
        }
        break;
    case SToken::STT_str_cast:
        value = str_cast();
        break;
    case SToken::STT_Minus:
        value = primary( true );
        value.negate();
        break;
    case SToken::STT_not:
        value = primary( true );
        value.logical_not();
        break;
    case SToken::STT_COMPLEMENT:
        value = primary( true );
        value.compliment();
        break;
    default:
        error( "Primary value expected." );
    }
    return value;
}

SValue Script::fields_expr( bool get )
{
    FieldVec fields;
    SToken token = get ? m_ts.next() : m_ts.current();

    for(;;) {
        switch( token.type() )
        {
        case SToken::STT_End:
        case SToken::STT_RCbracket:
            return SValue( fields );
        case SToken::STT_Comma:
            break;
        default:
            {
                SValue value = expr( false );
                Field field = value.get_field();
                if( value.is_error() ) {
                    error( "Field value expected." );
                } else {
                    fields.push_back( field );
                }
            }
            token = m_ts.current();
            continue;
        }
        token = m_ts.next();
    }
}

SValue Script::str_cast()
{
    SToken token = m_ts.next();
    SHandle sch = NULL;
    string sig, scode, fcode;
    if( token.type() == SToken::STT_Comma ) {
        // Includes scheme:format signiture
        expr( true ).get( sig );
        split_code( &scode, &fcode, sig );
        sch = m_cals->get_scheme( scode );
    }
    if( sch == NULL ) {
        sch = store()->oscheme;
    }
    SValue value = primary( false );
    if( sch ) {
        RangeList rlist;
        value.get_rlist( rlist );
        value.set_str( sch->rangelist_to_str( rlist, fcode ) );
    } else {
        error( "Valid scheme not set." );
    }
    return value;
}

SValue Script::get_value_var( const string& name )
{
    if( name == "true" ) {
        return SValue( true );
    }
    if( name == "false" ) {
        return SValue( false );
    }
    if( name == "invalid" ) {
        return SValue( f_invalid );
    }
    if( name == "past" ) {
        return SValue( f_minimum );
    }
    if( name == "future" ) {
        return SValue( f_maximum );
    }
    if( name == "today" ) {
        return SValue( Gregorian::today() );
    }
    if( store()->table.count( name ) ) {
        return store()->table.find( name )->second;
    }
    SValue value;
    value.set_error( "Variable \"" + name + "\" not found." );
    return value;
}

string Script::get_name_or_string( const SToken& token ) const
{
    string str;
    if( token.type() == SToken::STT_Name || token.type() == SToken::STT_String ) {
        str = token.get_str();
    }
    return str;
}

// End of src/cal/calscript.cpp file
