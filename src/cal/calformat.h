/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calformat.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Format class to control the formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th November 2013
 * Copyright:   Copyright (c) 2013 ~ 2016, Nick Matthews.
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

#ifndef CAL_CALFORMAT_H_GUARD
#define CAL_CALFORMAT_H_GUARD

#include "calprivate.h"

namespace Cal {

    class Base;
    class Record;

    class Format {
    public:
        enum Use { Use_inout, Use_output, Use_input, Use_strict };

        Format( const std::string& code, Grammar* gmr );
        ~Format();

        void set_user_input_str( const std::string str ) { m_input_str = str; }
        void set_user_output_str( const std::string str ) { m_output_str = str; }

        std::string get_code() const { return m_code; }
        Grammar* get_owner() const { return m_owner; }
        std::string get_user_input_str() const { return m_input_str; }
        std::string get_user_output_str() const { return m_output_str; }

        std::string rlist_to_string( Base* base, const RangeList& ranges ) const;
        std::string range_to_string( Base* base, Range range ) const;
        std::string jdn_to_string( Base* base, Field jdn ) const;
        std::string get_output( const Record& record ) const;

        RangeList string_to_rlist( Base* base, const std::string& input ) const;
        bool set_input( Record* record, const std::string& input, Boundary rb ) const;

        bool resolve_input(
            const Base* base, Field* fields, const InputFieldVec& input ) const;

        void set_format( const std::string& format, Use usefor = Use_inout );
        void set_separators( const std::string& sep ) { m_separators = sep; }

        std::string get_format() const { return m_format; }
        std::string get_separators() const { return m_separators; }

        StringVec get_input_fields() const { return m_input_fields; }

        std::string get_input_field( Vocab* vocab ) const;
        std::string get_1st_input_field( InputFieldType type ) const;

        void set_rank_fieldnames( StringVec fieldnames ) { m_rank_fieldnames = fieldnames; }
        StringVec get_rank_fieldnames() const;
        bool is_tier1( const std::string& fieldname ) const;

    private:
        enum CP_Group {
            GRP_Hyphen, GRP_Digit, GRP_Quest, GRP_Dual,
            GRP_Sep, GRP_Other
        };
        CP_Group get_cp_group( 
            std::string::const_iterator it,
            std::string::const_iterator end ) const;
        Field get_field( const Record& record, const std::string& fname ) const;
        std::string formatted_str( 
            Field field, const std::string& format, const std::string& specifier ) const;
        int parse_date( InputField* ifs, size_t size, const std::string& str ) const;

        RangeList multirange_str_to_rlist( Base* base, const std::string& input ) const;
        RangeList bare_str_to_rlist( Base* base, const std::string& input ) const;
        bool set_range_as_begin( Range* range, const Record& mask ) const;
        bool set_range_as_next( Range* range, const Record& mask ) const;

        std::string m_code;
        Grammar*    m_owner;
        std::string m_input_str;
        std::string m_output_str;

        std::string m_format;
        std::string m_separators;

        std::vector<Vocab*> m_vocabs;
        std::vector<InputFieldType> m_types;
        StringVec   m_input_fields;

        StringVec   m_rank_fieldnames;
    };

}

#endif // CAL_CALFORMAT_H_GUARD