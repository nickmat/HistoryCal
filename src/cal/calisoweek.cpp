/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calisoweek.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base ISO Week calendar implimentation.
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

#include "calisoweek.h"

#include "calgregorian.h"
#include "calmath.h"


using namespace Cal;
using std::string;


Field IsoWeek::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid || fields[1] == f_invalid || fields[2] == f_invalid ) {
        return f_invalid;
    }
    return to_jdn( fields[0], fields[1], fields[2] );
}

bool IsoWeek::set_fields_as_begin_first( Field* fields, const Field* mask ) const
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

bool IsoWeek::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    if( mask[1] == f_invalid ) {
        fields[1] = last_week_of_year( fields[0] );
    } else {
        fields[1] = mask[1];
    }
    fields[2] = ( mask[2] == f_invalid ) ? 7 : mask[2];
    return true;
}

void IsoWeek::set_fields( Field* fields, Field jdn ) const
{
    from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}

Field IsoWeek::get_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // Last week of year
        return last_week_of_year( fields[0] );
    case 2: // Last day of week
        return 7;
    }
    return f_invalid;
}

double IsoWeek::get_average_days( const Field* fields, Unit unit ) const
{
    if( unit == unit_year ) {
        return 365.2425;
    }
    // We only deal with non-integer units
    return 0.0;
}

bool IsoWeek::normalise( Field* fields, Norm norm ) const
{
    // Normalises for weeks in year
    Field lwiy = last_week_of_year( fields[0] );
    if( fields[1] > lwiy ) {
        switch( norm )
        {
        case norm_expand:
            fields[0]++;
            fields[1] = lwiy - fields[1];
            break;
        case norm_expand_min:
            fields[0]++;
            fields[1] = 1;
            break;
        case norm_truncate:
            fields[1] = lwiy;
            break;
        }
        return true;
    }
    return false;
}

Field IsoWeek::to_jdn( Field year, Field week, Field day )
{
    Field jdn = kday_nearest( WDAY_Monday, Gregorian::to_jdn( year, 1, 1 ) );
    return jdn + ( week - 1 ) * 7 + day - 1;
}

void IsoWeek::from_jdn( Field* year, Field* week, Field* day, Field jdn )
{
    if( jdn != f_invalid ) {
        if( year ) {
            *year = year_from_jdn( jdn );
        }
        if( week ) {
            Field y = year ? *year : year_from_jdn( jdn );
            *week = div_f( jdn - to_jdn( y, 1, 1 ), 7 ) + 1;
        }
        if( day ) {
            *day = day_of_week( jdn ) + 1; // Mon=1, Sun=7
        }
    }
}

Field IsoWeek::year_from_jdn( Field jdn )
{
    Field year = Gregorian::year_from_jdn( jdn - 3 );
    if( jdn >= to_jdn( year + 1, 1, 1 ) ) {
        return year + 1;
    }
    return year;
}

int IsoWeek::get_std_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "year" ) {
        return 0;
    } else if( fieldname == "week" ) {
        return 1;
    } else if( fieldname == "wday" ) { // We use "wday" rather than "day"
        return 2;                      // to match the week day vocab
    }
    return -1;
}

string IsoWeek::get_std_fieldname( size_t index ) const
{
    static const char* names[] = { "year", "week", "wday" };
    if( index < sizeof( names ) ) {
        return names[index];
    }
    return "";
}

Field IsoWeek::last_week_of_year( Field year ) const
{
    return ( to_jdn( year + 1, 1, 1 ) - to_jdn( year, 1, 1 ) ) / 7;
}

// End of src/cal/calisoweek.cpp file
