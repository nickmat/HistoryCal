/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscript.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Evaluate script.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th May 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
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

#ifndef CAL_CALSCRIPT_H_GUARD
#define CAL_CALSCRIPT_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    using std::string;

    class Schemes;

    class Script
    {
    public:
        Script( Schemes* schs );

        bool run( const string& script );
        string get_output() const { return m_output; }
        RangeList get_date() const { return m_date_out; }

    private:
        enum SToken {
            ST_Null, ST_End,
            ST_Number, ST_String, ST_Name,
            ST_Equals, ST_Plus, ST_Minus, ST_Divide, ST_Star, ST_Percent, 
            ST_Backslash, ST_Semicolon, ST_Vline, ST_Ampersand, ST_Exclamation,
            ST_Carrot,
            ST_Lbracket, ST_Rbracket, ST_LCbracket, ST_RCbracket,
            ST_LSbracket, ST_RSbracket, 
            ST_date,
            ST_set, ST_evaluate, ST_write, ST_writeln,
            ST_vocab, ST_scheme, ST_grammar,
            // Alternative token names
            ST_UNION = ST_Vline,
            ST_INTERSECTION = ST_Ampersand,
            ST_REL_COMPLEMENT = ST_Backslash,
            ST_SYM_DIFFERENCE = ST_Carrot,
            ST_COMPLEMENT = ST_Exclamation
        };
        enum Mode { MODE_Normal, MODE_Date };

        void do_date();
        RangeList date_expr();
        RangeList date_value();
        string string_expr();
        string string_value();
        void do_set();
        void do_evaluate();
        SToken get_token();
        SToken look_next_token();
        RangeList get_rlist_name( const string& name ) const;
        string read_function();

        string      m_output;
        RangeList   m_date_out;
        Schemes*    m_schemes;
        Mode        m_mode;
        string::const_iterator m_it;
        string::const_iterator m_end;
        int         m_line;
        SToken      m_cur_token;
        string      m_cur_text;
        Field       m_cur_number;
        string      m_cur_name;
        RangeList   m_cur_rlist;
    };

}

#endif // CAL_CALSCRIPT_H_GUARD