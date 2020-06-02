/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calmark.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     ScriptMark class used by the scripts mark statement.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th October 2014
 * Copyright:   Copyright (c) 2014 .. 2020, Nick Matthews.
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

#include "calmark.h"

#include "calfile.h"
#include "calfunction.h"
#include "calformat.h"
#include "calgrammar.h"
#include "calscheme.h"
#include "calvocab.h"

using namespace Cal;
using std::string;
using std::vector;

Mark::Mark( const string& name ) 
    : m_name(name), m_ischeme( nullptr ), m_oscheme( nullptr )
{
}

Mark::~Mark()
{
    for( size_t i = 0 ; i < m_schemes.size() ; i++ ) {
        delete m_schemes[i];
    }
    for( size_t i = 0 ; i < m_grammars.size() ; i++ ) {
        delete m_grammars[i];
    }
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        delete m_vocabs[i];
    }
}

void Mark::add_scheme( Scheme* scheme )
{
    m_schemes.push_back( scheme );
}

void Mark::add_grammar( Grammar* grammar )
{
    m_grammars.push_back( grammar );
}

void Mark::add_vocab( Vocab* vocab )
{
    m_vocabs.push_back( vocab );
}

void Mark::add_format( Format* format )
{
    m_formats.push_back( format );
}

string Mark::remove_next_scheme()
{
    string code;
    if( m_schemes.size() ) {
        Scheme* sch = m_schemes[m_schemes.size()-1];
        code = sch->get_code();
        delete sch;
        m_schemes.pop_back();
    }
    return code;
}

string Mark::remove_next_grammar()
{
    string code;
    if( m_grammars.size() ) {
        Grammar* gmr = m_grammars[m_grammars.size()-1];
        code = gmr->code();
        delete gmr;
        m_grammars.pop_back();
    }
    return code;
}

string Mark::remove_next_vocab()
{
    string code;
    if( m_vocabs.size() ) {
        Vocab* voc = m_vocabs[m_vocabs.size()-1];
        code = voc->get_code();
        delete voc;
        m_vocabs.pop_back();
    }
    return code;
}

string Mark::remove_next_format()
{
    string code;
    if( m_formats.size() ) {
        Format* fmt = m_formats[m_formats.size()-1];
        code = fmt->get_code();
        Grammar* gmr = fmt->get_owner();
        if( gmr ) {
            gmr->remove_format( code );
        }
        delete fmt;
        m_formats.pop_back();
    }
    return code;
}

string Mark::remove_next_function()
{
    string code;
    if ( !m_functions.empty() ) {
        Function* func = m_functions[m_functions.size() - 1];
        code = func->get_code();
        delete func;
        m_functions.pop_back();
    }
    return code;
}

string Mark::remove_next_file()
{
    string code;
    if ( m_files.size() ) {
        File* file = m_files[m_files.size() - 1];
        code = file->get_code();
        delete file;
        m_files.pop_back();
    }
    return code;
}

// End of src/cal/calmark.cpp
