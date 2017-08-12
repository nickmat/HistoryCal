/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformatiso.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     FormatIso class for ISO 8601 input and output.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st March 2016
 * Copyright:   Copyright (c) 2016 ~ 2017, Nick Matthews.
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

#ifndef CAL_CALFORMATISO_H_GUARD
#define CAL_CALFORMATISO_H_GUARD

#include "calformat.h"

namespace Cal {

    class FormatIso : public Format
    {
    public:
        enum DateRep { DR_null, DR_gregorian, DR_week, DR_ordinal };

        FormatIso( const std::string& code, Grammar* gmr, const StringVec& rules );

        FormatType get_format_type() const override { return FT_iso; };
        std::string rlist_to_string( Base* base, const RangeList& ranges ) const override;
        std::string range_to_string( Base* base, Range range ) const override;
        std::string jdn_to_string( Base* base, Field jdn ) const override;
        std::string get_output( const Record& record ) const override { return get_masked_output( record, nullptr ); }
        std::string get_masked_output( const Record& record, const BoolVec* mask ) const;
        RangeList string_to_rlist( Base* base, const std::string& input ) const override;
        bool set_input( Record* record, const std::string& input, Boundary rb ) const override;

    private:
        std::string output_year( Field year ) const;

        DateRep recurring_interval( Field* fields, const std::string& input, Boundary rb ) const;
        DateRep interval( Field* fields, const std::string& input, Boundary rb ) const;
        DateRep datetime( Field* fields, const std::string& input, Boundary rb ) const;
        DateRep weekdate( Field* fields, const std::string& str, Boundary rb ) const;

        RangeList string_set_to_rlist( Base* base, const std::string& input ) const;

        DateRep  m_daterep;
        bool     m_extended;
        bool     m_yplusminus;
        bool     m_yminus; // Ignored if m_plusminus is true
        bool     m_dateset;
        size_t   m_ydigits;
    };

}

#endif // CAL_CALFORMATISO_H_GUARD