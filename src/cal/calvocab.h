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
        Token( Field field, const std::string& str, const std::string& astr = "" )
            : m_field(field), m_word(str), m_abbrev(astr) {}
        Token() : m_field(f_invalid) {}

        Field get_field() const { return m_field; }
        std::string get_word() const { return m_word; }
        std::string get_abbrev() const { return m_abbrev; }

    private:
        Field       m_field;
        std::string m_word;
        std::string m_abbrev;
    };

    class Vocab
    {
    public:
        enum Style { style_full, style_abbrev, style_ordsuffix };

        Vocab( const std::string& code );

        void set_name(const std::string& name ) { m_name = name; }
        void set_fieldname(const std::string& fname ) { m_fieldname = fname; }
        void set_lang(const std::string& lang ) { m_lang = lang; }
        void set_full_style_name(const std::string& fname ) { m_full_name = fname; }
        void set_abbrev_style_name(const std::string& aname ) { m_abbrev_name = aname; }
        void add_token( Field value, const std::string& name, const std::string& abbrev );

        std::string get_code() const { return m_code; }
        std::string get_name() const { return m_name; }
        std::string get_fieldname() const { return m_fieldname; }
        std::string get_lang() const { return m_lang; }
        std::string get_style_name( Style style ) const;
        void get_info( Vocab_info* info ) const;
        Field find( const std::string& word ) const;
        std::string lookup( Field field, Style style ) const;

    private:
        std::string  m_code;
        std::string  m_name;
		std::string  m_fieldname;
        std::string  m_lang;
        std::string  m_full_name;
        std::string  m_abbrev_name;
        std::map<std::string,Token> m_words;
        std::map<Field,Token> m_fields;
    };

}

#endif // CAL_CALVOCAB_H_GUARD