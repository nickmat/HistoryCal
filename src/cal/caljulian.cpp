/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caljulian.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Julian calendar implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
 * Copyright:   Copyright (c) 2013-2015, Nick Matthews.
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

#include "caljulian.h"

#include "calliturgical.h"
#include "calmath.h"
#include "calrecord.h"

#include <cassert>

using namespace Cal;
using std::string;

#define BASEDATE_Julian    1721058L

namespace {

    /*! Splits the given Julian Day Number date into the day, month and year
     *  for the Julian Calendar.
     */
    void julian_from_jdn( Field* year, Field* month, Field* day, Field jdn )
    {
        jdn -= BASEDATE_Julian;

        *year = floor_div( jdn, 1461 ) * 4;
        jdn = pos_mod( jdn, 1461 );

        if( jdn < 60 )
        {
            if( jdn < 31 )
            {
                *month = 1;
                *day = jdn + 1;
                return;
            }
            *month = 2;
            *day = jdn - 30;
            return;
        }
        --jdn; // remove the leap day
        *year += (int) jdn / 365;
        jdn %= 365;
        *month = 1;
        while( jdn >= latin_diy[(*month)+1] ) (*month)++;
        *day = jdn - latin_diy[*month] + 1;
        return;
    }

}

OptFieldID Julian::get_opt_field_id( const std::string& fieldname ) const
{
    if( fieldname == "ce" ) {  // 0 = BCE, 1 = CE
        return OFID_j_ce;
    }
    if( fieldname == "ceyear" ) {  // Positive CE or BCE year
        return OFID_j_ceyear;
    }
    if( fieldname == "litweek" ) {
        return OFID_j_litweek;
    }
    if( fieldname == "eastermonth" ) {
        return OFID_j_eastermonth;
    }
    if( fieldname == "easterday" ) {
        return OFID_j_easterday;
    }
    if( fieldname == "easter" ) {
        return OFID_j_easter;
    }
    if( fieldname == "eastershift" ) {
        return OFID_j_eastershift;
    }
    if( fieldname == "easterrpt" ) {
        return OFID_j_easterrpt;
    }
    return Base::get_opt_field_id( fieldname );
}

std::string Julian::get_opt_fieldname( OptFieldID field_id ) const
{
    switch( field_id )
    {
    case OFID_j_ce:
        return "ce";
    case OFID_j_ceyear:
        return "ceyear";
    case OFID_j_litweek:
        return "litweek";
    case OFID_j_eastermonth:
        return "eastermonth";
    case OFID_j_easterday:
        return "easterday";
    case OFID_j_easter:
        return "easter";
    case OFID_j_eastershift:
        return "eastershift";
    case OFID_j_easterrpt:
        return "easterrpt";
    default:
        return Base::get_opt_fieldname( field_id );
    }
}

Field Julian::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid || fields[1] == f_invalid || fields[2] == f_invalid ) {
        return f_invalid;
    }
    return jdn( fields[0], fields[1], fields[2] );
}

Field Julian::get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const
{
    switch( id )
    {
    case OFID_j_ce:  // 0 = BCE, 1 = CE
        return fields[YMD_year] < 1 ? 0 : 1;
    case OFID_j_ceyear: // Positive CE or BCE year
        return fields[YMD_year] < 1 ? -fields[YMD_year] + 1 : fields[YMD_year];
    case OFID_j_litweek:
        return liturgical_get_litweek( this, jdn );
    case OFID_j_eastermonth:
        {
            Record rec( this, easter( fields[YMD_year] ) );
            return rec.get_field( YMD_month );
        }
    case OFID_j_easterday:
        {
            Record rec( this, easter( fields[YMD_year] ) );
            return rec.get_field( YMD_day );
        }
    case OFID_j_easter:
        return easter( fields[YMD_year] );
    case OFID_j_eastershift:
        {
            Record rec( this, easter( fields[YMD_year] ) );
            if( rec.get_field( YMD_month ) < fields[YMD_month] ||
                ( rec.get_field( YMD_month ) ==  fields[YMD_month] &&
                rec.get_field( YMD_day ) < fields[YMD_day] ) )
            {
                return rec.get_field( YMD_year );
            } else {
                return rec.get_field( YMD_year ) - 1;
            }
        }
    default:
        return Base::get_opt_field( fields, jdn, id );
    }
}

bool Julian::set_fields_as_begin_first( Field* fields, const Field* mask ) const
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

bool Julian::set_fields_as_next_first( Field* fields, const Field* mask ) const
{
    return false;
}

bool Julian::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? 12 : mask[1];
    if( mask[2] == f_invalid ) {
        fields[2] = last_day_in_month( fields[0], fields[1] );
        if( fields[2] == f_invalid ) {
            return false;
        }
    } else {
        fields[2] = mask[2];
    }
    return true;
}

bool Julian::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    return false;
}

