/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calvocab.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Vocab class to handle date name part tokens.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th November 2013
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

#ifndef CAL_CALVOCAB_H_GUARD
#define CAL_CALVOCAB_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class Token
    {
    public:
        Token( Field field, const std::string& str ) : m_field(field), m_word(str) {}
        Token() : m_field(f_invalid) {}

        Field get_field() const { return m_field; }
        std::string get_word() const { return m_word; }

    private:
        Field       m_field;
        std::string m_word;
    };

    class Vocab
    {
    public:
        Vocab( const std::string& definition );

        std::string get_code() const { return m_code; }
        std::string get_name() const { return m_name; }
        std::string get_style() const { return m_style; }
        std::string get_lang() const { return m_lang; }
        std::string get_field() const { return m_field; }
        void get_info( Vocab_info* info ) const;
        Field find( const std::string& word ) const;
        std::string lookup( Field field ) const;

    private:
        std::string  m_code;
        std::string  m_name;
        std::string  m_style;
        std::string  m_lang;
        std::string  m_field;
        std::map<std::string,Token> m_words;
        std::map<Field,Token> m_fields;
    };

}

#endif // CAL_CALVOCAB_H_GUARD