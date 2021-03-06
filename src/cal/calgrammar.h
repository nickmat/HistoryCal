/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calgrammar.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Grammar class to control date formatting.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th November 2013
 * Copyright:   Copyright (c) 2013 ~ 2020, Nick Matthews.
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

#include "calprivate.h"

namespace Cal {

    class Base;
    class Calendars;
    class Record;
    class FormatText;
    class FormatIso;
    class FormatUnit;

    struct ElementField
    {
        std::string in_expression;
        std::string out_expression;
        std::string pseudo;
        std::string alias;
    };

    class Grammar
    {
    public:
        Grammar( const std::string& code, Calendars* cals );
        ~Grammar();

        bool constuct( const Base* base );
        void create_def_format();
        void create_u_format();
        bool is_ok() const { return m_ok; }

        void set_inherit( const std::string& code );
        void set_pref( const std::string& fcode );
        void add_vocab( Vocab* vocab );
        FormatText* create_format_text( const std::string& code );
        FormatIso* create_format_iso( const std::string& code, const StringVec& rules );
        FormatUnit* create_format_unit( const std::string& code );
        bool add_format( Format* fmt );
        void add_element( const std::string& elem, const ElementField& ef );
        void add_alias( const std::string& alias, const StringVec& pairs );

        std::string code() const { return m_code; }
        std::string get_field_alias( const std::string& fname ) const;
        std::string get_unit_fieldname( Field* multiple, const std::string& unit ) const;
        std::string get_unitname( const std::string& fieldname ) const;
        std::string get_num_pseudo_alias( const std::string& fname ) const;
        std::string get_element_pseudo_name( const std::string& fname ) const;
        Unit get_unit_alias( const std::string& fname ) const;
        std::string get_input_format( const std::string& code ) const;
        bool has_input_format() const;
        void get_input_formats( SchemeFormatInfo* info, const std::string& cur_code ) const;
        bool has_output_format() const;
        void get_output_formats( SchemeFormatInfo* info, const std::string& cur_code ) const;
        Format* get_format( const std::string& code ) const;
        std::string get_pref_input_fcode() const { return m_pref_input_fcode; }
        std::string get_pref_output_fcode() const { return m_pref_output_fcode; }
        StringVec get_vocab_codes() const;
        StringVec get_vocab_names() const;
        ElementField* get_calc_field( const std::string& name );
        bool get_element(
            Field* field, const Record& record, const std::string& fname, const BoolVec* reveal ) const;
        bool set_element( Record* record, const std::string& fname, Field field ) const;
        Calendars* get_calendars() const { return m_cals; }

        Field find_token( Vocab** vocab, const std::string& word ) const;
        std::string lookup_token( Field field, const std::string& vcode, bool abbrev ) const;

        Vocab* find_vocab( const std::string& code ) const;

        void remove_format( const std::string& fcode );

        void set_base_fieldnames( StringVec fieldnames ) { m_base_fieldnames = fieldnames; }
        StringVec get_base_fieldnames() const { return m_base_fieldnames; }
        void set_opt_fieldnames( StringVec fieldnames ) { m_opt_fieldnames = fieldnames; }
        StringVec get_opt_fieldnames() const { return m_opt_fieldnames; }
        void set_rank_fieldnames( StringVec fieldnames ) { m_rank_fieldnames = fieldnames; }
        StringVec get_rank_fieldnames() const{ return m_rank_fieldnames; }
        size_t get_sig_rank_size() const;
        int get_rank_field_index( const std::string& fieldname ) const;
        StringVec get_record_fieldnames() const { return m_record_fieldnames; }

        int next_format_priority() { return -int( m_formats.size() + 1 ); }

    private:
        enum INFO { INPUT_INFO, OUTPUT_INFO };
        bool has_format( INFO type ) const;
        void get_format_info( SchemeFormatInfo* info, const std::string& cur_code, INFO type ) const;

        Calendars*                 m_cals;
        std::string                m_code;
        bool                       m_ok;
        Grammar*                   m_inherit;
        std::map<std::string, ElementField> m_elements;
        StringMap                  m_field_alias;
        StringMap                  m_num_pseudo_alias;
        StringMap                  m_unit_alias;
        std::map<std::string,Unit> m_unit_type_alias;
        std::vector<Vocab*>        m_vocabs;
        FormatMap                  m_formats;
        std::string                m_pref_input_fcode;
        std::string                m_pref_output_fcode;
        StringVec                  m_base_fieldnames;
        StringVec                  m_opt_fieldnames;
        StringVec                  m_rank_fieldnames;
        size_t                     m_sig_rank_size;
        StringVec                  m_record_fieldnames;
    };

}

#endif // CAL_CALGRAMMAR_H_GUARD