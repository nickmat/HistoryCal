/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calshiftday.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base ShiftDay variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     18th March 2015
 * Copyright:   Copyright (c) 2015 ~ 2020, Nick Matthews.
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

#ifndef CAL_CALSHIFTDAY_H_GUARD
#define CAL_CALSHIFTDAY_H_GUARD

#include "calbase.h"

namespace Cal {

    class ShiftDay : public Base
    {
    public:
        ShiftDay( const Base* base, Field epoch ) : m_base(base), m_epoch(epoch) {}

        bool is_ok() const override { return m_base != NULL && m_epoch != f_invalid; }

        size_t record_size() const override { return m_base->record_size(); }
        size_t extended_size() const override { return m_base->extended_size(); }

        int get_fieldname_index( const std::string& fieldname ) const override { return m_base->get_fieldname_index( fieldname ); }
        std::string get_fieldname( size_t index ) const override { return m_base->get_fieldname( index ); }

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override { return m_base->set_fields_as_begin_first( fields, mask ); }
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override { return m_base->set_fields_as_next_first( fields, mask ); }
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override { return m_base->set_fields_as_begin_last( fields, mask ); }
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override { return m_base->set_fields_as_next_last( fields, mask ); }

        Field get_jdn( const Field* fields ) const override { return fields[0] + m_epoch; }

        void set_fields( Field* fields, Field jdn ) const override { fields[0] = jdn - m_epoch; }

    private:
        const Base*  m_base;
        Field  m_epoch;
    };

}

#endif // CAL_CALSHIFTDAY_H_GUARD