/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calgregorian.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Gregorian calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
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

#ifndef CAL_CALGREGORIAN_H_GUARD
#define CAL_CALGREGORIAN_H_GUARD

#include "caljulian.h"

namespace Cal {

    class Gregorian : public Julian
    {
    public:
        Gregorian() {}
        Gregorian( const std::string& data ) : Julian( data ) {}

        Field get_jdn( const Field* fields ) const override;

        void set_fields( Field* fields, Field jdn ) const override;

        double get_average_days( const Field* fields, Unit unit ) const override;

        Field jdn( Field year, Field month, Field day ) const override;
        Field easter( Field year ) const override;


        static Field to_jdn( Field year, Field month, Field day );
        static void from_jdn( Field* year, Field* month, Field* day, Field jdn );
        static Field year_from_jdn( Field jdn );
        static bool leap_year( Field year );
        static Field today();

    protected:
        bool is_leap_year( Field year ) const override { return leap_year( year ); }
    };

}

#endif // CAL_CALGREGORIAN_H_GUARD
