/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calcyclic.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base for cyclic calendars header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th October 2013
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

#ifndef CAL_CALCYCLIC_H_GUARD
#define CAL_CALCYCLIC_H_GUARD

#include "calbase.h"

namespace Cal {

    class Cyclic : public Base
    {
    public:
        Cyclic( const std::string& data );
        ~Cyclic();

        virtual size_t record_size() const { return 2; }

        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;

        virtual Field get_jdn( const Field* fields ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask );

        virtual void remove_fields_if_first( Field* fields ) const;
        virtual void remove_fields_if_last( Field* fields ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

        virtual Field get_last( const Field* fields, size_t index ) const { return m_cycle; }

    private:
        Field m_start;
        Field m_cycle;
        Field m_first;
    };

}

#endif // CAL_CALCYCLIC_H_GUARD