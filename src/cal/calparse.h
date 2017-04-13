/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calparse.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Functions to help parse scripts.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
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

#ifndef CAL_CALPARSE_H_GUARD
#define CAL_CALPARSE_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    std::string left_trim( const std::string& str );
    std::string right_trim( const std::string& str );
    std::string full_trim( const std::string& str );
    std::string get_first_word( const std::string& str, std::string* tail = NULL, char sep = ' ' );
    Field str_to_field( const std::string& str );
    Field str_to_dual2( Field dual1, const std::string& str );
    double str_to_double( const std::string& str );
    std::string make_key( const std::string& str );
    bool split_code( std::string* scheme, std::string* format, const std::string& str );
    std::string parse_date_expr( const std::string& str );

    inline bool u8_isspace( int ch ) { return ( ch > 0 && isspace( ch ) ); }
    inline bool u8_isdigit( int ch ) { return ( ch > 0 && isdigit( ch ) ); }
}

#endif // CAL_CALPARSE_H_GUARD
