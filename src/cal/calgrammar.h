/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calgrammar.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Grammar class to control date formatting.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th November 2013
 * Copyright:   Copyright (c) 2013 - 2015, Nick Matthews.
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

#ifndef CAL_CALGRAMMAR_H_GUARD
#define CAL_CALGRAMMAR_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class Calendars;

    class Grammar
    {
    public:
        Grammar( const std::string& code );
        ~Grammar();

        void set_inherit( Calendars* cals, const std::string& code );
        void set_pref( const std::string& fcode );
        void add_vocab( Vocab* vocab );
        Format* create_format( const std::string& code );
        bool add_format( Format* fmt );
        void add_alias( const std::string& alias, const StringVec& pairs );

        std::string code() const { return m_code; }
        std::string get_field_alias( const std::string& fname ) const;
        std::string get_num_code_alias( const std::string& fname ) const;
        Unit get_unit_alias( const std::string& fname ) const;
        std::string get_input_format( const std::string& code ) const;
        void get_input_formats( SchemeFormats* input, const std::string& cur_code ) const;
        void get_output_formats( SchemeFormats* output, const std::string& cur_code ) const;
        Format* get_format( const std::string& code ) const;
        std::string get_pref_input_fcode() const { return m_pref_input_fcode; }
        std::string get_pref_output_fcode() const { return m_pref_output_fcode; }
        StringVec get_vocab_codes() const;
        StringVec get_vocab_names() const;

        Field find_token( Vocab** vocab, const std::string& word ) const;
        std::string lookup_token( Field field, const std::string& vcode, bool abbrev ) const;

        Vocab* find_vocab( const std::string& code ) const;

        void remove_format( const std::string& fcode );

    private:
        std::string                m_code;
        Grammar*                   m_inherit;
        StringMap                  m_field_alias;
        StringMap                  m_num_code_alias;
        std::map<std::string,Unit> m_unit_alias;
        std::vector<Vocab*>        m_vocabs;
        FormatMap                  m_formats;
        std::string                m_pref_input_fcode;
        std::string                m_pref_output_fcode;
    };

}

#endif // CAL_CALGRAMMAR_H_GUARD