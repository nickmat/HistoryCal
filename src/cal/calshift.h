/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calshift.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Shift variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     22nd September 2013
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

#ifndef CAL_CALSHIFT_H_GUARD
#define CAL_CALSHIFT_H_GUARD

#include "calbase.h"
#include <string>
#include <vector>

namespace Cal {

    class Schemes;
    class Record;

    class Shift : public Base
    {
    public:
        Shift( Schemes* schemes, const std::string& data );
        ~Shift();

        virtual size_t record_size() const { return m_base->record_size(); }
        virtual size_t extended_size() const { return m_base->extended_size() + 1; }

        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;

        virtual Field get_jdn( const Field* fields ) const;
        virtual Field get_extended_field( const Field jdn, size_t index ) const;

        virtual Field get_field_last( const Field* fields, size_t index ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask );

        virtual void remove_fields_if_first( Field* fields ) const;
        virtual void remove_fields_if_last( Field* fields ) const;

        virtual void remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

        virtual Field unit_is_int( const Field* fields, Unit unit ) const;
        virtual bool can_add_unit( const Field* fields, Unit unit ) const;
        virtual double get_average_days( const Field* fields, Unit unit ) const;
        virtual bool add_to_fields( Field* fields, Field value, Unit unit ) const;
        virtual bool normalise( Field* fields, Norm norm ) const;

    private:
        Field get_adjustment( const Field* fields ) const;
        FieldVec get_vec_adjusted_to_base( const Field* fields ) const;
        Field get_adjusted_to_base( const Field* fields ) const;
        Field get_adjusted_to_shift( const Field* fields ) const;

        Base*   m_base;
        FieldVec m_start_era;
        FieldVec m_before_era;
    };

}

#endif // CAL_CALSHIFT_H_GUARD