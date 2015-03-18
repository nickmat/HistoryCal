/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calislamic.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Islamic Arithmetical calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th December 2014
 * Copyright:   Copyright (c) 2014 - 2015, Nick Matthews.
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

using namespace Cal;
using std::string;

#define BASEDATE_Islamic    1948440

Islamic::Islamic( const string& data )
    : m_tabtype(ITT_II), m_basedate(BASEDATE_Islamic)
{
    if( data == "Ia" ) {
        m_tabtype = ITT_I;
        m_basedate = BASEDATE_Islamic-1;
    } else if( data == "Ic" ) {
        m_tabtype = ITT_I;
        m_basedate = BASEDATE_Islamic;
    } else if( data == "IIa" ) {
        m_tabtype = ITT_I;
        m_basedate = BASEDATE_Islamic-1;
    } else if( data == "IIc" ) {
        m_tabtype = ITT_I;
        m_basedate = BASEDATE_Islamic;
    } else if( data == "IIIa" ) {
        m_tabtype = ITT_I;
        m_basedate = BASEDATE_Islamic-1;
    } else if( data == "IIIc" ) {
        m_tabtype = ITT_I;
        m_basedate = BASEDATE_Islamic;
    } else if( data == "IVa" ) {
        m_tabtype = ITT_I;
        m_basedate = BASEDATE_Islamic-1;
    } else if( data == "IVc" ) {
        m_tabtype = ITT_I;
        m_basedate = BASEDATE_Islamic;
    }
}

int Islamic::get_fieldname_index( const string& fieldname ) const
{
    // Base handles year, month, day.
    int ret = Base::get_fieldname_index( fieldname );
    if( ret >= 0 ) {
        return ret;
    }
    if( fieldname == "wsday" ) { // 7 Day week Sun=1 (1 to 7)
        return IFN_wsday;
    }
    return -1;
}

string Islamic::get_fieldname( size_t index ) const
{
    if( index < IFN_RCOUNT ) {
        // Base handles year, month, day.
        return Base::get_fieldname( index );
    }
    switch( index )
    {
    case IFN_wsday:
        return "wsday";
    }
    return "";
}

Field Islamic::get_jdn( const Field* fields ) const
{
    if( fields[IFN_year] == f_invalid ||
        fields[IFN_month] == f_invalid ||
        fields[IFN_day] == f_invalid
    ) {
        return f_invalid;
    }
    return to_jdn( fields[IFN_year], fields[IFN_month], fields[IFN_day] );
}

Field Islamic::get_extended_field( const Field* fields, Field jdn, size_t index ) const
{
    switch( index )
    {
    case IFN_wsday:
        return day_of_week( jdn + 1 ) + 1; // Sun=1, Sun=7
    }
    return f_invalid;
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
        last_day_in_month( fields[0], fields[1] ) : mask[2];
    return true;
}

bool Islamic::set_fields_as_next_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
	if( index == IFN_wsday ) {
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

bool Islamic::set_fields_as_prev_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
	if( index == IFN_wsday ) {
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

void Islamic::set_fields( Field* fields, Field jdn ) const
{
    from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}


Field Islamic::get_field_last( const Field* fields, size_t index ) const
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

//===================================

/*! Returns true if the year is a leap year in the Islamic Calendar.
 */
bool Islamic::is_leap_year( Field year ) const
{
    Field delta;
    switch( m_tabtype )
    {
    case ITT_I:
        delta = 15;
        break;
    case ITT_III:
        delta = 11;
        break;
    case ITT_IV:
        delta = 9;
        break;
    default: // ITT_II
        delta = 14;
        break;
    }
    return pos_mod( delta + 11 * year, 30 ) < 11;
}

/*! Returns the last day of the month for the given month and year
 *  in the Islamic Calendar.
 */
Field Islamic::last_day_in_month( Field year, Field month ) const
{
    if( month == 12 ) {
        return is_leap_year( year ) ? 30 : 29;
    } else {
        return (month % 2) == 0 ? 29 : 30;
    }
}

/*! Sets jdn to the Julian Day Number for the given day, month and year
 *  in the Islamic Calendar. Always returns true.
 */
Field Islamic::to_jdn( Field year, Field month, Field day ) const
{
    return
        m_basedate - 1
        + ( year - 1 ) * 354
        + floor_div( year * 11 + 3, 30 )
        + 29 * ( month - 1 )
        + floor_div( month, 2 ) + day;
}

/*! Splits the given Julian Day Number date into the day, month and year
 *  for the Islamic Calendar.
 */
bool Islamic::from_jdn( Field* year, Field* month, Field* day, Field jdn ) const
{
    *year = floor_div( 30 * ( jdn - m_basedate ) + 10646, 10631 );

    Field temp = to_jdn( *year, 1, 1 );
    *month = floor_div( 11 * ( jdn - temp ) + 330, 325 );

    temp = to_jdn( *year, *month, 1 );
    *day = 1 + jdn - temp;

    return true;
}


// End of src/cal/islamic.cpp
