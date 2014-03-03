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
        Grammar( Schemes* schemes, const std::string& definition );
        ~Grammar();

        void add_alias( const std::string& alias_def );
        void add_order( const std::string& order );
        void add_vocabs( Schemes* schemes, const std::string& str );
        void add_format( const std::string& format );

        std::string code() const { return m_code; }
        std::string get_field_alias( const std::string& fname ) const;
        std::string get_num_code_alias( const std::string& fname ) const;
        Unit get_unit_alias( const std::string& fname ) const;
        size_t order_size() const { return m_orders.size(); }
        StringVec* get_orders() { return &m_orders; }
        std::string order( size_t index ) const { return m_orders[index]; }
        int get_pref_order() const { return m_pref_order; }
        size_t format_size() const { return m_formats.size(); }
        StringVec get_formats() const;
        Format* get_format( size_t index ) const { return m_formats[index]; }
        std::string format( size_t index ) const;
        int get_pref_format() const { return m_pref_format; }
        StringVec get_vocab_codes() const;
        StringVec get_vocab_names() const;

        Field find_token( const std::string& word ) const;
        std::string lookup_token( Field field, const std::string& vcode, bool abbrev ) const;

        Vocab* find_vocab( const std::string& code ) const;

    private:
        std::string create_order_str( const std::string& str ) const;

        std::string                m_code;
        StringMap                  m_field_alias;
        StringMap                  m_num_code_alias;
        std::map<std::string,Unit> m_unit_alias;
        StringVec                  m_orders;
        std::vector<Vocab*>        m_vocabs;
        std::vector<Format*>       m_formats;
        int                        m_pref_order;
        int                        m_pref_format;
    };

}

#endif // CAL_CALGRAMMAR_H_GUARD