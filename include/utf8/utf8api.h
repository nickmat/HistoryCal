/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/utf8/utf8api.h
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


#ifndef UTF8API_H_GUARD
#define UTF8API_H_GUARD

#if defined(_WIN32) || defined(__WIN32__)
  #if defined(MAKINGLIB_UTF8API) || defined(utf8api_EXPORTS)
    #define  DLLIMPEXP_UTF8API  __declspec(dllexport)
  #else
    #define  DLLIMPEXP_UTF8API  __declspec(dllimport)
  #endif // MAKINGLIB_UTF8API
#else  // *nix
  #define DLLIMPEXP_UTF8API
#endif

#include <string>

namespace Utf8api {

    DLLIMPEXP_UTF8API const char* version();
    DLLIMPEXP_UTF8API std::string normal( const std::string& utf8 );

}

#endif // UTF8API_H_GUARD
