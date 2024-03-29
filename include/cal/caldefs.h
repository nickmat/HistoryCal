/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/caldefs.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Miscellaneous data types and constants.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
 * Copyright:   Copyright (c) 2013 ~ 2020, Nick Matthews.
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

#if defined(_WINDLL) && ( defined(_WIN32) || defined(__WIN32__) )
  #if defined(MAKINGLIB_CAL) || defined(cal_EXPORTS)
    #define  DLLIMPEXP_CAL  __declspec(dllexport)
  #else
    #define  DLLIMPEXP_CAL  __declspec(dllimport)
  #endif /* MAKINGLIB_CAL */
#else  // *nix or static
  #define DLLIMPEXP_CAL
#endif // _WIN32

#include <climits>
#include <map>
#include <string>
#include <vector>

namespace Cal {


    class Scheme;
    typedef Scheme* SHandle;
    typedef std::map<std::string,SHandle> SHandleMap;

    struct SchemeData
    {
        SchemeData() : handle(nullptr), has_in_format( false ), has_out_format( false ) {}

        std::string code;
        std::string name;
        SHandle     handle;
        bool        has_in_format;
        bool        has_out_format;
    };
    typedef std::vector<SchemeData> SchemeList;

    class Grammar;
    typedef std::map<std::string,Grammar*> GrammarMap;
    class Vocab;
    typedef std::map<std::string,Vocab*> VocabMap;
    class Format;
    typedef std::map<std::string,Format*> FormatMap;
    class Mark;
    typedef std::vector<Mark*> MarkVec;
    class Function;
    typedef std::map<std::string,Function*> FunctionMap;
    class File;
    typedef std::map<std::string, File*> FileMap;

    // Field must be at least 32 bit, use int for now.
    // Change to int_fast32_t if possible
    typedef int Field;
    // Used for intermediate values during calculations.
    // Change to int_fast64_t if possible
    typedef long long LongField;

    typedef std::vector<Field> FieldVec;
    typedef std::vector<FieldVec> FieldVecVec;
    typedef std::vector<std::string> StringVec;
    typedef std::map<std::string,std::string> StringMap;
    typedef std::vector<int> XRefVec;
    typedef std::vector<bool> BoolVec;

    inline void stringvec_cat( StringVec& a, const StringVec& b ) {
        a.insert( a.end(), b.begin(), b.end() );
    }

    const Field f_invalid  = INT_MIN;      // -2147483648;
    const Field f_minimum  = -(INT_MAX-2); // -2147483645;
    const Field f_maximum  = INT_MAX - 2;  //  2147483645;
    const Field f_end      = INT_MAX - 1;  //  2147483646;
    const Field f_invalid2 = INT_MAX;      //  2147483647;
    const char spec_sep = '#';     // Separates date specification from date string.
    const char range_sep = '~';    // Separates the two range values.
    const char range_div = '|';    // Used to separate ranges in range list.
    const char unknown_val = '?';  // Place holder for unknown or invalid value.

    struct Range
    {
        Range() {}
        Range( Field f1, Field f2 ) : jdn1(f1), jdn2(f2) {}
        // Maintain jdn1 <= jdn2
        Field jdn1;
        Field jdn2;
    };

    typedef std::vector<Range> RangeList;

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

    enum Format_style { FMT_STYLE_Default, FMT_STYLE_Hide };

    struct PCode {
        std::string code;
        int priority;
    };

    struct PDesc {
        std::string desc;
        std::vector<PCode> codes;
    };

    struct SchemeFormatInfo {
        std::vector<PDesc> descs;
        size_t current;
    };

    enum FormatType { FT_null, FT_text, FT_unit, FT_iso };

    struct Format_info {
        Format_info() : type(FT_null), priority(0), style(FMT_STYLE_Default) {}

        std::string  code;
        FormatType   type;
        std::string  gmr_code;
        std::string  input_str;
        std::string  output_str;
        int          priority;
        Format_style style;
    };

    struct FormatText_info {
        Format_info info;
        std::string control_str;
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
