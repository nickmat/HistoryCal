/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calrecord.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Header for Record class to hold date values.
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

#ifndef CAL_CALRECORD_H_GUARD
#define CAL_CALRECORD_H_GUARD

#include "calprivate.h"

namespace Cal {

    class Base;

    class Record
    {
    public:
        Record( const Base* base );
        Record( const Base* base, Field jdn );
        Record( const Base* base, const Field* fields, size_t size );
        Record( const Base* base, const std::string& str, const std::string& fcode, Boundary rb );
        Record( const Record& rec );

        void clear_fields();

        void set_jdn( Field jdn );
        void set_fields( const Field* fields, size_t size );
        void set_str( const std::string& str, const std::string& fcode, Boundary rb );

        bool set_fields_as_begin_first( const Field* mask, bool check = true );
        bool set_fields_as_next_first( const Field* mask );
        bool correct_fields_as_first( const Field* mask );
        bool set_fields_as_begin_last( const Field* mask, bool check = true );
        bool set_fields_as_next_last( const Field* mask );
        bool correct_fields_as_last( const Field* mask );

        RangeList get_rlist_from_mask() const;
        bool set_range_as_begin( Range* range ) const;
        bool set_range_as_next( Range* range ) const;

        BoolVec mark_balanced_fields( Record& record, const XRefVec& rank_xref );

        Field get_jdn() const;
        Field jdn() const { return m_jdn; }
        FieldVec get_fieldvec() const { return m_f; };
        std::string get_str( const std::string& fcode ) const;

        Field* get_field_ptr() { return &m_f[0]; }
        const Field* get_field_ptr() const { return &m_f[0]; }
        const Base* get_base() const { return m_base; }

        Field is_unit_int( Unit unit ) const;
        bool can_add_unit( Unit unit ) const;
        bool add( Field value, Unit unit, Norm norm );
        bool normalise( Norm norm );
        double get_average_days( Unit unit ) const;

        Field get_field( int index, const BoolVec* mask = nullptr ) const;
        Field get_field_at( int index ) const { return m_f[index]; }
        Field get_field( const std::string& fieldname ) const;
        void set_field( Field value, size_t index ) { m_f[index] = value; }

        bool is_mask_valid( Field* mask, size_t mask_size ) const;
        int get_field_index( const std::string& fieldname ) const;
        int get_unit_index( const std::string& unitname ) const;

    private:
        const Base* m_base;
        FieldVec    m_f;
        Field       m_jdn;
    };

}

#endif // CAL_CALRECORD_H_GUARD
