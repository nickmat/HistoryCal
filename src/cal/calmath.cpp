/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calmath.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Miscellaneous math functions.
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

#include "calmath.h"

#include <cmath>

using namespace Cal;

// Use our own value of pi for consistancy
double const Cal::cal_pi = 3.141592653589793;

// Days in the year at the start of each month (Jan = 1) (not leap year)
Field Cal::latin_diy[14] = {
    // Note: we add an extra 0 to the beginning of the array to save
    //       subtracting 1 from the month number
    0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

Field Cal::latin_length_of_month[3][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, // Nomal year
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, // Leap year
    { 31, 30, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }  // For the Swedish calendar scheme
};

// Helper function.
// Integer function to return floor( a / b )
Field Cal::floor_div( Field a, Field b )
{
    Field q = a / b;
    if( a % b < 0 ) --q;
    return q;
}

// Helper function.
// Integer function to return positive value of ( a % b )
Field Cal::pos_mod( Field a, Field b )
{
    Field r = a % b;
    if( r < 0 ) r += b;
    return r;
}

#define calSEARCH_MAX 30

// CC3 p20
Field Cal::min_search( Field start, calSearchFunc func, const void* data )
{
    for( Field i = 0, d = start ; i < calSEARCH_MAX ; i++, d++ ) {
        if( func( d, data ) ) {
            return d;
        }
    }
    return f_invalid;
}

int Cal::cal_signum( double n )
{
    if( n > 0 ) return 1;
    if( n < 0 ) return -1;
    return 0;
}

double Cal::cal_mod( double x, double y )
{
    return x - y * floor( x / y ); 
} 

Field Cal::cal_round( double x )
{
  Field i = Field( x );
  if( x >= 0.0 ) {
      return (x-i >= 0.5) ? i + 1 : i;
  }
  return (-x+i >= 0.5) ? i - 1 : i;
}

// End of src/cal/calmath.cpp
