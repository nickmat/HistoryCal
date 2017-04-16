/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calchinese.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Chinese Lunisolar calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th June 2016
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

#include "calchinese.h"

#include "calastro.h"
#include "calmath.h"
#include "calrecord.h"

#include <cmath>
#include <cassert>

using namespace Cal;
using std::string;


#define BASEDATE_Chinese 758326L  // g# 15 Feb -2636

namespace {

    enum {
        CHIN_cycle, CHIN_year, CHIN_month, CHIN_lmonth, CHIN_day
    };

    // CC3 p248 chinese-location
    // A zone is expressed as the fraction of a day
    double chinese_zone( double moment )
    {
        // g# 1 Jan 1929 = 2425613
        double longitude = ( moment < 2425613 ) ? 116.41667 : 120.0;
        return zone_from_longitude( longitude );
    }

    // CC3 p248
    Field current_major_solar_term( Field jdn )
    {
        double s = 
            solar_longitude( universal_from_standard( jdn, chinese_zone( jdn ) ) );
        return amod_f( 2 + floor_f( s / 30 ), 12 );
    }

    // CC3 p251
    inline double midnight_in_china( Field jdn )
    {
        return universal_from_standard( jdn, chinese_zone( jdn ) );
    }

    bool chinese_winter_solstice_on_or_before_min_func( Field day, const void* data )
    {
        return winter < solar_longitude( midnight_in_china( day + 1 ) );
    }

    // CC3 p251
    Field chinese_winter_solstice_on_or_before( Field jdn )
    {
        double approx = estimate_prior_solar_longitude(
            winter, midnight_in_china( jdn + 1 ) );
        return min_search(
            floor_f( approx ) - 1, chinese_winter_solstice_on_or_before_min_func, NULL );
    }

    // CC3 p251
    Field chinese_new_moon_on_or_after( Field jdn )
    {
        double t = new_moon_at_or_after( midnight_in_china( jdn ) );
        return floor_f( standard_from_universal( t, chinese_zone( t ) ) );
    }

    // CC3 p252
    Field chinese_new_moon_before( Field jdn )
    {
        double t = new_moon_before( midnight_in_china( jdn ) );
        return floor_f( standard_from_universal( t, chinese_zone( t ) ) );
    }
    // CC3 p253 chinese-no-major-solar-term?
    bool chinese_is_no_major_solar_term( Field jdn )
    {
        return current_major_solar_term( jdn ) ==
            current_major_solar_term( chinese_new_moon_on_or_after( jdn + 1 ) );
    }

    // CC3 p255 chinese-prior-leap-month?
    bool chinese_is_prior_leap_month( Field m_dash, Field m )
    {
        return m >= m_dash && (
            chinese_is_no_major_solar_term( m ) ||
            chinese_is_prior_leap_month( m_dash, chinese_new_moon_before( m ) )
        );
    }

    // CC3 p257
    Field chinese_new_year_in_sui( Field jdn )
    {
        Field s1 = chinese_winter_solstice_on_or_before( jdn );
        Field s2 = chinese_winter_solstice_on_or_before( s1 + 370 );
        Field m12 = chinese_new_moon_on_or_after( s1 + 1 );
        Field m13 = chinese_new_moon_on_or_after( m12 + 1 );
        Field next_m11 = chinese_new_moon_before( s2 + 1 );

        if( round_f( double( next_m11 - m12 ) / mean_synodic_month ) == 12 &&
            ( 
                chinese_is_no_major_solar_term( m12 ) || 
                chinese_is_no_major_solar_term( m13 )
            )
        ) {
            return chinese_new_moon_on_or_after( m13 + 1 );
        }
        return m13;
    }

    // CC3 p258
    Field chinese_new_year_on_or_before( Field jdn )
    {
        Field newyear = chinese_new_year_in_sui( jdn );
        if( jdn >= newyear ) {
            return newyear;
        }
        return chinese_new_year_in_sui( jdn - 180 );
    }

    // CC3 p259 chinese-from-fixed
    bool chinese_from_jdn(
        Field* cycle, Field* year, Field* month, Field* lmonth, Field* day, Field jdn )
    {
        Field s1 = chinese_winter_solstice_on_or_before( jdn );
        Field s2 = chinese_winter_solstice_on_or_before( s1 + 370 );
        Field m12 = chinese_new_moon_on_or_after( s1 + 1 );
        Field nextm11 = chinese_new_moon_before( s2 + 1 );
        Field m = chinese_new_moon_before( jdn + 1 );
        bool leapyear =
            round_f( double( nextm11 - m12 ) / mean_synodic_month ) == 12;

        Field mon = round_f( double( m - m12 ) / mean_synodic_month );
        Field addm = 0;
        if( leapyear && chinese_is_prior_leap_month( m12, m ) ) {
            addm = 1;
        }
        *month = amod_f( mon - addm, 12 );

        *lmonth = 0;
        if( leapyear && 
            chinese_is_no_major_solar_term( m ) &&
            !chinese_is_prior_leap_month( m12, chinese_new_moon_before( m ) )
        ) {
            *lmonth = 1;
        }
        Field elapsed_years = floor_f( 1.5 - double( *month ) / 12.0
            + double( jdn - BASEDATE_Chinese ) / mean_tropical_year );
        *cycle = div_f( elapsed_years - 1, 60 ) + 1;
        *year = amod_f( elapsed_years, 60 );
        *day = jdn - m + 1;

        return true;
    }

