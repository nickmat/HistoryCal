/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caljdn.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Jdn (Julian Day Number) calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th December 2013
 * Copyright:   Copyright (c) 2013-2015, Nick Matthews.
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

#ifndef CAL_CALJDN_H_GUARD
#define CAL_CALJDN_H_GUARD

#include "calbase.h"

namespace Cal {

    class Jdn : public Base
    {
    public:
        virtual size_t record_size() const { return 1; }

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const;

        virtual Field get_jdn( const Field* fields ) const { return fields[0]; }

        virtual void set_fields( Field* fields, Field jdn ) const { fields[0] = jdn; }

    protected:
        virtual int get_std_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_std_fieldname( size_t index ) const;
    };

}

#endif // CAL_CALJDN_H_GUARD
