/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caleaster.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Julian Easter calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     16th June 2019
 * Copyright:   Copyright (c) 2019, Nick Matthews.
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

#include "caleaster.h"

#include "calparse.h"
#include "calrecord.h"

#include <cassert>

using namespace Cal;
using std::string;


Cal::Easter::Easter( const string& data )
    : m_offset(0), Julian(data)
{
}

void Cal::Easter::set_data( const string& data )
{
    string code, tail;
    split_code( &code, &tail, data );
    if ( code == "offset" ) {
        m_offset = str_to_field( tail );
    }
}

bool Easter::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    Field eyear = mask[YMD_year];
    Field month = mask[YMD_month];
    Field day = mask[YMD_day];
    Field hyear = mask[YMD_hist];
    unsigned bm = get_valid_field_bitmap( mask );
    if ( bm == 0b1100 ) { // Valid eyear and month
        day = 1;
        Field e0 = get_eyear_start( eyear );
        Record rec( this, e0 );
        Field emonth = rec.get_field_at( YMD_month );
        if ( month == emonth ) {
            day = rec.get_field_at( YMD_day );
        } else if ( month < emonth ) {
            Field e1 = get_eyear_end( eyear );
            rec.set_jdn( e1 );
            emonth = rec.get_field_at( YMD_month );
            if ( month > emonth ) {
                return false;
            }
        }
        bm = 0b1110;
    }

    if ( bm == 0b1110 ) { // Valid eyear, month and day
        hyear = eyear;
        EYMD_Classified eymd = get_ceymd_classified( eyear, month, day );
        if ( eymd == EYMDC_invalid ) {
            return false;
        }
        if ( eymd == EYMDC_unique1 ) {
            hyear++;
        }
    }

    if ( eyear == f_invalid || month == f_invalid || day == f_invalid || hyear == f_invalid ) {
        return false;
    }
    fields[YMD_year] = eyear;
    fields[YMD_month] = month;
    fields[YMD_day] = day;
    fields[YMD_hist] = hyear;
    return true;
}

bool Easter::set_fields_as_next_first( Field* fields, const Field* mask ) const
{
    assert( fields[YMD_year] != f_invalid && fields[YMD_month] != f_invalid &&
        fields[YMD_day] != f_invalid && fields[YMD_hist] != f_invalid );

    if ( fields[YMD_year] != fields[YMD_hist] ) {
        return false;
    }
    unsigned bm = get_valid_field_bitmap( mask );
    if ( bm == 0b1100 ) {  // YM??
        Field jdn = julian_to_jdn( fields[YMD_year] + 1, fields[YMD_month], 1 );
        if ( jdn < easter( fields[YMD_year] + 1 ) ) {
            fields[YMD_hist] = fields[YMD_year] + 1;
            fields[YMD_day] = 1;
            return true;
        }
    }
    if ( bm == 0b1110 ) {
        EYMD_Classified rel = get_ceymd_classified( fields );
        if ( rel == EYMDC_both ) {
            fields[YMD_hist] = fields[YMD_year] + 1;
            return true;
        }
    }
    return false;
}

bool Easter::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    Field eyear = mask[YMD_year];
    Field month = mask[YMD_month];
    Field day = mask[YMD_day];
    Field hyear = mask[YMD_hist];
    unsigned bm = get_valid_field_bitmap( mask );
    if ( bm == 0b1100 ) { // Valid eyear and month
        day = last_day_in_month( eyear, month );
        Field e0 = get_eyear_start( eyear );
        Record rec( this, e0 );
        Field emonth = rec.get_field_at( YMD_month );
        if ( month < emonth ) {
            Field e1 = get_eyear_end( eyear );
            rec.set_jdn( e1 );
            emonth = rec.get_field_at( YMD_month );
            if ( month == emonth ) {
                day = rec.get_field_at( YMD_day );
            } else if ( month > emonth ) {
                return false;
            } else {
                day = last_day_in_month( eyear + 1, month );
            }
        }
        bm = 0b1110;
    }

    if ( bm == 0b1110 ) { // Valid eyear, month and day
        hyear = eyear;
        EYMD_Classified eymd = get_ceymd_classified( eyear, month, day );
        if ( eymd == EYMDC_invalid ) {
            return false;
        }
        if ( eymd == EYMDC_unique1 ) {
            hyear++;
        }
    }

    if ( eyear == f_invalid || month == f_invalid || day == f_invalid || hyear == f_invalid ) {
        return false;
    }
    fields[YMD_year] = eyear;
    fields[YMD_month] = month;
    fields[YMD_day] = day;
    fields[YMD_hist] = hyear;
    return true;
}

