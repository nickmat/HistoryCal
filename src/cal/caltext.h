/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caltext.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Miscellaneous text functions header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     6th May 2015
 * Copyright:   Copyright (c) 2015, Nick Matthews.
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

#ifndef CAL_CALTEXT_H_GUARD
#define CAL_CALTEXT_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    enum StringStyle { SS_undefined, SS_lowercase, SS_uppercase };

    extern std::string field_to_str( Field field );
    extern std::string dual_fields_to_str( Field field, Field dual );
    extern std::string get_ordinal_suffix( Field field, StringStyle style = SS_lowercase );
    extern std::string get_ordinal_suffix_style( StringStyle style = SS_lowercase );
    extern std::string get_roman_numerals( Field field, StringStyle style );
    extern std::string get_roman_numerals_style( StringStyle style );

}

#endif // CAL_CALTEXT_H_GUARD