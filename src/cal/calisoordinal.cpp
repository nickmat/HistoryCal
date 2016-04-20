/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calisoordinal.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base ISO Ordinal calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     22nd March 2016
 * Copyright:   Copyright (c) 2016, Nick Matthews.
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

#include "calisoordinal.h"

#include "calgregorian.h"
#include "calmath.h"


using namespace Cal;
using std::string;


Field IsoOrdinal::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid || fields[1] == f_invalid ) {
        return f_invalid;
    }
    return to_jdn( fields[0], fields[1] );
}

bool IsoOrdinal::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? 1 : mask[1];
    return true;
}

bool IsoOrdinal::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    fields[0] = mask[0];
    if( mask[1] == f_invalid ) {
        fields[1] = last_day_of_year( fields[0] );
    } else {
        fields[1] = mask[1];
    }
    return true;
}

void IsoOrdinal::set_fields( Field* fields, Field jdn ) const
{
    from_jdn( &fields[0], &fields[1], jdn );
}

Field IsoOrdinal::get_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // Last week of year
        return last_day_of_year( fields[0] );
    }
    return f_invalid;
}

double IsoOrdinal::get_average_days( const Field* fields, Unit unit ) const
{
    if( unit == unit_year ) {
        return 365.2425;
    }
    // We only deal with non-integer units
    return 0.0;
}

bool IsoOrdinal::normalise( Field* fields, Norm norm ) const
{
    // Normalises for weeks in year
    Field ldoy = last_day_of_year( fields[0] );
    if( fields[1] > ldoy ) {
        switch( norm )
        {
        case norm_expand:
            fields[0]++;
            fields[1] = ldoy - fields[1];
            break;
        case norm_expand_min:
            fields[0]++;
            fields[1] = 1;
            break;
        case norm_truncate:
            fields[1] = ldoy;
            break;
        }
        return true;
    }
    return false;
}

Field IsoOrdinal::to_jdn( Field year, Field day )
{
    return Gregorian::to_jdn( year, 1, 1 ) + day - 1;
}

void IsoOrdinal::from_jdn( Field* year, Field* day, Field jdn )
{
    if( jdn != f_invalid ) {
        if( year ) {
            *year = year_from_jdn( jdn );
        }
        if( day ) {
            Field y = year ? *year : year_from_jdn( jdn );
            *day = jdn - Gregorian::to_jdn( y, 1, 1 ) + 1;
        }
    }
}

Field IsoOrdinal::year_from_jdn( Field jdn )
{
    return Gregorian::year_from_jdn( jdn );
}

int IsoOrdinal::get_std_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "year" ) {
        return 0;
    } else if( fieldname == "day" ) {
        return 1;
    }
    return -1;
}

string IsoOrdinal::get_std_fieldname( size_t index ) const
{
    static char* names[] = { "year", "day" };
    if( index < sizeof( names ) ) {
        return names[index];
    }
    return "";
}

Field IsoOrdinal::last_day_of_year( Field year ) const
{
    return Gregorian::leap_year( year) ? 366 : 365;
}

// End of src/cal/calisoordinal.cpp file
