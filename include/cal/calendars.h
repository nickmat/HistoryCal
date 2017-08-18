/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calendars.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Main library API header
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     19th September 2013
 * Copyright:   Copyright (c) 2013 ~ 2017, Nick Matthews.
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

#ifndef CAL_CALENDARS_H_GUARD
#define CAL_CALENDARS_H_GUARD

#include "caldefs.h"

namespace Cal {

    class ScriptStore;
    class Record;
    class FormatText;
    class FormatIso;
    class FormatUnit;

    enum Init_schemes { Init_script_none, Init_script_default };

    class Calendars
    {
    public:
        DLLIMPEXP_CAL Calendars( Init_schemes = Init_script_none );
        DLLIMPEXP_CAL ~Calendars();

        DLLIMPEXP_CAL static const char* version();
        DLLIMPEXP_CAL std::string get_init_error();
        DLLIMPEXP_CAL std::string run_script( const std::string& script );
        DLLIMPEXP_CAL std::string run_script_file( const std::string& filename );
        DLLIMPEXP_CAL SHandle get_scheme( const std::string& code ) const;
        DLLIMPEXP_CAL SchemeList get_scheme_list() const;
        DLLIMPEXP_CAL void get_scheme_info( Scheme_info* info, SHandle scheme ) const; 
        DLLIMPEXP_CAL void get_input_info( SchemeFormatInfo* info, SHandle scheme ) const; 
        DLLIMPEXP_CAL void get_output_info( SchemeFormatInfo* info, SHandle scheme ) const;
        DLLIMPEXP_CAL void get_format_info( Format_info* info, SHandle scheme, const std::string& fcode ) const;
        DLLIMPEXP_CAL void get_format_text_info( FormatText_info* info, SHandle scheme, const std::string& fcode ) const;
        DLLIMPEXP_CAL bool get_vocab_info( Vocab_info* info, const std::string& code ) const; 

        DLLIMPEXP_CAL void set_input_format( SHandle scheme, const std::string& code );
        DLLIMPEXP_CAL void set_output_format( SHandle scheme, const std::string& code );

        DLLIMPEXP_CAL Field fieldvec_to_jdn( SHandle scheme, const FieldVec& fieldv );
        DLLIMPEXP_CAL FieldVec jdn_to_fieldvec( SHandle scheme, Field jdn );
        DLLIMPEXP_CAL RangeList fieldvec_to_rlist( SHandle scheme, const FieldVec& fieldv );
        DLLIMPEXP_CAL std::string fieldvec_to_str( SHandle scheme, const FieldVec& fieldv, const std::string& fcode );
        DLLIMPEXP_CAL FieldVec str_to_fieldvec( SHandle scheme, const std::string& str, const std::string& fcode = "" );

        DLLIMPEXP_CAL Field str_to_jdn( SHandle scheme, const std::string& str, const std::string& fcode = "" );
        DLLIMPEXP_CAL std::string jdn_to_str( SHandle scheme, Field jdn, const std::string& fcode = "" );
        DLLIMPEXP_CAL Range str_to_range( SHandle scheme, const std::string& str, const std::string& fcode = "" );
        DLLIMPEXP_CAL std::string range_to_str( SHandle scheme, Range range, const std::string& fcode = "" );
        DLLIMPEXP_CAL RangeList str_to_rangelist( SHandle scheme, const std::string& str, const std::string& fcode = "" );
        DLLIMPEXP_CAL RangeList expr_str_to_rangelist( SHandle scheme, const std::string& str );
        DLLIMPEXP_CAL std::string rangelist_to_str( SHandle scheme, const RangeList& ranges, const std::string& fcode = "" );

        DLLIMPEXP_CAL Field add_to_jdn( SHandle scheme, Field jdn, Field value, Unit unit, Norm norm );

        DLLIMPEXP_CAL bool str_to_rel_info( SHandle scheme, const std::string& str, Rel_info* info ) const;
        DLLIMPEXP_CAL RangeList rel_rangelist( SHandle scheme, const RangeList& ranges, Rel_info* info );

        // The following members are not part of the Cal library public api.
        bool add_scheme( SHandle sch, const std::string& code );
        Grammar* create_grammar( const std::string& code );
        Grammar* get_grammar( const std::string& code ) const;
        Vocab* create_vocab( const std::string& code );
        Vocab* get_vocab( const std::string& code ) const;
        FormatText* create_format_text( const std::string& code, Grammar* gmr );
        FormatIso* create_format_iso( const std::string& code, Grammar* gmr, const StringVec& rules );
        FormatUnit* create_format_unit( const std::string& code, Grammar* gmr );
        Function* create_function( const std::string& code );
        Function* get_function( const std::string& code ) const;
        void add_or_replace_mark( const std::string& name );
        bool clear_mark( const std::string& name );
        ScriptStore* get_store() const { return m_store; }
        void push_store();
        bool pop_store();


    private:
        SHandleMap   m_shandles;
        GrammarMap   m_grammars;
        VocabMap     m_vocabs;
        FunctionMap  m_functions;
        MarkVec      m_marks;
        ScriptStore* m_store;
        std::string  m_init_error;
    };

}

#endif // CAL_CALENDARS_H_GUARD