// Return true if the fields where changed.
bool Julian::set_fields_as_next_optional( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
    assert( fields[YMD_year] != f_invalid );
    assert( fields[YMD_month] != f_invalid );
    assert( fields[YMD_day] != f_invalid );
    assert( jdn != f_invalid );
    if( index >= (extended_size() - opt_fields_size() ) ) {
        OptFieldID id = opt_index_to_id( index );
        switch( id )
        {
        case OFID_j_litweek:
            {
                Field litweek = mask[index];
                Field current = liturgical_get_litweek( this, jdn );
                if( litweek != current ) {
                    Field year = fields[YMD_year];
                    Field njdn = liturgical_get_jdn( this, year, litweek );
                    if( njdn > jdn ) {
                        njdn = liturgical_get_jdn( this, year-1, litweek );
                    }
                    if( njdn > jdn ) {
                        Record rec(this,njdn);
                        copy_fields( fields, rec.get_field_ptr() );
                        return true;
                    }
                }
            }
            return false;
        }
    }
    return Base::set_fields_as_next_optional( fields, jdn, mask, index );
}

// Return true if the fields where changed.
bool Julian::set_fields_as_prev_optional( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
    assert( fields[YMD_year] != f_invalid );
    assert( fields[YMD_month] != f_invalid );
    assert( fields[YMD_day] != f_invalid );
    assert( jdn != f_invalid );
    if( index >= (extended_size() - opt_fields_size() ) ) {
        OptFieldID id = opt_index_to_id( index );
        switch( id )
        {
        case OFID_j_litweek:
            {
                Field litweek = mask[index];
                Field current = liturgical_get_litweek( this, jdn );
                if( litweek != current ) {
                    Field year = fields[YMD_year];
                    Field pjdn = liturgical_get_jdn( this, year, litweek ) + 6;
                    if( pjdn < jdn ) {
                        pjdn = liturgical_get_jdn( this, year+1, litweek ) + 6;
                    }
                    if( pjdn < jdn ) {
                        Record rec(this,pjdn);
                        copy_fields( fields, rec.get_field_ptr() );
                        return true;
                    }
                }
            }
            return false;
        }
    }
    return Base::set_fields_as_prev_optional( fields, jdn, mask, index );
}

void Julian::set_fields( Field* fields, Field jdn ) const
{
    julian_from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}

Field Julian::get_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // Last month of year
        return 12;
    case 2: // Last day of month
        return last_day_in_month( fields[0], fields[1] );
    }
    return f_invalid;
}

double Julian::get_average_days( const Field* fields, Unit unit ) const
{
    const double year = 365.25;

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

bool Julian::add_to_fields( Field* fields, Field value, Unit unit ) const
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

bool Julian::normalise( Field* fields, Norm norm ) const
{
    // Normalises for days in month, assumes months >= 1 and <= 12
    Field ldim = last_day_in_month( fields[0], fields[1] );
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

void Julian::resolve_opt_input( Field* fields, size_t index ) const
{
    assert( fields[index] != f_invalid );
    OptFieldID id = opt_index_to_id( index );
    switch( id )
    {
    case OFID_j_ceyear:
        if( fields[YMD_year] == f_invalid ) {
            int cei = opt_id_to_index( OFID_j_ce );
            if( cei >= 0 ) {
                switch( fields[cei] )
                {
                case 0:
                    fields[YMD_year] = -(fields[index]-1);
                    break;
                default:
                    fields[YMD_year] = fields[index];
                    break;
                }
            }
        }
        break;
    case OFID_j_litweek:
        // If the "day" and "month" fields are missing
        // but the "wday", "litweek" and "year" fields are given,
        // these are used to fill in the missing fields.
        if( fields[YMD_year] != f_invalid ) {
            // Before we fill these in, both litweek and wday fields must be invalid
            int windex = opt_id_to_index( OFID_wday );
            if( windex < 0 || fields[windex] == f_invalid ) {
                break;
            }
            // And month and day invalid.
            if( fields[YMD_month] != f_invalid || fields[YMD_day] != f_invalid ) {
                break;
            }
            Weekday wday = day_of_week( fields[windex] - 1 );
            Field jdn = liturgical_get_jdn( this, fields[YMD_year], fields[index] );
            Record rec( this, kday_on_or_after( wday, jdn ) );
            fields[YMD_month] = rec.get_field( YMD_month );
            fields[YMD_day] = rec.get_field( YMD_day );
        }
        break;
    default:
        break;
    }
}

/*! Sets jdn to the Julian Day Number for the given day, month and year
 *  in the Julian Calendar. Always returns true.
 */
Field Julian::jdn( Field year, Field month, Field day ) const
{
    Field jdn =
        floor_div( year, 4 ) * 1461 + pos_mod( year, 4 ) * 365
        + latin_diy[month] + day + BASEDATE_Julian;

    // Adjust if in the 1st 2 months of 4 year cycle
    if( month < 3 && (year % 4) == 0 ) --(jdn);

    return jdn;
}

/*! Return the jdn for Easter Sunday in the given year.
 */
Field Julian::easter( Field year ) const
{
    Field shifted_epact = ( 14 + 11 * ( year % 19 ) ) % 30;
    Field paschal_moon = jdn( year, 4, 19 ) - shifted_epact;
    return kday_after( WDAY_Sunday, paschal_moon );
}

/*! Returns true if the year is a leap year in the Julian Calendar.
 */
bool Julian::is_leap_year( Field year ) const
{
    return ( year % 4 ) == 0;
}

/*! Returns the last day of the month for the given month and year.
 */
Field Julian::last_day_in_month( Field year, Field month ) const
{
    switch( month )
    {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        return is_leap_year( year ) ? 29 : 28;
    }
    return f_invalid;
}

// End of src/cal/caljulian.cpp file
