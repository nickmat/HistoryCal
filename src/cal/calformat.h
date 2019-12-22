/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformat.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Format class to control the formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th November 2013
 * Copyright:   Copyright (c) 2013 ~ 2019, Nick Matthews.
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
#include "calgrammar.h"

namespace Cal {

    class Base;
    class Record;
    class Calendars;

    class Format
    {
    public:
        Format( const std::string& code, Grammar* gmr );
        virtual ~Format();

        void set_priority( int priority ) { m_priority = priority; }
        void set_user_input_str( const std::string str ) { m_input_str = str; }
        void set_user_output_str( const std::string str ) { m_output_str = str; }
        void set_style( Format_style style ) { m_style = style; }

        std::string get_code() const { return m_code; }
        Grammar* get_owner() const { return m_owner; }
        Calendars* get_calenders() const { return m_owner->get_calendars(); }
        int get_priority() const { return m_priority; }
        std::string get_user_input_str() const { return m_input_str; }
        std::string get_user_output_str() const { return m_output_str; }
        Format_style get_style() const { return m_style; }
        void get_info( Format_info* info ) const;

        virtual FormatType get_format_type() const = 0;
        virtual std::string rlist_to_string( Base* base, const RangeList& ranges ) const;
        virtual std::string range_to_string( Base* base, Range range ) const;
        virtual std::string jdn_to_string( Base* base, Field jdn ) const;
        virtual std::string get_output( const Record& record ) const = 0;
        virtual std::string get_control_str() const { return ""; }

        virtual RangeList string_to_rlist( Base* base, const std::string& input ) const = 0;
        virtual bool set_input( Record* record, const std::string& input, Boundary rb ) const = 0;

    private:
        std::string  m_code;
        Grammar*     m_owner;
        // The priority value is used when the format in/out descriptor string
        // is the same for more than one format.
        // If this string is used to select a format (ie from a pick list)
        // then the format with the highest priority is used.
        int          m_priority;
        std::string  m_input_str;
        std::string  m_output_str;
        Format_style m_style;
    };

}

#endif // CAL_CALFORMAT_H_GUARD