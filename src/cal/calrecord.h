/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calrecord.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Header for Record class to hold date values.
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

#ifndef CAL_CALRECORD_H_GUARD
#define CAL_CALRECORD_H_GUARD

#include "cal/caldefs.h"

#include <string>
#include <vector>

namespace Cal {

    class Base;

    class Record
    {
    public:
        Record( const Base* base );
        Record( const Base* base, Field jdn );
        Record( const Base* base, const Field* fields, size_t size );
        Record( const Base* base, const std::string& str );
        Record( const Record& rec );
        virtual ~Record() {}

        void set_jdn( Field jdn );
        void set_fields( const Field* fields, size_t size );
        void set_str( const std::string& str );

        bool set_fields_as_begin_first( const Field* mask );
        bool set_fields_as_next_first( const Field* mask );
        bool set_fields_as_begin_last( const Field* mask );
        bool set_fields_as_next_last( const Field* mask );

        void remove_balanced_fields( Record* record );

        Field get_jdn() const;
        FieldVec get_fieldvec() { return m_f; };
        std::string get_str() const;

        Field* get_field_ptr() { return &m_f[0]; }
        const Field* get_field_ptr() const { return &m_f[0]; }

        Field is_unit_int( Unit unit ) const;
        bool can_add_unit( Unit unit ) const;
        bool add( Field value, Unit unit, Norm norm );
        bool normalise( Norm norm );
        double get_average_days( Unit unit ) const;

        Field get_field( int index ) const;
        void set_field( Field value, size_t index ) { m_f[index] = value; }

    private:
        enum InputFieldType { 
            IFT_null, IFT_quest, IFT_number, IFT_dual1, IFT_dual2, IFT_vocab
        };
        typedef std::vector<InputFieldType> IftVec;

        void clear_fields();

        int get_field_index( const std::string& fieldname ) const;

        enum CP_Group {
            GRP_Hyphen, GRP_Digit, GRP_Quest, GRP_Dual,
            GRP_Sep, GRP_Other
        };
        CP_Group get_cp_group( std::string::const_iterator it );
        Field get_token_value( const std::string& str );
        Field get_dual2_value( Field dual1, const std::string& str2 ) const;
        int parse_date( Field* nums, InputFieldType* ifts, size_t size, const std::string& str );

        std::string value_from_field( int index ) const;
        std::string lookup_token(
            int index, int dual, const std::string& vcode, const std::string& abbrev ) const;
        std::string get_output( const std::string& format ) const;

        const Base* m_base;
        FieldVec    m_f;
        Field       m_jdn;
    };

}

#endif // CAL_CALRECORD_H_GUARD
