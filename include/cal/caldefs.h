/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/caldefs.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Miscellaneous data types and constants.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
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

#ifndef CAL_CALDEFS_H_GUARD
#define CAL_CALDEFS_H_GUARD

#if defined(_WIN32) || defined(__WIN32__)
  #if defined(MAKINGLIB_CAL)
    #define  DLLIMPEXP_CAL  __declspec(dllexport)
  #else
    #define  DLLIMPEXP_CAL  __declspec(dllimport)
  #endif /* MAKINGLIB_CAL */
#else  // *nix
  #define DLLIMPEXP_CAL
#endif // _WIN32

#include <string>
#include <vector>
#include <map>

namespace Cal {

    // Field must be at least 32 bit (int_fast32_t), use int for now.
    // Change to long if neccessary
    typedef int Field;
    typedef std::vector<Field> FieldVec;
    typedef std::vector<FieldVec> FieldVecVec;
    typedef std::vector<std::string> StringVec;
    typedef std::map<std::string,std::string> StringMap;
    typedef std::vector<int> XRefVec;

    const Field f_invalid  = -2147483647;
    const Field f_minimum  = -2147483646;
    const Field f_maximum  = 2147483647;
    const char range_sep = '~';    // Separates the two range values.
    const char range_div = '|';    // Used to separate ranges in range list.
    const char unknown_val = '?';  // Place holder for unknown or invalid value.

    enum Scheme_style { SCH_STYLE_Default, SCH_STYLE_Hide };

    struct Scheme_info
    {
        std::string name;
        std::string code;
        Scheme_style style;
        std::string grammar_code;
        StringVec vocab_codes;
        StringVec vocab_names;
    };

    struct Scheme_input
    {
        StringVec orders;
        int       current;
    };

    struct Scheme_output
    {
        StringVec formats;
        int       current;
    };

    struct Vocab_info
    {
        std::string name;
        std::string lang;
        std::string style_full_name;
        std::string style_abbrev_name;
        StringVec words;
        StringVec abbrevs;
    };

    struct Range
    {
        // Maintain jdn1 <= jdn2
        Field jdn1;
        Field jdn2;
    };

    typedef std::vector<Range> RangeList;

    enum Unit {
        unit_null,
        unit_year,
        unit_month,
        unit_week,
        unit_day,
        unit_max
    };

    // Normalisation flag. Determines how invalid dates are corrected.
    enum Norm {
        norm_expand,     // Add missing days to the next unit.
        norm_expand_min, // Go to the start of the next unit.
        norm_truncate,   // Truncate the current unit.
        norm_average     // Add the average number of days for the unit.
    };

    // Relative date functions
    enum Rel {
        rel_duration,
        rel_add_to_begin,
        rel_add_to_end,
        rel_age_round_down,
    };

    // Relative date info
    struct Rel_info
    {
        Rel   rel;
        Field value;
        Unit  unit;
        Norm  norm;
    };

}

#endif // CAL_CALDEFS_H_GUARD
