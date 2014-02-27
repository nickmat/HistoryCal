/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/utf8/utf8api.cpp
 * Project:     pCalCalc: Programmable Calendric Calculator.
 * Purpose:     Wrapper for the 3rd party utf8proc library.
 * Author:      Nick Matthews
 * Website:     http://pcalcalc.org
 * Created:     13th October 2013
 * Copyright:   Copyright (c) 2013, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  pCalCalc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  pCalCalc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with pCalCalc.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/


#include "utf8/utf8api.h"

#include "utf8proc.h"
#include <cstring>
#include <cstdlib>

DLLIMPEXP_UTF8API const char* Utf8api::version()
{
    return utf8proc_version();
}

DLLIMPEXP_UTF8API std::string Utf8api::normal( const std::string& utf8 )
{
    uint8_t* retval;
    ssize_t size = utf8proc_map( (const uint8_t*) utf8.c_str(), 0, &retval,
        UTF8PROC_NULLTERM | UTF8PROC_STABLE | UTF8PROC_COMPOSE | 
        UTF8PROC_COMPAT | UTF8PROC_CASEFOLD
    );
    // TODO: Do some checks on size and throw errors
    std::string str( (char*) retval );
    free( retval );
    return str;
}

// End of utf8api.cpp source file.
