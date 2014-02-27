/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caljulian.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Julian calendar header.
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

#ifndef CAL_CALJULIAN_H_GUARD
#define CAL_CALJULIAN_H_GUARD

#include "calbase.h"

namespace Cal {
    class Julian : public Base
    {
    public:
        virtual size_t record_size() const { return 3; }

        virtual Field get_jdn( const Field* fields ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask );

        virtual void remove_fields_if_first( Field* fields ) const;
        virtual void remove_fields_if_last( Field* fields ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

        virtual Field get_field_last( const Field* fields, size_t index ) const;

        virtual double get_average_days( const Field* fields, Unit unit ) const;
        virtual bool add_to_fields( Field* fields, Field value, Unit unit ) const;
        virtual bool normalise( Field* fields, Norm norm ) const;
    };

}

#endif // CAL_CALJULIAN_H_GUARD