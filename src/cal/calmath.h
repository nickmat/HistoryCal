/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calmath.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Miscellaneous math functions header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
 * Copyright:   Copyright (c) 2013 - 2015, Nick Matthews.
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

#ifndef CAL_CALMATH_H_GUARD
#define CAL_CALMATH_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    // We use our own value of pi for consistancy.
    extern const double cal_pi;

    extern Field latin_diy[14];
    extern Field latin_length_of_month[3][12];

    //! Integer function to return floor( a / b )
    extern Field floor_div( Field a, Field b );

    //! Integer function to return positive value of ( a % b )
    extern Field pos_mod( Field a, Field b );

    typedef bool (*calLongSearchFunc)( Field value, Field constant );

    extern Field min_search( Field start, calLongSearchFunc func, Field constant );

    extern int cal_signum( double n );

    extern double cal_mod( double x, double y );

    extern Field cal_round( double x );

    // Inline functions

    double inline deg_to_rad( double deg ) { return deg * cal_pi / 180; }

    double inline rad_to_deg( double rad ) { return rad * 180 / cal_pi; }

    enum Weekday { 
        WDAY_Monday, WDAY_Tuesday, WDAY_Wednesday,
        WDAY_Thursday, WDAY_Friday, WDAY_Saturday, WDAY_Sunday
    };

    Weekday inline day_of_week( Field jdn ) { 
        return Weekday( jdn % 7 );
    }
    Field inline kday_on_or_before( Weekday wday, Field jdn ) { 
        return jdn - day_of_week( jdn - wday );
    }
    Field inline kday_on_or_after( Weekday wday, Field jdn ) { 
        return kday_on_or_before( wday, jdn + 6 );
    }
    Field inline kday_nearest( Weekday wday, Field jdn ) { 
        return kday_on_or_before( wday, jdn + 3 );
    }
    Field inline kday_before( Weekday wday, Field jdn ) { 
        return kday_on_or_before( wday, jdn - 1 );
    }
    Field inline kday_after( Weekday wday, Field jdn ) { 
        return kday_on_or_before( wday, jdn + 7 );
    }
}

#endif // CAL_CALMATH_H_GUARD