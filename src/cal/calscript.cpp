/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscript.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Run script.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th May 2014
 * Copyright:   Copyright (c) 2014 ~ 2020, Nick Matthews.
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
#include "calelement.h"
#include "calformatiso.h"
#include "calformattext.h"
#include "calformatunit.h"
#include "calfunction.h"
#include "calgrammar.h"
#include "calgregorian.h"
#include "calparse.h"
#include "calrecord.h"
#include "calregnal.h"
#include "calscheme.h"
#include "caltext.h"
#include "calvocab.h"

#include <cassert>
#include <sstream>

using namespace Cal;
using std::string;
using std::vector;


Script::Script( Calendars* cals, std::istream& in, std::ostream& out ) 
    : m_cals(cals), m_ts(in,out), m_out(&out), m_err(&out), m_record(nullptr)
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

Field Script::evaluate_field( const Record& record )
{
    STokenStream* prev_ts = SValue::set_token_stream( &m_ts );
    m_record = &record;
    SValue value = expr( true );
    m_record = nullptr;
    Field field;
    if ( !value.get( field ) ) {
        field = f_invalid;
    }
    SValue::set_token_stream( prev_ts );
    return field;
}

ScriptStore* Script::store() const
{
    return m_cals->get_store();
}

bool Script::statement()
{
    SToken token = m_ts.current();

    if (token.type() == SToken::STT_Name) {
        string name = token.get_str();
        if ( name == "end" ) return false;
        if ( name == "mark" ) return do_mark();
        if ( name == "clear" ) return do_clear();
        if ( name == "if" ) return do_if();
        if ( name == "do" ) return do_do();
        if ( name == "set" ) return do_set();
        if ( name == "let" ) return do_let();
        if ( name == "write" ) return do_write();
        if ( name == "writeln" ) return do_writeln();
        if ( name == "scheme" ) return do_scheme();
        if ( name == "call" ) return do_call();
        if ( name == "vocab" ) return do_vocab();
        if ( name == "grammar" ) return do_grammar();
        if ( name == "format" ) return do_format( nullptr );
        if ( name == "function" ) return do_function();
        if ( store()->exists( name ) ) return do_assign( name );
    }
    else if ( token.type() == SToken::STT_Semicolon ) {
        return true; // Empty statement
    }
    // Assume error
    return error("Unrecognised statement.");
}

bool Script::do_mark()
{
    string mark = get_name_or_primary( true );
    if( m_ts.current().type() != SToken::STT_Semicolon ) {
        error( "';' expected." );
        return false;
    }
    if( mark.empty() ) {
        error( "Mark name string expected." );
        return false;
    }
    m_cals->add_or_replace_mark( mark );
    return true;
}

bool Script::do_clear()
{
    string mark;
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_Semicolon ) {
        mark = get_name_or_primary( false );
        token = m_ts.current();
    }
    if( token.type() != SToken::STT_Semicolon ) {
        error( "';' expected." );
        return false;
    }
    if( !mark.empty() ) {
        m_cals->clear_mark( mark );
    }
    store()->clear();
    return true;
}

