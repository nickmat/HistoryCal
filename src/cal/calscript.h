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
#include "calscriptstore.h"

#include <iostream>

namespace Cal {

    class SToken
    {
    public:
        enum Type {
            STT_Null, STT_End, 
            STT_String,
            STT_Name, STT_Number, STT_Range, STT_RList,
            STT_Equal, STT_Plus, STT_Minus, STT_Divide, STT_Star,
            STT_Percent, STT_Backslash, STT_Semicolon, STT_Tilde,
            STT_Vline, STT_Ampersand, STT_Exclamation, STT_Carrot,
            STT_Lbracket, STT_Rbracket, STT_LCbracket, STT_RCbracket,
            STT_LSbracket, STT_RSbracket, 
            STT_and, STT_or, STT_not, STT_str_cast, STT_date,
            STT_NotEqual/*<>*/,
            STT_GtThan, STT_GtThanEq, STT_LessThan, STT_LessThanEq, 
            // Alternative token names
            STT_UNION = STT_Vline,
            STT_INTERSECTION = STT_Ampersand,
            STT_REL_COMPLEMENT = STT_Backslash,
            STT_SYM_DIFFERENCE = STT_Carrot,
            STT_COMPLEMENT = STT_Exclamation
        };
        SToken() : m_type(STT_End) {}

        void set_type( Type type ) { m_type = type; }
        void set_value( const std::string& str ) { m_value.set_str( str ); }
        void set_value( Field field ) { m_value.set_field( field ); }

        Type type() const { return m_type; }
        SValue value() const { return m_value; }
        std::string get_str() const { return m_value.get_str(); }
        bool get_bool() const { return m_value.get_bool(); }
        Field get_field() const { return m_value.get_field(); }
        Range get_range() const { return m_value.get_range(); }
        RangeList get_rlist() const { return m_value.get_rlist(); }

    private:
        Type   m_type;
        SValue m_value;
    };

    class STokenStream {
    public:
        STokenStream( std::istream& in, std::ostream& err ) 
            : m_in(&in), m_err(&err), m_line(1), m_errors(0) {}

        void reset();

        SToken next();
        SToken& current() { return m_cur_token; }
        std::string read_function();

        bool error( const std::string& mess );
        int errors() const { return m_errors; }

    private:
        void set_type( SToken::Type type ) { m_cur_token.set_type( type ); } 
        void set_current( SToken::Type type, const std::string& str );
        void set_current( SToken::Type type, Field num );

        std::istream* m_in;
        std::ostream* m_err;
        SToken   m_cur_token;
        int      m_line;
        int      m_errors;
    };

    class Calendars;

    class Script
    {
    public:
        Script( Calendars* cals, std::istream& in, std::ostream& out );

        bool run();

    private:
        bool error( const std::string& mess ) { return m_ts.error( mess ); }
        ScriptStore* store() const;
        bool statement();
        bool do_set();
        bool do_rlist();
        bool do_let();
        bool do_write();
        bool do_writeln();
        bool do_scheme();
        bool do_vocab();
        bool do_grammar();

        SValue expr( bool get );
        SValue compare( bool get );
        SValue sum( bool get );
        SValue term( bool get );
        SValue combine( bool get );
        SValue range( bool get );
        SValue primary( bool get );

        SValue get_value_var( const std::string& name );

        Calendars*    m_cals;
        STokenStream  m_ts;
        std::ostream* m_out;
        std::ostream* m_err;
    };

}

#endif // CAL_CALSCRIPT_H_GUARD