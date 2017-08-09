/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calhebrew.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Hebrew calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     10th December 2014
 * Copyright:   Copyright (c) 2014 ~ 2017, Nick Matthews.
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

#ifndef CAL_CALHEBREW_H_GUARD
#define CAL_CALHEBREW_H_GUARD

#include "calbase.h"

namespace Cal {

    class Hebrew : public Base
    {
    public:
        Hebrew() {}
        Hebrew( const std::string& data ) : Base( data ) {}

        size_t record_size() const override { return 3; }

        Field get_jdn( const Field* fields ) const override;

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override { return false; }
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override { return false; }

        void set_fields( Field* fields, Field jdn ) const override;

        Field get_field_first( const Field* fields, size_t index ) const override;
        Field get_field_last( const Field* fields, size_t index ) const override;

        double get_average_days( const Field* fields, Unit unit ) const override;
        bool add_to_fields( Field* fields, Field value, Unit unit ) const override;
        bool normalise( Field* fields, Norm norm ) const override;
    };

}

#endif // CAL_CALHEBREW_H_GUARD