bool Script::do_if()
{
    bool done = false, result = false;
    int nested = 0;
    const char* enderr = "if ended unexpectedly.";
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
                    error(enderr);
                    return false;
                }
                if( token.type() == SToken::STT_Name ) {
                    string name = token.get_str();
                    if ( name == "endif" ) {
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
            nested = 0;
            for(;;) {
                if ( token.type() == SToken::STT_At ) {
                    token = m_ts.next();
                    token = m_ts.next(); // Ignore function name
                }
                if( token.type() == SToken::STT_End ) {
                    error(enderr);
                    return false;
                }
                if( token.type() == SToken::STT_Name ) {
                    string name = token.get_str();
                    if ( name == "if" ) {
                        nested++;
                    } else if ( name == "endif" ) {
                        if ( nested > 0 ) {
                            --nested;
                        } else {
                            return true;
                        }
                    } else if ( name == "else" && nested == 0 ) {
                        m_ts.next();
                        result = true;
                        break;
                    } else if ( name == "elseif" && nested == 0 ) {
                        break;
                    }
                }
                token = m_ts.next();
            }
        }
        if( done ) {
            // Skip to "endif"
            nested = 0;
            for(;;) {
                token = m_ts.next();
                if ( token.type() == SToken::STT_End ) {
                    error( enderr );
                    return false;
                }
                if ( token.type() == SToken::STT_Name ) {
                    string name = token.get_str();
                    if ( name == "if" ) {
                        nested++;
                    } else if ( name == "endif" ) {
                        if ( nested > 0 ) {
                            --nested;
                        } else {
                            return true;
                        }
                    }
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
        error( "Do loop not terminated." );
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
                if ( name == "loop" ) {
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
    } else {
        error( "Set property expected." );
        return false;
    }
    string value = get_name_or_primary( true );
    if( m_ts.current().type() != SToken::STT_Semicolon ) {
        error( "set statement is \"set propery value;\"." );
        return false;
    }
    string scheme, format;
    split_code( &scheme, &format, value );
    SHandle sch = m_cals->get_scheme( scheme );
    if( sch ) {
        if ( prop == "input" ) {
            m_cals->set_ischeme( sch );
            if ( !format.empty() ) {
                m_cals->set_input_format( sch, format );
            }
        } else if ( prop == "output" ) {
            m_cals->set_oscheme( sch );
            if ( !format.empty() ) {
                m_cals->set_output_format( sch, format );
            }
        } else if ( prop == "inout" ) {
            m_cals->set_ischeme( sch );
            m_cals->set_oscheme( sch );
            if ( !format.empty() ) {
                m_cals->set_input_format( sch, format );
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
    } else if ( store()->get( &value, name ) ) {
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
    store()->set( name, value );
    if ( value.type() == SValue::SVT_Error ) {
        m_ts.skip_to( SToken::STT_Semicolon );
    }
    if( m_ts.current().type() != SToken::STT_Semicolon ) {
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
    if ( value.type() == SValue::SVT_Error ) {
        m_ts.skip_to( SToken::STT_Semicolon );
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

bool Script::do_scheme()
{
    string code = get_name_or_primary( true );
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
        return nullptr;
    }
    Base* base = nullptr;
    string name, gmr_code;
    Grammar* gmr = nullptr;
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
            } else if( token.get_str() == "grammar" ) {
                token = m_ts.next();
                if ( token.type() == SToken::STT_LCbracket ) {
                    gmr = m_cals->create_grammar( "" );
                    if ( !do_anon_grammar( gmr ) ) {
                        error( "Unable to create grammar." );
                    }
                } else {
                    gmr_code = get_name_or_primary( false );
                    gmr = m_cals->get_grammar( gmr_code );
                }
            } else if( token.get_str() == "style" ) {
                string str = get_name_or_primary( true );
                if ( str == "hide" ) {
                    style = SCH_STYLE_Hide;
                } else if ( str != "none" ) {
                    error( "Style name expected." );
                }
            } else {
                error( "Scheme sub-statement expected." );
            }
        }
    }
    if( base == nullptr ) {
        if ( gmr_code.empty() ) {
            delete gmr;
        }
        error( "Scheme not created." );
        return nullptr;
    }
    if ( gmr == nullptr ) {
        gmr = m_cals->create_grammar( "" );
    }
    if ( !base->attach_grammar( gmr ) ) {
        if ( gmr_code.empty() ) {
            delete gmr;
        }
        delete base;
        error( "Unable to attach grammar." );
        return nullptr;
    }
    
    SHandle sch = new Scheme( name, base );
    sch->set_style( style );
    sch->set_code( code );
    return sch;
}

Base* Script::do_base()
{
    Scheme::BaseScheme bs = Scheme::BS_NULL;
    string data;
    SToken token = m_ts.next();
    if( token.type() == SToken::STT_Name ) {
        if( token.get_str() == "jdn" ) {
            bs = Scheme::BS_jdn;
        } else if( token.get_str() == "julian" ) {
            bs = Scheme::BS_julian;
        } else if( token.get_str() == "gregorian" ) {
            bs = Scheme::BS_gregorian;
        } else if( token.get_str() == "isoweek" ) {
            bs = Scheme::BS_isoweek;
        } else if( token.get_str() == "isoordinal" ) {
            bs = Scheme::BS_isoordinal;
        } else if( token.get_str() == "french" ) {
            bs = Scheme::BS_french;
        } else if( token.get_str() == "hebrew" ) {
            bs = Scheme::BS_hebrew;
        } else if( token.get_str() == "islamic" ) {
            bs = Scheme::BS_islamic;
        } else if ( token.get_str() == "chinese" ) {
            bs = Scheme::BS_chinese;
        } else if ( token.get_str() == "easter" ) {
            bs = Scheme::BS_easter;
        } else {
            error( "Base scheme not recognised." );
            return NULL;
        }
        token = m_ts.next();
        if( token.type() != SToken::STT_Semicolon ) {
            data = get_name_or_primary( false );
        }
    } else {
        error( "Base name expected." );
    }
    return Scheme::create_base( bs, data );
}

Base* Script::do_base_shift()
{
    string scode = get_name_or_primary( true );
    SHandle sch = m_cals->get_scheme( scode );
    Field era = f_invalid;
    if( sch && m_ts.current().type() == SToken::STT_Comma ) {
        expr( true ).get( era );
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
    StringVec fieldnames;
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
                fieldnames = get_string_list( true );
            } else if( token.get_str() == "scheme" ) {
                string scode = get_name_or_primary( true );
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
                if ( !expr( true ).get( data.start ) ) {
                    error( "Change start must be Field." );
                }
            } else {
                error( "Unrecognised statement." );
            }
        }
    }
    return Scheme::create_base_hybrid( fieldnames, data_vec );
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
                fieldnames = get_string_list( true );
            } else if( token.get_str() == "extended" ) {
                ext_fieldnames = get_string_list( true );
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
        return false;
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
                if ( !expr( true ).get( range ) ) {
                    error( "Range value expected." );
                }
            } else if( token.get_str() == "scheme" ) {
                string scode = get_name_or_primary( true );
                if( m_ts.current().type() == SToken::STT_Semicolon ) {
                    sch = m_cals->get_scheme( scode );
                    local = false;
                } else {
                    // Create new scheme 
                    sch = do_create_scheme( scode );
                }
            } else if ( token.get_str() == "match" ) {
                matchs = get_string_list( true );
                assert( matchs.size() % 2 == 0 ); // must be even number
            }
        }
    }
    if ( sch == NULL ) {
        error( "Unable to create scheme." );
        return false;
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
    string code = get_name_or_primary( true );
    if( code.empty() ) {
        error( "Vocab code missing." );
        return false;
    }
    if( m_cals->get_vocab( code ) != NULL ) {
        error( "vocab \"" + code + "\" already exists." );
        return false;
    }
    if( m_ts.current().type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
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
                expr( true ).get( str );
                voc->set_name( str );
            } else if( name == "fieldname" ) {
                str = get_name_or_primary( true );
                voc->set_fieldname( str );
            } else if( name == "lang" ) {
                str = get_name_or_primary( true );
                voc->set_lang( str );
            } else if( name == "pseudo" ) {
                StringVec pseudos = get_string_list( true );
                voc->set_pseudo_names( pseudos );
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
    while ( m_ts.next().type() != SToken::STT_RCbracket &&
        m_ts.current().type() != SToken::STT_End )
    {
        Field number;
        if ( !expr( false ).get( number ) ) {
            error( "Number expected." );
            return false;
        }
        if( m_ts.current().type() != SToken::STT_Comma ) {
            error( "',' expected." );
            return false;
        }
        StringVec names = get_string_list( true );
        string name, abbrev;
        if ( names.size() > 0 ) {
            name = names[0];
        }
        if ( names.size() > 1 ) {
            abbrev = names[1];
        }
        voc->add_token( number, name, abbrev );
    }
    return true;
}

bool Script::do_grammar()
{
    string code = get_name_or_primary( true );
    if ( code.empty() ) {
        error( "Grammar code missing." );
        return false;
    }
    if ( m_cals->get_grammar( code ) != nullptr ) {
        error( "Grammar \"" + code + "\" already exists." );
        return false;
    }
    Grammar* gmr = do_create_grammar( code );
    return m_cals->add_grammar( gmr, code );
}

Grammar* Script::do_create_grammar( const string& code )
{
    if ( m_ts.current().type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return nullptr;
    }
    Grammar* gmr = new Grammar( code, m_cals );
    string str;
    for ( ;;) {
        SToken token = m_ts.next();
        if ( token.type() == SToken::STT_LCbracket ) {
            continue;
        } else if ( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End ) {
            break; // All done.
        } else if ( token.type() == SToken::STT_Name ) {
            string name = token.get_str();
            if ( name == "vocabs" ) {
                do_grammar_vocabs( gmr );
            } else if ( name == "format" ) {
                do_format( gmr );
            } else if ( name == "pref" ) {
                str = get_name_or_primary( true );
                gmr->set_pref( str );
            } else if ( name == "element" ) {
                do_grammar_element( gmr );
            } else if ( name == "alias" ) {
                do_grammar_alias( gmr );
            } else if ( name == "inherit" ) {
                str = get_name_or_primary( true );
                gmr->set_inherit( str );
            } else if ( name == "fields" ) {
                StringVec basefields = get_string_list( true );
                gmr->set_base_fieldnames( basefields );
            } else if ( name == "optional" ) {
                StringVec optfields = get_string_list( true );
                gmr->set_opt_fieldnames( optfields );
            } else if ( name == "rank" ) {
                StringVec rankfields = get_string_list( true );
                gmr->set_rank_fieldnames( rankfields );
            }
        } else {
            error( "Grammar sub-statement expected." );
        }
    }
    gmr->constuct();
    return gmr;
}

bool Script::do_anon_grammar( Grammar* gmr )
{
    string str;
    for ( ;;) {
        SToken token = m_ts.next();
        if ( token.type() == SToken::STT_LCbracket ) {
            continue;
        } else if ( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End ) {
            break; // All done.
        } else if ( token.type() == SToken::STT_Name ) {
            string name = token.get_str();
            if ( name == "vocabs" ) {
                do_grammar_vocabs( gmr );
            } else if ( name == "format" ) {
                do_format( gmr );
            } else if ( name == "pref" ) {
                str = get_name_or_primary( true );
                gmr->set_pref( str );
            } else if ( name == "element" ) {
                do_grammar_element( gmr );
            } else if ( name == "alias" ) {
                do_grammar_alias( gmr );
            } else if ( name == "inherit" ) {
                str = get_name_or_primary( true );
                gmr->set_inherit( str );
            } else if ( name == "fields" ) {
                StringVec basefields = get_string_list( true );
                gmr->set_base_fieldnames( basefields );
            } else if ( name == "optional" ) {
                StringVec optfields = get_string_list( true );
                gmr->set_opt_fieldnames( optfields );
            } else if ( name == "rank" ) {
                StringVec rankfields = get_string_list( true );
                gmr->set_rank_fieldnames( rankfields );
            }
        } else {
            error( "Grammar sub-statement expected." );
        }
    }
    return true;
}

bool Script::do_grammar_vocabs( Grammar* gmr )
{
    StringVec vocabs = get_string_list( false );
    for( size_t i = 0 ; i < vocabs.size() ; i++ ) {
        Vocab* voc = m_cals->get_vocab( vocabs[i] );
        gmr->add_vocab( voc );
    }
    return true;
}

bool Script::do_grammar_element( Grammar* gmr )
{
    string name = get_name_or_primary( true );
    if ( m_ts.current().type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return false;
    }
    ElementField ef;
    for ( ;; ) {
        SToken token = m_ts.next();
        if ( token.type() == SToken::STT_RCbracket ||
            token.type() == SToken::STT_End )
        {
            break; // All done.
        }
        if ( token.type() == SToken::STT_Name ) {
            string sub = token.get_str();
            if ( sub == "output" ) {
                ef.out_expression = m_ts.read_until( ";", "" );
            } else if ( sub == "pseudo" ) {
                expr( true ).get( ef.pseudo );
            }
        }
    }
    if ( name.empty() || ef.out_expression.empty() ) {
        return false;
    }
    gmr->add_element( name, ef );
    return true;
}

bool Script::do_grammar_alias( Grammar* gmr )
{
    string alias = get_name_or_primary( true );
    if( m_ts.current().type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return false;
    }
    StringVec pairs;
    for(;;) {
        // Look ahead for '}'
        m_ts.next();
        if( m_ts.current().type() == SToken::STT_RCbracket ||
            m_ts.current().type() == SToken::STT_End )
        {
            break; // All done.
        }
        string str1 = get_name_or_primary( false );
        if( str1.empty() ) {
            error( "Alias string or code expected." );
            return false;
        }
        if( m_ts.current().type() != SToken::STT_Comma ) {
            error( "',' expected." );
            return false;
        }
        string str2 = get_name_or_primary( true );
        if( str2.empty() ) {
            error( "Original string or code expected." );
            return false;
        }
        if( m_ts.current().type() != SToken::STT_Semicolon ) {
            error( "';' expected." );
            return false;
        }
        pairs.push_back( str1 );
        pairs.push_back( str2 );
    }
    gmr->add_alias( alias, pairs );
    return true;
}

// If parsing the format within a grammar then gmr is not NULL
// and the Format code just contains just the Format code, as in "fmt".
// If parsing the format in the global space then gmr is NULL and
// the Grammar to be used should be encoded into the Format code,
// as in "gmr:fmt"
bool Script::do_format( Grammar* gmr )
{
    string format, informat, separators;
    FormatText::Use usefor = FormatText::Use_inout;
    StringVec rankfields, rankoutfields, rules;
    Format_style style = FMT_STYLE_Default;
    string code = get_name_or_primary( true );
    if( code.empty() ) {
        error( "Format code missing." );
        return false;
    }
    if( m_ts.current().type() == SToken::STT_LCbracket ) {
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
                    usefor = FormatText::Use_output;
                } else if( name == "inout" ) {
                    usefor = FormatText::Use_inout;
                } else if( name == "input" ) {
                    expr( true ).get( informat );
                    continue;
                } else if( name == "separators" ) {
                    expr( true ).get( separators );
                    continue;
                } else if ( name == "rank" ) {
                    rankfields = get_string_list( true );
                    continue;
                } else if ( name == "rankout" ) {
                    rankoutfields = get_string_list( true );
                    continue;
                } else if ( name == "rules" ) {
                    rules = get_string_list( true );
                    continue;
                } else if(name == "style" ) {
                    string str = get_name_or_primary( true );
                    if ( str == "hide" ) {
                        style = FMT_STYLE_Hide;
                    } else if ( str != "none" ) {
                        error( "Style name expected." );
                    }
                } else {
                    error( "Expected format sub-statement." );
                    continue;
                }
                expr( true ).get( format );
            }
        }
    } else {
        if( m_ts.current().type() == SToken::STT_Comma ) {
            m_ts.next();
        }
        expr( false ).get( format );
        if( format.empty() ) {
            error( "Format missing." );
            return false;
        }
        if( m_ts.current().type() != SToken::STT_Semicolon ) {
            error( "';' expected." );
            return false;
        }
    }

    Format* fmt = nullptr;
    if( rules.empty() || rules[0] == "text" ) {
        if( format.empty() ) {
            error( "Format string not found." );
            return false;
        }
        FormatText* fmtt = m_cals->create_format_text( code, gmr );
        if( fmtt == nullptr ) {
            error( "Unable to create format." );
            return false;
        }
        if( separators.size() ) {
            fmtt->set_separators( separators );
        }
        if ( rankfields.size() ) {
            fmtt->set_rank_fieldnames( rankfields );
        }
        if ( rankoutfields.size() ) {
            fmtt->set_rankout_fieldnames( rankoutfields );
        }
        fmtt->set_control( format, usefor );
        if( informat.size() ) {
            fmtt->set_control( informat, FormatText::Use_input );
        }
        fmt = fmtt;
    } else if ( rules[0] == "iso8601" ) {
        fmt = m_cals->create_format_iso( code, gmr, rules );
        if ( fmt == nullptr ) {
            error( "Unable to create ISO format." );
            return false;
        }
    } else if ( rules[0] == "units" ) {
        fmt = m_cals->create_format_unit( code, gmr );
        if ( fmt == nullptr ) {
            error( "Unable to create Units format." );
            return false;
        }
    } else {
        error( "Unknown rules statement." );
        return false;
    }
    fmt->set_style( style );
    return true;
}

