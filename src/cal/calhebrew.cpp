/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calhebrew.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Hebrew calendar implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     10th December 2014
 * Copyright:   Copyright (c) 2014-2015, Nick Matthews.
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

#include "calhebrew.h"

#include "calmath.h"

#include <cassert>

using namespace Cal;
using std::string;

#define BASEDATE_Hebrew    347998L

namespace {

    Field hebrew_elapsed_days( Field year )
    {
        Field months = floor_div( 235 * year - 234, 19 );
        Field days = 29 * months + floor_div( 12084L + 13753L * months, 25920 );
        if( ( ( 3 * ( days + 1 ) ) % 7 ) < 3 ) {
            return days + 1;
        }
        return days;
    }

    Field hebrew_new_year( Field year )
    {
        Field ny0 = hebrew_elapsed_days( year - 1 );
        Field ny1 = hebrew_elapsed_days( year );
        Field ny2 = hebrew_elapsed_days( year + 1 );
        Field correction = 0;
        if( ( ny2 - ny1 ) == 356 ) {
            correction = 2;
        } else if( ( ny1 - ny0 ) == 382 ) {
            correction = 1;
        }
        return BASEDATE_Hebrew + ny1 + correction;
    }

    Field hebrew_days_in_year( Field year )
    {
        return hebrew_new_year( year + 1 ) - hebrew_new_year( year );
    }

    /*! Returns true if the year is a leap year in the Hebrew Calendar.
     *  Note that a Hebrew leap years are years with an additional 13th
     *  month. Also note, non-leap years are not all the same length.
     */
    bool hebrew_is_leap_year( Field year )
    {
        return pos_mod( 1 + 7 * year, 19 ) < 7;
    }

    /*! Returns the last day of the month for the given year and month
     *  in the Hebrew Calendar.
     */
    Field hebrew_last_day_in_month( Field year, Field month )
    {
    //    wxASSERT( month >= 1 && month <= 13 );
        Field diy;
        switch( month )
        {
        case 2: case 4: case 6: case 10: case 13:
            return 29;
        case 8:
            diy = hebrew_days_in_year( year );
            if( diy != 355 && diy != 385 ) {
                return 29;
            }
            break;
        case 9:
            diy = hebrew_days_in_year( year );
            if( diy == 353 || diy == 383 ) {
                return 29;
            }
            break;
        case 12:
            if( ! hebrew_is_leap_year( year ) ) {
                return 29;
            }
            break;
        }
        return 30;
    }

    /*! Sets jdn to the Julian Day Number for the given day, month and year
     *  in the Hebrew Calendar. Always returns true.
     */
    Field hebrew_to_jdn( Field year, Field month, Field day )
    {
        Field diy = hebrew_days_in_year( year );
        bool leap = hebrew_is_leap_year( year );

        Field mdays = 0;
        switch( month )
        {
        case 6: // fall thru intended
            mdays += 30; // month 5
        case 5:
            mdays += 29; // month 4
        case 4:
            mdays += 30; // month 3
        case 3:
            mdays += 29; // month 2
        case 2:
            mdays += 30; // month 1
        case 1:
            if( leap ) { // month 13
                mdays += 29;
            }
        case 13:
            if( leap ) { // month 12
                mdays += 30;
            } else {
                mdays += 29;
            }
        case 12:
            mdays += 30; // month 11
        case 11:
            mdays += 29; // month 10
        case 10:
            if( diy == 353 || diy == 383 ) { // month 9
                mdays += 29;
            } else {
                mdays += 30;
            }
        case 9:
            if( diy == 355 || diy == 385 ) { // month 8
                mdays += 30;
            } else {
                mdays += 29;
            }
        case 8:
            mdays += 30;  // month 7
        }
        return hebrew_new_year( year ) + mdays + day - 1;
    }

    /*! Splits the given Hebrew Day Number date into the day, month and year
     *  for the Hebrew Calendar.
     */
    void hebrew_from_jdn( Field* year, Field* month, Field* day, Field jdn )
    {
        Field y = floor_div( 4L * ( jdn - BASEDATE_Hebrew ), 1461 );
        for(;;) {
            if( hebrew_new_year( y ) > jdn ) {
                --y;
                break;
            }
            y++;
        }

        Field m = ( jdn < hebrew_to_jdn( y, 1, 1 ) ) ? 7 : 1;
        for(;;) {
            Field ldim = hebrew_last_day_in_month( y, m );
            if( jdn <= hebrew_to_jdn( y, m, ldim ) ) {
                break;
            }
            m++;
            assert( m <= 13 );
        }
        *day = jdn - hebrew_to_jdn( y, m, 1 ) + 1;
        *year = y;
        *month = m;
    }

} // namespace

