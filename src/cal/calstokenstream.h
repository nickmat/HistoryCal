/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calstokenstream.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Translate a character stream to a token stream.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     24th June 2014
 * Copyright:   Copyright (c) 2014 - 2015, Nick Matthews.
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

#ifndef CAL_CALSTOKENSTREAM_H_GUARD
#define CAL_CALSTOKENSTREAM_H_GUARD

#include "cal/caldefs.h"
#include "calscriptstore.h"

#include <iostream>

namespace Cal {

    class SToken
    {
    public:
        enum Type {
            STT_Null, STT_End, 
            STT_String, STT_Bool,
            STT_Name, STT_Number, STT_Range, STT_RList,
            STT_Equal, STT_Plus, STT_Minus, STT_Divide, STT_Star,
            STT_PlusEq, STT_MinusEq, STT_DivideEq, STT_StarEq,
            STT_Percent, STT_Backslash, STT_Tilde,
            STT_Comma, STT_Colon, STT_Semicolon, STT_Qmark,
            STT_Vline, STT_Ampersand, STT_Exclamation, STT_Carrot,
            STT_Lbracket, STT_Rbracket, STT_LCbracket, STT_RCbracket,
            STT_LSbracket, STT_RSbracket, 
            STT_and, STT_or, STT_not, STT_match,
            STT_str_cast, STT_date, STT_mask,
            STT_NotEqual,    //  "<>"
            STT_GtThan,      //  ">"
            STT_GtThanEq,    //  ">="
            STT_LessThan,    //  "<"
            STT_LessThanEq,  //  "<="
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
        void set_value( bool b ) { m_value.set_bool( b ); }

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

        SToken next();
        SToken& current() { return m_cur_token; }
        std::string read_until( const std::string& name, const std::string& esc );

        bool error( const std::string& mess );
        int errors() const { return m_errors; }

        int get_line() const { return m_line; }
        void set_line( int line ) { m_line = line; }
        std::istream* reset_in( std::istream* in );

    private:
        void set_type( SToken::Type type ) { m_cur_token.set_type( type ); } 
        void set_current( SToken::Type type, const std::string& str );
        void set_current( SToken::Type type, Field num );
        void set_current( SToken::Type type, bool b );

        std::istream* m_in;
        std::ostream* m_err;
        SToken   m_cur_token;
        int      m_line;
        int      m_errors;
    };

}

#endif // CAL_CALSTOKENSTREAM_H_GUARD