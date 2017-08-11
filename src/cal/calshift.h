/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calshift.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Shift variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     22nd September 2013
 * Copyright:   Copyright (c) 2013 - 2017, Nick Matthews.
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

#ifndef CAL_CALSHIFT_H_GUARD
#define CAL_CALSHIFT_H_GUARD

#include "calbase.h"
#include <string>
#include <vector>

namespace Cal {

    class Calendars;
    class Record;

    class Shift : public Base
    {
    public:
        Shift( Base* base, Field era );
        ~Shift();

        bool is_ok() const override { return m_ok; }

        size_t record_size() const override { return m_base->record_size(); }

        Field get_jdn( const Field* fields ) const override;

        Field get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const override;

        Field get_rec_field_last( const Field* fields, size_t index ) const override;

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override;

        void remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const override;
        BoolVec mark_balanced_fields(
            Field* left, Field ljdn, Field* right, Field rjdn, const XRefVec& rank ) const override;

        void set_fields( Field* fields, Field jdn ) const override;

        Field unit_is_int( const Field* fields, Unit unit ) const override;
        bool can_add_unit( const Field* fields, Unit unit ) const override;
        double get_average_days( const Field* fields, Unit unit ) const override;
        bool add_to_fields( Field* fields, Field value, Unit unit ) const override;
        bool normalise( Field* fields, Norm norm ) const override;

    private:
        Field get_adjustment( const Field* fields ) const;
        FieldVec get_vec_adjusted_to_base( const Field* fields ) const;
        Field get_adjusted_to_base( const Field* fields ) const;
        Field get_adjusted_to_shift( const Field* fields ) const;

        Base*    m_base;
        FieldVec m_start_era;
        FieldVec m_before_era;
        bool     m_ok;
    };

}

#endif // CAL_CALSHIFT_H_GUARD