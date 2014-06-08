/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calgrammar.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Grammar class to control date formatting.
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

#ifndef CAL_CALGRAMMAR_H_GUARD
#define CAL_CALGRAMMAR_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class Vocab;
    class Schemes;
    class Format;

    class Grammar
    {
    public:
        Grammar( const std::string& code );
        ~Grammar();

        void add_alias( const std::string& alias_def );
        void add_vocabs( Schemes* schemes, const std::string& str );
        void add_format( const std::string& format );
        void set_inherit( Schemes* schemes, const std::string& code );

        std::string code() const { return m_code; }
        std::string get_field_alias( const std::string& fname ) const;
        std::string get_num_code_alias( const std::string& fname ) const;
        Unit get_unit_alias( const std::string& fname ) const;
        size_t order_size() const;
        StringVec get_orders() const;
        std::string get_order( size_t index ) const;
        int get_pref_order() const { return m_pref_order; }
        size_t format_size() const;
        StringVec get_formats() const;
        Format* get_format( size_t index ) const;
        std::string format( size_t index ) const;
        int get_pref_format() const { return m_pref_format; }
        StringVec get_vocab_codes() const;
        StringVec get_vocab_names() const;

        Field find_token( const std::string& word ) const;
        std::string lookup_token( Field field, const std::string& vcode, bool abbrev ) const;

        Vocab* find_vocab( const std::string& code ) const;

    private:
        std::string                m_code;
        Grammar*                   m_inherit;
        StringMap                  m_field_alias;
        StringMap                  m_num_code_alias;
        std::map<std::string,Unit> m_unit_alias;
        std::vector<Vocab*>        m_vocabs;
        std::vector<Format*>       m_formats;
        StringVec                  m_orders;
        int                        m_pref_format;
        int                        m_pref_order;
    };

}

#endif // CAL_CALGRAMMAR_H_GUARD