/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calfrench.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     French Republican calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th December 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
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

#ifndef CAL_CALFRENCH_H_GUARD
#define CAL_CALFRENCH_H_GUARD

#include "calbase.h"

namespace Cal {

    class French : public Base
    {
        enum FrenchFieldNumber {
            FFN_year, FFN_month, FFN_day,
            FFN_RCOUNT,
            FFN_nmonth = FFN_RCOUNT, FFN_nmday, FFN_cday, FFN_dday, FFN_wday,
            FFN_ECOUNT
        };
    public:
        virtual size_t record_size() const { return FFN_RCOUNT; }
        virtual size_t extended_size() const { return FFN_ECOUNT; }

        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;

        virtual Field get_jdn( const Field* fields ) const;
        virtual Field get_extended_field( const Field* fields, Field jdn, size_t index ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const { return false; }
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const { return false; }

        virtual bool set_fields_as_next_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const;
        virtual bool set_fields_as_prev_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

        virtual Field get_field_last( const Field* fields, size_t index ) const;

        virtual bool add_to_fields( Field* fields, Field value, Unit unit ) const { return false; }
        virtual bool normalise( Field* fields, Norm norm ) const;

        virtual bool resolve_input( Field* fields, const InputFieldVec& input, const std::string& fmt_code ) const;
    };

}

#endif // CAL_CALFRENCH_H_GUARD