    // CC3 p260 fixed-from-chinese
    Field chinese_to_jdn( Field cycle, Field year, Field month, Field lmonth, Field day )
    {
        Field midyear = BASEDATE_Chinese + 
            floor_f( ( double( (cycle-1) * 60 + year ) - 0.5 ) * mean_tropical_year );

        Field newyear = chinese_new_year_on_or_before( midyear );
        Field p = chinese_new_moon_on_or_after( newyear + ( month - 1 ) * 29 );

        Field d[5];
        bool ret = chinese_from_jdn( &d[0], &d[1], &d[2], &d[3], &d[4], p );
        if( !ret ) {
            return f_invalid;
        }

        Field prior_newmoon;
        if( month == d[CHIN_month] && lmonth == d[CHIN_lmonth] ) {
            prior_newmoon = p;
        } else {
            prior_newmoon = chinese_new_moon_on_or_after( p + 1 );
        }
        return prior_newmoon + day - 1;
    }

    Field chinese_last_day_of_month( Field cycle, Field year, Field month, Field leap )
    {
        Field jdn1 = chinese_to_jdn( cycle, year, month, leap, 1 );
        Field jdn2 = chinese_new_moon_on_or_after( jdn1 + 1 );
        return jdn2 - jdn1;
    }

    bool chinese_is_leap_month( Field cycle, Field year, Field month )
    {
        Field jdn1 = chinese_to_jdn( cycle, year, month, 0, 1 );
        Field m = month + 1;
        Field y = year;
        Field c = cycle;
        if( m == 13 ) {
            m = 1;
            y++;
            if( y == 61 ) {
                y = 1;
                c++;
            }
        }
        Field day = chinese_last_day_of_month( c, y, m, 0 );
        Field jdn2 = chinese_to_jdn( cycle, year, month, 0, day );
        return (jdn2 - jdn1) > 30;
    }

} // namespace


const char* Chinese::s_cymld_fieldnames[] = { "cycle", "year", "month", "lmonth", "day" };
size_t Chinese::s_sizeof_cymld_fieldnames = sizeof(s_cymld_fieldnames) / sizeof(const char*);

Field Chinese::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid ||
        fields[1] == f_invalid ||
        fields[2] == f_invalid ||
        fields[3] == f_invalid ||
        fields[4] == f_invalid
    ) {
        return f_invalid;
    }
    return chinese_to_jdn( fields[0], fields[1], fields[2], fields[3], fields[4] );
}

bool Chinese::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    if( mask[CHIN_cycle] == f_invalid ) {
        return false; // Must have at least cycle
    }
    fields[CHIN_cycle]  = mask[CHIN_cycle];
    fields[CHIN_year]   = ( mask[CHIN_year] == f_invalid )   ? 1 : mask[CHIN_year];
    fields[CHIN_month]  = ( mask[CHIN_month] == f_invalid )  ? 1 : mask[CHIN_month];
    fields[CHIN_lmonth] = ( mask[CHIN_lmonth] == f_invalid ) ? 0 : mask[CHIN_lmonth];
    fields[CHIN_day]    = ( mask[CHIN_day] == f_invalid )    ? 1 : mask[CHIN_day];
    return true;
}

bool Chinese::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    if( mask[CHIN_cycle] == f_invalid ) {
        return false; // Must have at least cycle
    }
    fields[CHIN_cycle] = mask[CHIN_cycle];
    fields[CHIN_year] = ( mask[CHIN_year] == f_invalid ) ? 60 : mask[CHIN_year];
    fields[CHIN_month] = ( mask[CHIN_month] == f_invalid ) ? 12 : mask[CHIN_month];
    if( mask[CHIN_lmonth] == f_invalid ) {
        fields[CHIN_lmonth] = chinese_is_leap_month(
            fields[CHIN_cycle], fields[CHIN_year], fields[CHIN_month] ) ? 1 : 0;
    } else {
        fields[CHIN_lmonth] = mask[CHIN_lmonth];
    }
    if( mask[CHIN_day] == f_invalid ) {
        fields[CHIN_day] = chinese_last_day_of_month( 
            fields[CHIN_cycle], fields[CHIN_year], fields[CHIN_month], fields[CHIN_lmonth] );
    } else {
        fields[CHIN_day] = mask[CHIN_day];
    }
    return true;
}

void Chinese::set_fields( Field* fields, Field jdn ) const
{
    chinese_from_jdn( &fields[CHIN_cycle], &fields[CHIN_year],
        &fields[CHIN_month], &fields[CHIN_lmonth], &fields[CHIN_day], jdn );
}

Field Chinese::get_field_first( const Field* fields, size_t index ) const
{
    if( index == CHIN_lmonth ) {
        return 0;
    }
    if( index > 0 && index < record_size() ) {
        return 1;
    }
    return f_invalid;
}

Field Chinese::get_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case CHIN_year:
        return 60;
    case CHIN_month:
        return 12;
    case CHIN_lmonth:
        return chinese_is_leap_month(
            fields[CHIN_cycle], fields[CHIN_year], fields[CHIN_month] ) ? 1 : 0;
    case CHIN_day:
        return chinese_last_day_of_month( fields[CHIN_cycle], fields[CHIN_year],
             fields[CHIN_month], fields[CHIN_lmonth] );
    }
    return f_invalid;
}

int Chinese::get_std_fieldname_index( const string& fieldname ) const
{
    for( size_t i = 0 ; i < s_sizeof_cymld_fieldnames ; i++ ) {
        if( fieldname == s_cymld_fieldnames[i] ) {
            return i;
        }
    }
    return -1;
}

string Chinese::get_std_fieldname( size_t index ) const
{
    if( index < s_sizeof_cymld_fieldnames ) {
        return s_cymld_fieldnames[index];
    }
    return "";
}


// End of src/cal/calfrench.cpp