FieldVec Script::do_fixed_fields( const StringVec& fieldnames )
{
    FieldVec vec(fieldnames.size()+1,f_invalid);
    for(;;) {
        string name = get_name_or_primary( true );
        if( name.empty() ) {
            error( "Field name expected." );
            break;
        }
        if( m_ts.current().type() != SToken::STT_Equal ) {
            error( "Equal sign expected." );
            break;
        }
        Field field;
        if( !expr( true ).get( field ) ) {
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

bool Script::do_function()
{
    string code = get_name_or_primary( true );
    if ( code.empty() ) {
        error( "Function name missing." );
        return false;
    }
    if ( m_cals->get_function( code ) != NULL ) {
        error( "function \"" + code + "\" already exists." );
        return false;
    }
    SToken token = m_ts.current();
    StringVec args;
    SValueVec defs;
    if ( token.type() == SToken::STT_Lbracket ) {
        token = m_ts.next();
        for ( ;;) {
            if ( token.type() == SToken::STT_Rbracket ) {
                break;
            }
            string str = m_ts.current().get_str();
            if( token.type() != SToken::STT_Name || str.empty() ) {
                error( "Argument name expected." );
                return false;
            }
            args.push_back( str );
            token = m_ts.next();
            SValue value;
            if ( token.type() == SToken::STT_Equal ) {
                value = expr( true );
            }
            defs.push_back( value );
            token = m_ts.current();
            if ( token.type() == SToken::STT_Comma ) {
                token = m_ts.next();
            }
        }
        token = m_ts.next();
    }
    if ( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return false;
    }
    int line = m_ts.get_line();
    string script = m_ts.read_until( "}", "{" );
    if ( script.empty() ) {
        error( "Terminating '}' not found." );
        return false;
    }
    Function* fun = m_cals->create_function( code );
    if ( fun == nullptr ) {
        return false;
    }
    fun->set_args( args );
    fun->set_defaults( defs );
    fun->set_line( line );
    fun->set_script( script );
    return true;
}

bool Cal::Script::do_call()
{
    SValue value = function_call();
    if ( value.is_error() ) {
        error( value.get_str() );
        return false;
    }
    if ( m_ts.current().type() != SToken::STT_Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
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
            break;
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
        case SToken::STT_mod:
            left.modulus( subscript( true ) );
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
            {
                SValue right;
                token = m_ts.next();
                if( token.type() == SToken::STT_Dot ) {
                    token = m_ts.next();
                    if( token.type() == SToken::STT_Name ) {
                        right.set_str( token.get_str() );
                        m_ts.next();
                    } else {
                        right = expr( false );
                    }
                    left.property_op( right );
                } else {
                    if( token.type() == SToken::STT_Name ) {
                        right.set_str( token.get_str() );
                        m_ts.next();
                    } else {
                        right = expr( false );
                    }
                    left = do_subscript( left, right );
                }
                if( m_ts.current().type() != SToken::STT_RSbracket ) {
                    error( "']' expected." );
                }
                m_ts.next();
            }
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
        {
            Field field = token.get_field();
            assert( field >= 0 ); // Literals are always positive.
            if( field > f_maximum ) {
                field = f_maximum;
            }
            value.set_field( field );
        }
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
        value = get_record( true );
        m_ts.next();
        break;
    case SToken::STT_date:
        value = date_cast();
        break;
    case SToken::STT_record:
        value = record_cast();
        break;
    case SToken::STT_convert:
        value = convert_cast();
        break;
    case SToken::STT_error:
        value = error_cast();
        break;
    case SToken::STT_str_cast:
        value = str_cast();
        break;
    case SToken::STT_At:
        value = function_call();
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
        value.set_error( "Primary value expected." );
    }
    return value;
}

string Script::get_name_or_primary( bool get )
{
    string str;
    SToken token = get ? m_ts.next() : m_ts.current();
    if( token.type() == SToken::STT_Name ) {
        str = token.get_str();
        m_ts.next();
    } else {
        SValue value = primary( false );
        if ( value.type() == SValue::SVT_Str || value.type() == SValue::SVT_Field ) {
            value.get( str );
        }
    }
    return str;
}

StringVec Script::get_string_list( bool get )
{
    StringVec vec;
    SToken token = get ? m_ts.next() : m_ts.current();
    while( token.type() != SToken::STT_Semicolon &&
        token.type() != SToken::STT_End )
    {
        string str = get_name_or_primary( false );
        if ( !str.empty() ) {
            vec.push_back( str );
        }
        token = m_ts.current();
        if ( token.type() == SToken::STT_Comma ) {
            token = m_ts.next();
        }
    }
    return vec;
}

SValue Script::get_record( bool get )
{
    SToken token = get ? m_ts.next() : m_ts.current();
    string scode;
    if ( token.type() == SToken::STT_Comma ) {
        Scheme* sch = m_cals->get_ischeme();
        if ( sch ) {
            scode = sch->get_code();
        }
    } else {
        scode = get_name_or_primary( false );
    }
    FieldVec fields;
    token = m_ts.current();

    for(;;) {
        switch( token.type() )
        {
        case SToken::STT_End:
        case SToken::STT_RCbracket:
            return SValue( scode, fields );
        case SToken::STT_Comma:
            break;
        default:
            {
                SValue value = expr( false );
                Field field;
                if ( value.get( field ) ) {
                    fields.push_back( field );
                } else {
                    return value;
                }
            }
            token = m_ts.current();
            continue;
        }
        token = m_ts.next();
    }
}

SValue Script::do_subscript( const SValue& left, const SValue& right )
{
    SValue value(left);
    if( right.type() == SValue::SVT_Str ) {
        if( left.type() == SValue::SVT_Field ) {
            string scode, fname;
            SHandle sch;
            split_code( &scode, &fname, right.get_str() );
            if ( fname.empty() ) {
                fname = scode;
                sch = m_cals->get_ischeme();
            } else {
                sch = m_cals->get_scheme( scode );
            }
            Field jdn = left.get_field();
            if( sch ) {
                value.set_field( sch->jdn_fieldname_to_field( jdn, fname ) );
                return value;
            }
        }
        if( left.type() == SValue::SVT_Record ) {
            SHandle sch = m_cals->get_scheme( left.get_str() );
            int index = -1;
            if( sch ) {
                Base* base = sch->get_base();
                index = base->get_fieldname_index( right.get_str() );
            }
            if( index >= 0 ) {
                value.subscript_op( SValue(index) );
                return value;
            }
        }
        value.set_error( "Cannot determine subscript index." );
        return value;
    }
    value.subscript_op( right );
    return value;
}

SValue Script::str_cast()
{
    SToken token = m_ts.next();
    SHandle sch = nullptr;
    string sig, scode, fcode;
    if( token.type() == SToken::STT_Comma ) {
        // Includes scheme:format signiture
        sig = get_name_or_primary( true );
        split_code( &scode, &fcode, sig );
        sch = m_cals->get_scheme( scode );
    }
    SValue value = primary( false );
    RangeList rlist;
    if( value.type() == SValue::SVT_Record ) {
        SHandle r_sch = m_cals->get_scheme( value.get_record_scode() );
        rlist = m_cals->fieldvec_to_rlist( r_sch, value.get_record() );
        if( sch == nullptr ) {
            sch = r_sch;
        }
    } else {
        bool result = value.get_rlist( rlist );
        if( result == false ) {
            value.set_error( "Expected field, range, rlist or record type." );
            return value;
        }
    }
    if( sch == nullptr ) {
        sch = m_cals->get_oscheme();
        if ( sch == nullptr ) {
            value.set_error( "No default scheme set." );
            return value;
        }
    }
    value.set_str( m_cals->rangelist_to_str( sch, rlist, fcode ) );
    return value;
}

SValue Script::date_cast()
{
    SToken token = m_ts.next();
    SHandle sch = nullptr;
    string sig, scode, fcode;
    if( token.type() == SToken::STT_Comma ) {
        // Includes scheme:format signiture
        sig = get_name_or_primary( true );
        split_code( &scode, &fcode, sig );
        sch = m_cals->get_scheme( scode );
    }
    SValue value = primary( false );
    if( value.type() == SValue::SVT_Str ) {
        if( sch == nullptr ) {
            sch = m_cals->get_ischeme();
            if ( sch == nullptr ) {
                value.set_error( "No default scheme set." );
                return value;
            }
        }
        value.set( m_cals->str_to_rangelist( sch, value.get_str(), fcode ) );
    } else if( value.type() == SValue::SVT_Record ) {
        scode = value.get_record_scode();
        sch = m_cals->get_scheme( scode );
        if ( sch == nullptr ) {
            value.set_error( "No default scheme set." );
            return value;
        }
        value.set( m_cals->fieldvec_to_rlist( sch, value.get_record() ) );
    } else {
        value.set_error( "Expected string or record type." );
    }
    return value;
}

SValue Script::record_cast()
{
    SToken token = m_ts.next();
    SHandle sch = nullptr;
    string sig, scode, fcode;
    if( token.type() == SToken::STT_Comma ) {
        // Includes scheme:format signiture
        sig = get_name_or_primary( true );
        split_code( &scode, &fcode, sig );
        sch = m_cals->get_scheme( scode );
    }
    SValue value = primary( false );
    if( sch == nullptr ) {
        sch = m_cals->get_ischeme();
        if ( sch == nullptr ) {
            value.set_error( "No default scheme set." );
            return value;
        }
        scode = sch->get_code();
    }
    if( scode.empty() ) {
        value.set_error( "No scheme set." );
        return value;
    }
    FieldVec fields;
    string str;
    if( value.type() == SValue::SVT_Field ) {
        fields = m_cals->jdn_to_fieldvec( sch, value.get_field() );
    } else if( value.get( str ) ) {
        fields = m_cals->str_to_fieldvec( sch, str, fcode );
    } else {
        value.set_error( "Expected a field or string type." );
        return value;
    }
    if( fields.empty() ) {
        value.set_error( "Unable to create record." );
    } else {
        value.set_record( scode, fields );
    }
    return value;
}

SValue Script::convert_cast()
{
    SToken token = m_ts.next();
    string sig;
    if ( token.type() == SToken::STT_Comma ) {
        sig = get_name_or_primary( true );
    }
    SValue value = primary( false );
    Element ele;
    if ( !sig.empty() ) {
        ele.add_char( ':' );
        ele.add_string( sig );
    }
    if ( value.type() == SValue::SVT_Field ) {
        value.set_str( ele.get_formatted_element( m_cals, value.get_field() ) );
    } else if ( value.type() == SValue::SVT_Str ) {
        value.set_field( ele.get_converted_field( m_cals, value.get_str() ) );
    } else {
        value.set_error( "Convert requires field or string type." );
    }
    return value;
}

SValue Script::error_cast()
{
    SValue value = primary( true );
    string mess;
    if ( !value.get( mess ) ) {
        mess = "Unable to read error message.";
    }
    value.set_error( mess );
    return value;
}

SValue Script::function_call()
{
    SValue value;
    SToken token = m_ts.next();
    if ( token.type() != SToken::STT_Name ) {
        value.set_error( "Function name expected." );
        return value;
    }
    string funcode = token.get_str();
    token = m_ts.next();
    vector<SValue> args;
    if ( token.type() == SToken::STT_Lbracket ) {
        for ( ;; ) {
            SValue arg = expr( true );
            args.push_back( arg );
            token = m_ts.current();
            if ( token.type() == SToken::STT_Rbracket ||
                token.type() == SToken::STT_End )
            {
                break;
            }
            if ( token.type() != SToken::STT_Comma ) {
                value.set_error( "',' expected." );
                return value;
            }
        }
        token = m_ts.next();
    }
    Function* fun = m_cals->get_function( funcode );
    if ( fun == nullptr ) {
        value.set_error( "Function " + funcode + " not found." );
        return value;
    }

    STokenStream prev_ts = m_ts;
    m_ts.set_line( fun->get_line() );
    std::istringstream iss( fun->get_script() );
    m_ts.reset_in( &iss );
    m_cals->push_store();

    store()->set( "result", SValue() );
    for ( size_t i = 0; i < fun->get_arg_size(); i++ ) {
        if ( i < args.size() ) {
            store()->set( fun->get_arg_name( i ), args[i] );
        } else {
            store()->set( fun->get_arg_name( i ), fun->get_default_value( i ) );
        }
    }

    m_ts.next();
    while ( statement() ) {
        if ( m_ts.next().type() == SToken::STT_End ) {
            break;
        }
    }

    store()->get( &value, "result" );
    m_cals->pop_store();
    m_ts = prev_ts;

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
    if ( m_record ) { // Is it a field name.
        int index = m_record->get_field_index( name );
        if ( index >= 0 ) {
            return m_record->get_field( index );
        }
    }
    SValue value;
    if( store()->get( &value, name ) ) {
        return value;
    }
    value.set_error( "Variable \"" + name + "\" not found." );
    return value;
}


// End of src/cal/calscript.cpp file
