/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calislamic.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Islamic Arithmetical calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th December 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
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

#include "calislamic.h"

#include "calmath.h"

#include <cassert>


using namespace Cal;

#define BASEDATE_Islamic    1948440L

namespace {

    /*! Returns true if the year is a leap year in the Islamic Calendar.
     */
    bool islamic_is_leap_year( Field year )
    {
        return pos_mod( 14 + 11 * year, 30 ) < 11;
    }

    /*! Returns the last day of the month for the given month and year
     *  in the Islamic Calendar.
     */
    Field islamic_last_day_in_month( Field year, Field month )
    {
    //    wxASSERT( month >= 1 && month <= 12 );
        if( month == 12 ) {
            return islamic_is_leap_year( year ) ? 30 : 29;
        } else {
            return (month % 2) == 0 ? 29 : 30;
        }
    }

    /*! Sets jdn to the Julian Day Number for the given day, month and year
     *  in the Islamic Calendar. Always returns true.
     */
    //bool calIslamicToJdn( long* jdn, long year, long month, long day )
    Field islamic_to_jdn( Field year, Field month, Field day )
    {
        return
            BASEDATE_Islamic - 1
            + ( year - 1 ) * 354
            + floor_div( year * 11 + 3, 30 )
            + 29 * ( month - 1 )
            + floor_div( month, 2 ) + day;
    }

    /*! Splits the given Julian Day Number date into the day, month and year
     *  for the Islamic Calendar.
     */
    bool islamic_from_jdn( Field* year, Field* month, Field* day, Field jdn )
    {
        *year = floor_div( 30 * ( jdn - BASEDATE_Islamic ) + 10646, 10631 );

        Field temp = islamic_to_jdn( *year, 1, 1 );
        *month = floor_div( 11 * ( jdn - temp ) + 330, 325 );

        temp = islamic_to_jdn( *year, *month, 1 );
        *day = 1 + jdn - temp;

        return true;
    }

} // namespace

//===================================

Field Islamic::get_jdn( const Field* fields ) const
{
    return islamic_to_jdn( fields[0], fields[1], fields[2] );
}

bool Islamic::set_fields_as_begin_first( Field* fields, const Field* mask ) const
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

bool Islamic::set_fields_as_begin_last( Field* fields, const Field* mask ) const
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
        islamic_last_day_in_month( fields[0], fields[1] ) : mask[2];
    return true;
}

void Islamic::set_fields( Field* fields, Field jdn ) const
{
    islamic_from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}


Field Islamic::get_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // Last month of year
        return 12;
    case 2: // Last day of month
        return islamic_last_day_in_month( fields[0], fields[1] );
    }
    return f_invalid;
}

double Islamic::get_average_days( const Field* fields, Unit unit ) const
{
    const double lunar_month = 29.53059;

    switch( unit )
    {
    case unit_year:
        return lunar_month * 12;
    case unit_month:
        return lunar_month;
    }
    // We only deal with non-integer units
    return 0.0;
}

bool Islamic::normalise( Field* fields, Norm norm ) const
{
    // Normalises for days in month, assumes months >= 1 and <= 12
    Field ldim = islamic_last_day_in_month( fields[0], fields[1] );
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

// End of src/cal/islamic.cpp
