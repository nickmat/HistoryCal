/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformatunit.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     FormatUnit class to read and write as units.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     4th April 2016
 * Copyright:   Copyright (c) 2016, Nick Matthews.
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

#ifndef CAL_CALFORMATUNIT_H_GUARD
#define CAL_CALFORMATUNIT_H_GUARD

#include "calformat.h"

namespace Cal {

    class FormatUnit : public Format
    {
    public:
        FormatUnit( const std::string& code, Grammar* gmr ) : Format( code, gmr ) {}

        FormatType get_format_type() const override { return FT_unit; };
        std::string get_output( const Record& record ) const override;
        RangeList string_to_rlist( Base* base, const std::string& input ) const override;
        bool set_input( Record* record, const std::string& input, Boundary rb ) const override;

    private:
        void set_field_by_unit( Record* record, const std::string& value, const std::string& unit ) const;

    };

}

#endif // CAL_CALFORMATUNIT_H_GUARD