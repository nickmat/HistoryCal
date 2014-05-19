/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calschemes.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Schemes (scheme list) class.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
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

#include "calschemes.h"

#include "calgrammar.h"
#include "calparse.h"
#include "calscheme.h"
#include "calscript.h"
#include "calvocab.h"


using namespace std;
using namespace Cal;

Schemes::~Schemes()
{
    for( size_t i = 0 ; i < m_scheme_vec.size() ; i++ ) {
        delete m_scheme_vec[i];
    }
    for( GrammarMap::iterator it = m_grammars.begin() ; it != m_grammars.end() ; it++ ) {
        delete it->second;
    }
    for( VocabMap::iterator it = m_vocabs.begin() ; it != m_vocabs.end() ; it++ ) {
        delete it->second;
    }
}

string Schemes::read_script( const string& script )
{
    Script scr( this );
    if( scr.run( script ) ) {
        return scr.get_output();
    }
    return "";
}

RangeList Schemes::expr_str_to_rangelist( int scheme_id, const std::string& str )
{
    RangeList rlist;
    Scheme* sch = get_scheme( scheme_id );
    if( sch == NULL ) {
        return rlist;
    }
    string script = "set input \"" + sch->get_code() + "\";\n"
        + "evaluate date " + parse_date_expr( str ) + ";";

    Script scr( this );
    if( scr.run( script ) ) {
        return scr.get_date();
    }
    return rlist;
}

void Schemes::add_scheme( const string& definition )
{
    Scheme* sch = new Scheme( this, definition );
    if( sch->is_ok() == false ) {
        delete sch;
        return;
    }
    string code = sch->get_code();
    int sch_id = m_scheme_vec.size();
    m_scheme_vec.push_back( sch );
    m_scheme_ids[code] = sch_id;
}

void Schemes::add_vocab( const string& definition )
{
    Vocab* voc = new Vocab( definition );
    string code = voc->get_code();
    m_vocabs[code] = voc;
}

void Schemes::add_grammar( const string& definition )
{
    Grammar* gmr = new Grammar( this, definition );
    string code = gmr->code();
    m_grammars[code] = gmr;
}

Scheme* Schemes::get_scheme( int scheme_id ) const
{
    if( scheme_id < 0 || scheme_id >= (int) m_scheme_vec.size() ) {
        return NULL;
    }
    return m_scheme_vec[scheme_id];
}

Scheme* Schemes::get_scheme( const string& code ) const
{
    int index = get_scheme_id( code );
    if( index < 0 ) {
        return NULL;
    }
    return m_scheme_vec[index];
}

int Schemes::get_scheme_id( const string& code ) const
{
    if( m_scheme_ids.count( code ) == 0 ) {
        return -1;
    }
    return m_scheme_ids.find( code )->second;
}

Grammar* Schemes::get_grammar( const string& code ) const
{
    if( grammar_exist( code ) ) {
        return m_grammars.find( code )->second;
    }
    return NULL;
}

Vocab* Schemes::get_vocab( const string& code ) const
{
    if( vocab_exist( code ) ) {
        return m_vocabs.find( code )->second;
    }
    return NULL;
}

bool Schemes::grammar_exist( const string& code ) const
{
    return m_grammars.count( code ) > 0;
}

bool Schemes::vocab_exist( const string& code ) const
{
    return m_vocabs.count( code ) > 0;
}


// End of src/cal/calschemes.cpp file
