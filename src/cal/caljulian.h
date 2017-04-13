/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caljulian.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Julian calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
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

#ifndef CAL_CALJULIAN_H_GUARD
#define CAL_CALJULIAN_H_GUARD

#include "calbase.h"

namespace Cal {

    class Julian : public Base
    {
    public:
        Julian() {}
        Julian( const std::string& data ) : Base( data ) {}

        virtual size_t record_size() const { return 3; }

        virtual OptFieldID get_opt_field_id( const std::string& fieldname ) const;
        virtual std::string get_opt_fieldname( OptFieldID field_id ) const;

        virtual Field get_jdn( const Field* fields ) const;

        virtual Field get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const;

        virtual bool set_fields_as_next_optional( Field* fields, Field jdn, const Field* mask, size_t index ) const;
        virtual bool set_fields_as_prev_optional( Field* fields, Field jdn, const Field* mask, size_t index ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

        virtual Field get_field_last( const Field* fields, size_t index ) const;

        virtual double get_average_days( const Field* fields, Unit unit ) const;
        virtual bool add_to_fields( Field* fields, Field value, Unit unit ) const;
        virtual bool normalise( Field* fields, Norm norm ) const;

        virtual void resolve_opt_input( Field* fields, size_t index ) const;

        // Virtual members shared with Gregorian class.
        virtual Field jdn( Field year, Field month, Field day ) const;
        virtual Field easter( Field year ) const;

    protected:
        virtual bool is_leap_year( Field year ) const;
        Field last_day_in_month( Field year, Field month ) const;
    };

}

#endif // CAL_CALJULIAN_H_GUARD