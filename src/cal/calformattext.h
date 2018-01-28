/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformattext.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     FormatText class to control formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st March 2016
 * Copyright:   Copyright (c) 2016 ~ 2018, Nick Matthews.
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

#ifndef CAL_CALFORMATTEXT_H_GUARD
#define CAL_CALFORMATTEXT_H_GUARD

#include "calformat.h"

namespace Cal {

    class FormatText : public Format
    {
    public:
        enum Use { Use_inout, Use_output, Use_input, Use_strict };

        FormatText( const std::string& code, Grammar* gmr );

        FormatType get_format_type() const override { return FT_text; };
        std::string range_to_string( Base* base, Range range ) const override;
        std::string get_output( const Record& record ) const override { return get_masked_output( record, nullptr ); }
        std::string get_masked_output( const Record& record, const BoolVec* mask ) const;
        RangeList string_to_rlist( Base* base, const std::string& input ) const override;
        bool set_input( Record* record, const std::string& input, Boundary rb ) const override;

        bool resolve_input(
            const Base* base, Field* fields, const InputFieldVec& input ) const;

        void set_control( const std::string& format, Use usefor = Use_inout );
        void set_separators( const std::string& sep ) { m_separators = sep; }
        std::string get_control() const { return m_control; }

        StringVec get_input_fields() const { return m_input_fields; }

        std::string get_input_field( Vocab* vocab ) const;
        std::string get_1st_input_field( InputFieldType type ) const;

        void set_rank_fieldnames( StringVec fieldnames ) { m_rank_fieldnames = fieldnames; }
        void set_rankout_fieldnames( StringVec fieldnames ) { m_rankout_fieldnames = fieldnames; }
        StringVec get_rank_fieldnames() const;
        StringVec get_rankout_fieldnames() const;
        bool is_tier1( const std::string& fieldname ) const;

    private:
        enum CP_Group {
            GRP_Hyphen, GRP_Digit, GRP_Quest, GRP_Dual,
            GRP_Sep, GRP_Other
        };
        CP_Group get_cp_group( 
            std::string::const_iterator it,
            std::string::const_iterator end ) const;
        Field get_field( const Record& record, const std::string& fname, const BoolVec* mask ) const;
        std::string formatted_str(
            Field field, const std::string& format, const std::string& specifier ) const;
        int parse_date( InputField* ifs, size_t size, const std::string& str ) const;

        RangeList multirange_str_to_rlist( Base* base, const std::string& input ) const;
        RangeList bare_str_to_rlist( Base* base, const std::string& input ) const;

        std::string m_control;
        std::string m_separators;

        std::vector<Vocab*> m_vocabs;
        std::vector<InputFieldType> m_types;
        StringVec   m_input_fields;

        StringVec   m_default_names;
        FieldVec    m_default_values;

        StringVec   m_rank_fieldnames;
        StringVec   m_rankout_fieldnames;
    };

}

#endif // CAL_CALFORMATTEXT_H_GUARD