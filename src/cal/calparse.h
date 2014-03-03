/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calparse.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Functions to help parse scripts.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
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

#ifndef CAL_CALPARSE_H_GUARD
#define CAL_CALPARSE_H_GUARD

#include "cal/caldefs.h"

#include <string>
#include <vector>

namespace Cal {

    StringVec parse_statements( const std::string& str );
    std::string left_trim( const std::string& str );
    std::string right_trim( const std::string& str );
    std::string full_trim( const std::string& str );
    std::string get_first_word( const std::string& str, std::string* tail = NULL, char sep = ' ' );
    std::string get_next_phrase( const std::string& str, std::string* tail = NULL, char sep = ' ' );
    std::string peel_cbrackets( const std::string& str );
    std::string field_to_str( Field field );
    Field str_to_field( const std::string& str );

}

#endif // CAL_CALPARSE_H_GUARD
