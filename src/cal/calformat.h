/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calformat.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Format class to control the formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th November 2013
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

#ifndef CAL_CALFORMAT_H_GUARD
#define CAL_CALFORMAT_H_GUARD

#include "calprivate.h"

namespace Cal {

    class Grammar;
    class Vocab;

    class Format {
    public:
        Format( const Grammar* gmr, const std::string& format );
        ~Format();

        std::string get_format() const { return m_format; }
        // Get a user friendly format string for user selection
        std::string get_user_format() const { return m_output_str; }
        std::string get_order_str() const { return m_input_str; }

        std::string get_output_field( Vocab* vocab ) const;
        std::string get_1st_output_field( InputFieldType type ) const;

        StringVec get_output_fields() const { return m_output_fields; }
        std::vector<Vocab*> get_vocabs() const { return m_vocabs; }

    private:
        std::string m_format;
        std::string m_input_str;
        std::string m_output_str;
        StringVec   m_output_fields;
        std::vector<Vocab*> m_vocabs;
        std::vector<InputFieldType> m_types;
    };

}

#endif // CAL_CALFORMAT_H_GUARD