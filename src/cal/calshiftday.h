/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calshiftday.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base ShiftDay variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     18th March 2015
 * Copyright:   Copyright (c) 2015, Nick Matthews.
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
        ShiftDay( Base* base, Field epoch ) : m_base(base), m_epoch(epoch) {}

        virtual bool is_ok() const { return m_base != NULL; }

        virtual size_t record_size() const { return m_base->record_size(); }
        virtual size_t extended_size() const { return m_base->extended_size(); }

        virtual int get_fieldname_index( const std::string& fieldname ) const { return m_base->get_fieldname_index( fieldname ); }
        virtual std::string get_fieldname( size_t index ) const { return m_base->get_fieldname( index ); }

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const { return m_base->set_fields_as_begin_first( fields, mask ); }
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const { return m_base->set_fields_as_next_first( fields, mask ); }
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const { return m_base->set_fields_as_begin_last( fields, mask ); }
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const { return m_base->set_fields_as_next_last( fields, mask ); }

        virtual Field get_jdn( const Field* fields ) const { return fields[0] + m_epoch; }

        virtual void set_fields( Field* fields, Field jdn ) const { fields[0] = jdn - m_epoch; }

    private:
        Base*  m_base;
        Field  m_epoch;
    };

}

#endif // CAL_CALSHIFTDAY_H_GUARD