/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformat.h
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

    class Format
    {
    public:
        Format( const std::string& code, Grammar* gmr ) : m_code(code), m_owner(gmr) {}
        virtual ~Format() {}

        void set_user_input_str( const std::string str ) { m_input_str = str; }
        void set_user_output_str( const std::string str ) { m_output_str = str; }

        std::string get_code() const { return m_code; }
        Grammar* get_owner() const { return m_owner; }
        std::string get_user_input_str() const { return m_input_str; }
        std::string get_user_output_str() const { return m_output_str; }

        virtual std::string rlist_to_string( Base* base, const RangeList& ranges ) const;
        virtual std::string range_to_string( Base* base, Range range ) const;
        virtual std::string jdn_to_string( Base* base, Field jdn ) const;
        virtual std::string get_output( const Record& record ) const = 0;

        virtual RangeList string_to_rlist( Base* base, const std::string& input ) const = 0;
        virtual bool set_input( Record* record, const std::string& input, Boundary rb ) const = 0;

    private:
        std::string m_code;
        Grammar*    m_owner;
        std::string m_input_str;
        std::string m_output_str;
    };

}

#endif // CAL_CALFORMAT_H_GUARD