bool Easter::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    assert( fields[YMD_year] != f_invalid && fields[YMD_month] != f_invalid &&
        fields[YMD_day] != f_invalid && fields[YMD_hist] != f_invalid );

    if ( fields[YMD_year] != fields[YMD_hist] ) {
        return false;
    }
    unsigned bm = get_valid_field_bitmap( mask );
    if ( bm == 0b1100 ) {  // YM??
        Field jdn = julian_to_jdn( fields[YMD_year] + 1, fields[YMD_month], 1 );
        Field e = get_eyear_end( fields[YMD_year] );
        if ( jdn < e ) {
            fields[YMD_hist] = fields[YMD_year] + 1;
            Record rec( this, e );
            if ( rec.get_field_at( YMD_month ) == mask[YMD_month] ) {
                fields[YMD_day] = rec.get_field_at( YMD_day );
            } else {
                fields[YMD_day] = last_day_in_month( fields[YMD_hist], fields[YMD_month] );
            }
            return true;
        }
    }
    if ( bm == 0b1110 ) {  // YMD?
        EYMD_Classified rel = get_ceymd_classified( fields );
        if ( rel == EYMDC_both ) {
            fields[YMD_hist] = fields[YMD_year] + 1;
            return true;
        }
    }
    return false;
}

Field Easter::get_jdn( const Field* fields ) const
{
    if ( fields[YMD_hist] != f_invalid ) {
        return jdn( fields[YMD_hist], fields[YMD_month], fields[YMD_day] );
    }
    EYMD_Classified rel = get_ceymd_classified( fields );
    if ( rel == EYMDC_unique0 ) {
        return jdn( fields[YMD_year], fields[YMD_month], fields[YMD_day] );
    }
    if ( rel == EYMDC_unique1 ) {
        return jdn( fields[YMD_year] + 1, fields[YMD_month], fields[YMD_day] );
    }
    return f_invalid;
}

void Easter::set_fields( Field* fields, Field jdn ) const
{
    julian_from_jdn( &fields[YMD_hist], &fields[YMD_month], &fields[YMD_day], jdn );
    Field year = fields[YMD_hist];
    Field e = easter( year ) + m_offset;
    if ( jdn < e ) {
        --year;
    }
    fields[YMD_year] = year;
}

int Easter::get_std_fieldname_index( const string& fieldname ) const
{
    if ( fieldname == "hist" ) {
        return YMD_hist;
    }
    return get_ymd_fieldname_index( fieldname );
}

string Easter::get_std_fieldname( size_t index ) const
{
    if ( index == YMD_hist ) {
        return "hist";
    }
    return get_ymd_fieldname( index );
}

Easter::EYMD_Classified Easter::get_ceymd_classified( Field eyear, Field month, Field day ) const
{
    if ( eyear == f_invalid || month == f_invalid || day == f_invalid ) {
        return EYMDC_invalid;
    }
    Field d0 = Julian::jdn( eyear, month, day );
    d0 += m_offset;
    Field d1 = Julian::jdn( eyear + 1, month, day );
    d1 += m_offset;
    Field e0 = get_eyear_start( eyear );
    Field e1 = get_eyear_end( eyear );
    assert( e0 != f_invalid && e1 != f_invalid );
    // The easter year is the range e0 ~ (e1-1). 
    // If both d0 and d1 are outside that range, it is an invalid date.
    if ( d0 < e0 && d1 > e1 ) {
        return EYMDC_invalid;
    }
    // If both d0 and d1 are inside the easter year range, it is ambiguous.
    if ( d0 >= e0 && d1 <= e1 ) {
        return EYMDC_both;
    }
    // It is either unique0 (eyear == hyear) or unique1 (eyear+1 == hyear).
    if ( d0 < e0 ) {
        return EYMDC_unique1;
    }
    return EYMDC_unique0;
}

Field Easter::get_eyear_start( Field eyear ) const
{
    Field e = easter( eyear );
    if ( e != f_invalid ) {
        e += m_offset;
    }
    return e;
}

// End of src/cal/caleaster.cpp file