//===================================

int Hebrew::get_fieldname_index( const string& fieldname ) const
{
    int ret = get_ymd_fieldname_index( fieldname );
    if( ret >= 0 ) {
        return ret;
    }
    if( fieldname == "wsday" ) { // 7 Day week Sun=1 (1 to 7)
        return HFN_wsday;
    }
    return -1;
}

string Hebrew::get_fieldname( size_t index ) const
{
    if( index < sizeof_ymd_fieldnames() ) {
        return get_ymd_fieldname( index );
    }
    switch( index )
    {
    case HFN_wsday:
        return "wsday";
    }
    return "";
}

Field Hebrew::get_jdn( const Field* fields ) const
{
    return hebrew_to_jdn( fields[0], fields[1], fields[2] );
}

Field Hebrew::get_extended_field( const Field* fields, Field jdn, size_t index ) const
{
    switch( index )
    {
    case HFN_wsday:
        return day_of_week( jdn + 1 ) + 1; // Sun=1, Sun=7
    }
    return f_invalid;
}

bool Hebrew::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? 7 : mask[1];
    fields[2] = ( mask[2] == f_invalid ) ? 1 : mask[2];
    return true;
}

bool Hebrew::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    if( mask[1] != f_invalid ) {
        fields[1] = mask[1];
    } else {
        fields[1] = 6;
    }
    if( mask[2] != f_invalid ) {
        fields[2] = mask[2];
    } else {
        fields[2] = hebrew_last_day_in_month( fields[0], fields[1] );
    }
    return true;
}

bool Hebrew::set_fields_as_next_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
	if( index == HFN_wsday ) {
		if( mask[index] >= 1 && mask[index] <= 7 && jdn != f_invalid ) {
            // Adjust jdn and knext for week starting Sunday 
			Field knext = kday_on_or_after( Weekday( mask[index] - 1 ), jdn + 1 ) - 1;
			if( knext != jdn ) {
				set_fields( fields, knext );
				return true;
			}
		}
	}
    return false;
}

bool Hebrew::set_fields_as_prev_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
	if( index == HFN_wsday ) {
		if( mask[index] >= 1 && mask[index] <= 7 && jdn != f_invalid ) {
            // Adjust jdn and knext for week starting Sunday 
			Field knext = kday_on_or_before( Weekday( mask[index] - 1 ), jdn + 1 ) - 1;
			if( knext != jdn ) {
				set_fields( fields, knext );
				return true;
			}
		}
    }
    return false;
}

void Hebrew::set_fields( Field* fields, Field jdn ) const
{
    hebrew_from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}

Field Hebrew::get_field_first( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // First month of year
        return 7;
    case 2: // First day of month
        return 1;
    }
    return f_invalid;
}

Field Hebrew::get_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // Last month of year
        return 6;
    case 2: // Last day of month
        return hebrew_last_day_in_month( fields[0], fields[1] );
    }
    return f_invalid;
}

double Hebrew::get_average_days( const Field* fields, Unit unit ) const
{
    const double lunar_month = 29.53059;
    const double tropical_year = 365.24219;

    switch( unit )
    {
    case unit_year:
        return tropical_year;
    case unit_month:
        return lunar_month;
    }
    // We only deal with non-integer units
    return 0.0;
}

// Add value * units to the Records fields. Value may be negative.
// Exactly what this means depends on the Calendar Scheme.
bool Hebrew::add_to_fields( Field* fields, Field value, Unit unit ) const
{
    switch( unit )
    {
        // TODO: we need to work out how to handle months 
    case unit_year:
        fields[0] += value;
        return true;
    }
    return false;
}

// Return true if value adjusted or false if no change.
// TODO: We need to normalise for leap months
bool Hebrew::normalise( Field* fields, Norm norm ) const
{
    bool change = false;
    // Normalises for days in month
    Field ldim = get_field_last( fields, 2 );
    if( fields[2] > ldim ) {
        switch( norm )
        {
        case norm_expand:
            fields[1]++;
            fields[2] = ldim - fields[2];
            change = true;
            break;
        case norm_expand_min:
            fields[1]++;
            fields[2] = 1;
            change = true;
            break;
        case norm_truncate:
            fields[2] = ldim;
            return true;
        }
        Field months = get_field_last( fields, 1 );
        if( fields[1] > months ) {
            fields[0]++;
            fields[1] -= months;
        }
    }
    return change;
}

// End of src/cal/calHebrew.cpp
