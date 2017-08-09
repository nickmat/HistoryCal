/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caljdn.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Jdn (Julian Day Number) calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th December 2013
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

#ifndef CAL_CALJDN_H_GUARD
#define CAL_CALJDN_H_GUARD

#include "calbase.h"

namespace Cal {

    class Jdn : public Base
    {
    public:
        Jdn() {}
        Jdn( const std::string& data ) : Base( data ) {}

        size_t record_size() const override { return 1; }

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override;

        Field get_jdn( const Field* fields ) const override { return fields[0]; }

        void set_fields( Field* fields, Field jdn ) const override { fields[0] = jdn; }

    protected:
        int get_std_fieldname_index( const std::string& fieldname ) const override;
        std::string get_std_fieldname( size_t index ) const override;
    };

}

#endif // CAL_CALJDN_H_GUARD
