/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calgregorian.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Gregorian calendar implimentation.
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


#include "calgregorian.h"

#include "calmath.h"

#include <cassert>

#define BASEDATE_Gregorian 1721060

using namespace Cal;

namespace {

    void gregorian_from_jdn( Field* year, Field* month, Field* day, Field jdn )
    {
        Field date = jdn - BASEDATE_Gregorian;
        *year = floor_div( date, 146097 ) * 400;
        date = pos_mod( date, 146097 );

        if( date < 60 )
        {
            if( date < 31 )
            {
                *month = 1;
                *day = date + 1;
                return;
            }
            *month = 2;
            *day = date - 30;
            return;
        }
        --date; // remove the leap day
        *year += ((date/36524) * 100);
        date %= 36524;

        if( date < 59 ) // Note, this is not a leap year
        {
            if( date < 31 )
            {
                *month = 1;
                *day = date + 1;
                return;
            }
            *month = 2;
            *day = date - 30;
            return;
        }
        ++date; // add the missing the leap day
        *year += (date/1461) * 4;
        date %= 1461;

        if( date < 60 )
        {
            if( date < 31 )
            {
                *month = 1;
                *day = date + 1;
                return;
            }
            *month = 2;
            *day = date - 30;
            return;
        }
        --date; // remove the leap day
        *year += date / 365;
        date %= 365;
        *month = 1;
        while( date >= latin_diy[(*month)+1] ) {
            (*month)++;
        }
        *day = date - latin_diy[*month] + 1;
        return;
    }

}

Field Gregorian::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid || fields[1] == f_invalid || fields[2] == f_invalid ) {
        return f_invalid;
    }
    return jdn( fields[0], fields[1], fields[2] );
}

void Gregorian::set_fields( Field* fields, Field jdn ) const
{
    gregorian_from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}

double Gregorian::get_average_days( const Field* fields, Unit unit ) const
{
    const double year = 365.2425;

    switch( unit )
    {
    case unit_year:
        return year;
    case unit_month:
        return year / 12;
    }
    // We only deal with non-integer units
    return 0.0;
}

/*! Returns the Julian Day Number for the given day, month and year
 *  in the Gregorian Calendar.
 */
Field Gregorian::jdn( Field year, Field month, Field day ) const
{
    Field jdn =
        floor_div( year, 400 )*146097         //     days in 400 year cycles
        + (pos_mod( year, 400 )/100)*36524    // - 1 days in 100 year cycles
        + (pos_mod( year, 100 )/4)*1461       // + 1 days in 4 year cycles
        + pos_mod( year, 4 )*365              // + 1 days in year
        + latin_diy[month] + day              // - 1 days numbered from 1 not 0
        + BASEDATE_Gregorian;

    // Adjust if in the 1st 2 months of 4 year cycle
    if( month < 3 && year%4 == 0 ) --jdn;

    // Adjust if in the 1st 2 months of 100 year cycle
    if( year%100 == 0 && month < 3 ) ++jdn;

    // Adjust if in the 1st 2 months of 400 year cycle
    if( year%400 == 0 && month < 3 ) --jdn;

    return jdn;
}

/*! Return the jdn for Easter Sunday in the given year.
 */
Field Gregorian::easter( Field year ) const
{
    Field century = year / 100 + 1;
    Field epact =
        (14 + 11*(year%19) - (3*century)/4 + (5+8*century)/25) % 30;
    if( epact == 0 || ( epact == 1 && 10 < (year%19) ) ) {
        epact++;
    }
    Field paschal_moon = jdn( year, 4, 19 ) - epact;
    return kday_after( WDAY_Sunday, paschal_moon );
}

/*! Returns true if the year is a leap year in the Gregorian Calendar.
 */
bool Gregorian::is_leap_year( Field year ) const
{
    return ( year%4 == 0 && year%100 != 0 ) || year%400 == 0;
}

// End of src/cal/calgregorian.cpp file
