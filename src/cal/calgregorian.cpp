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

    /*! Returns the Julian Day Number for the given day, month and year
     *  in the Gregorian Calendar.
     */
    Field gregorian_to_jdn( Field year, Field month, Field day )
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

    /*! Returns true if the year is a leap year in the Gregorian Calendar.
     */
    bool gregorian_is_leap_year( Field year )
    {
        return ( year%4 == 0 && year%100 != 0 ) || year%400 == 0;
    }

    /*! Returns the last day of the month for the given month and year
     *  in the Gregorian Calendar.
     */
    Field gregorian_last_day_in_month( Field year, Field month )
    {
        switch( month )
        {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return gregorian_is_leap_year( year ) ? 29 : 28;
        }
        return f_invalid;
    }

}

Field Gregorian::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid || fields[1] == f_invalid || fields[2] == f_invalid ) {
        return f_invalid;
    }
    return gregorian_to_jdn( fields[0], fields[1], fields[2] );
}

bool Gregorian::set_fields_as_begin_first( Field* fields, const Field* mask )
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? 1 : mask[1];
    fields[2] = ( mask[2] == f_invalid ) ? 1 : mask[2];
    return true;
}

bool Gregorian::set_fields_as_next_first( Field* fields, const Field* mask )
{
    return false;
}

bool Gregorian::set_fields_as_begin_last( Field* fields, const Field* mask )
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? 12 : mask[1];
    fields[2] = ( mask[2] == f_invalid ) ? 
        gregorian_last_day_in_month( fields[0], fields[1] ) : mask[2];
    return true;
}

bool Gregorian::set_fields_as_next_last( Field* fields, const Field* mask )
{
    return false;
}
#if 0
void Gregorian::remove_fields_if_first( Field* fields ) const
{
    if( fields[2] == 1 ) {
        fields[2] = f_invalid;
    }
    if( fields[2] != f_invalid ) {
        return;
    }
    if( fields[1] == 1 ) {
        fields[1] = f_invalid;
    }
}

void Gregorian::remove_fields_if_last( Field* fields ) const
{
    if( fields[2] == gregorian_last_day_in_month( fields[0], fields[1] ) ) {
        fields[2] = f_invalid;
    }
    if( fields[2] != f_invalid ) {
        return;
    }
    if( fields[1] == 12 ) {
        fields[1] = f_invalid;
    }
}
#endif
void Gregorian::set_fields( Field* fields, Field jdn ) const
{
    gregorian_from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}

Field Gregorian::get_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // Last month of year
        return 12;
    case 2: // Last day of month
        return gregorian_last_day_in_month( fields[0], fields[1] );
    }
    return f_invalid;
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

bool Gregorian::add_to_fields( Field* fields, Field value, Unit unit ) const
{
    switch( unit )
    {
    case unit_year:
        fields[0] += value;
        return true;
    case unit_month:
        fields[0] += value / 12;
        fields[1] += value % 12;
        while( fields[1] > 12 ) {
            fields[0]++;
            fields[1] -= 12;
        }
        while( fields[1] < 1 ) {
            --fields[0];
            fields[1] += 12;
        }
        return true;
    }
    return false;
}

bool Gregorian::normalise( Field* fields, Norm norm ) const
{
    // Normalises for days in month, assumes months >= 1 and <= 12
    Field ldim = gregorian_last_day_in_month( fields[0], fields[1] );
    if( fields[2] > ldim ) {
        switch( norm )
        {
        case norm_expand:
            fields[1]++;
            fields[2] = ldim - fields[2];
            break;
        case norm_expand_min:
            fields[1]++;
            fields[2] = 1;
            break;
        case norm_truncate:
            fields[2] = ldim;
            break;
        }
        if( fields[1] > 12 ) {
            fields[0]++;
            fields[1] -= 12;
        }
        return true;
    }
    return false;
}

// End of src/cal/calgregorian.cpp file
