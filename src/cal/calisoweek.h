/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calisoweek.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base ISO Week calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     17th March 2016
 * Copyright:   Copyright (c) 2016 ~ 2017, Nick Matthews.
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

#ifndef CAL_CALISOWEEK_H_GUARD
#define CAL_CALISOWEEK_H_GUARD

#include "calbase.h"

namespace Cal {

    class IsoWeek : public Base
    {
    public:
        IsoWeek() {}
        IsoWeek( const std::string& data ) : Base( data ) {}

        size_t record_size() const override { return 3; }

        Field get_jdn( const Field* fields ) const override;

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override { return false; }
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override { return false; }

        void set_fields( Field* fields, Field jdn ) const override;

        Field get_rec_field_last( const Field* fields, size_t index ) const override;

        double get_average_days( const Field* fields, Unit unit ) const override;
        bool normalise( Field* fields, Norm norm ) const override;

        static Field to_jdn( Field year, Field week, Field day );
        static void from_jdn( Field* year, Field* week, Field* day, Field jdn );
        static Field year_from_jdn( Field jdn );

    protected:
        int get_std_fieldname_index( const std::string& fieldname ) const override;
        std::string get_std_fieldname( size_t index ) const override;

    private:
        Field last_week_of_year( Field year ) const;
    };

}

#endif // CAL_CALISOWEEK_H_GUARD
