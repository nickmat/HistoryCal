/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calfrench.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     French Republican calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th December 2014
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

#include "calfrench.h"

#include "calastro.h"
#include "calbase.h"
#include "calmath.h"
#include "calrecord.h"

#include <cmath>
#include <cassert>

using namespace Cal;
using std::string;


#define BASEDATE_French 2375840L  // 22 Sep 1792 (G)

namespace {

    const double paris_longitude = 2.3375;

    double midnight_in_paris( Field jdn )
    {
        double jdnp1 = jdn + 1;
        double universalfromlocal = jdnp1 - paris_longitude / 360;
        double localfromapparent = jdnp1 - equation_of_time( universalfromlocal );
        return localfromapparent - paris_longitude / 360;
    }

    bool fnyoob_min_func( Field jdn, Field constant )
    {
        return constant <= solar_longtitude( midnight_in_paris( jdn ) );
    }

    Field french_new_year_on_or_before( Field jdn )
    {
        double approx =
            estimate_prior_solar_longitude( autumn, midnight_in_paris( jdn ) );
        return min_search( (Field) approx - 1, fnyoob_min_func, (Field) autumn );
    }

    Field french_last_day_in_month( Field year, Field month )
    {
        French base;
        if( month == 13 ) {
            Field r[] = { year+1, 1, 1 };
            Record rec( &base, &r[0], 3 );
            Field jdn = rec.get_jdn();
            rec.set_jdn( jdn-1 );
            return rec.get_field( 2 );
        } else {
            return 30;
        }
    }

    // CC3 p242
    Field french_to_jdn( Field year, Field month, Field day )
    {
        Field approx = BASEDATE_French + 180 + (Field) ( mean_tropical_year * ( year-1 ) );

        Field newyear = french_new_year_on_or_before( approx );

        return newyear - 1 + 30 * (month-1) + day;
    }

    // CC3 p242
    bool french_from_jdn( Field* year, Field* month, Field* day, Field jdn )
    {
        Field newyear = french_new_year_on_or_before( jdn );

        double y = ( (double) (newyear - BASEDATE_French) ) / mean_tropical_year;
        *year = (Field) floor( y + 1.5 );

        *month = ( jdn - newyear ) / 30 + 1;

        *day = ( jdn - newyear ) % 30 + 1;

        return true;
    }

} // namespace


int French::get_fieldname_index( const string& fieldname ) const
{
    // Base handles year, month, day.
    int ret = Base::get_fieldname_index( fieldname );
    if( ret >= 0 ) {
        return ret;
    }
    int offset = record_size();
    if( fieldname == "nmonth" ) { // Named month (1 to 12)
        return offset + FEFN_nmonth;
    }
    if( fieldname == "nmday" ) { // Named month day (1 to 30)
        return offset + FEFN_nmday;
    }
    if( fieldname == "decade" ) { // Decade within month (1 to 3)
        return offset + FEFN_decade;
    }
    if( fieldname == "dday" ) { // Day within Decade (1 to 10)
        return offset + FEFN_dday;
    }
    if( fieldname == "cday" ) { // Complementary day (1 to 6)
        return offset + FEFN_cday;
    }
    return -1;
}

string French::get_fieldname( size_t index ) const
{
    if( index < record_size() ) {
        // Base handles year, month, day.
        return Base::get_fieldname( index );
    }
    switch( index - record_size() )
    {
    case FEFN_nmonth:
        return "nmonth";
    case FEFN_nmday:
        return "nmday";
    case FEFN_decade:
        return "decade";
    case FEFN_dday:
        return "dday";
    case FEFN_cday:
        return "cday";
    }
    return "";
}

Field French::get_jdn( const Field* fields ) const
{
    return french_to_jdn( fields[0], fields[1], fields[2] );
}

Field French::get_extended_field( const Field* fields, Field jdn, size_t index ) const
{
    switch( index - record_size() )
    {
    case FEFN_nmonth:
        return fields[1] == 13 ? f_invalid : fields[1];
    case FEFN_nmday:
        return fields[1] == 13 ? f_invalid : fields[2];
    case FEFN_decade:
        return fields[1] == 13 ? f_invalid : ( ( fields[2] - 1 ) / 10 ) + 1;
    case FEFN_dday:
        return fields[1] == 13 ? f_invalid : ( ( fields[2] - 1 ) % 10 ) + 1;
    case FEFN_cday:
        return fields[1] == 13 ? fields[2] : f_invalid;
    }
    return f_invalid;
}

bool French::set_fields_as_begin_first( Field* fields, const Field* mask ) const
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

bool French::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? 13 : mask[1];
    fields[2] = ( mask[2] == f_invalid ) ? 
        french_last_day_in_month( fields[0], fields[1] ) : mask[2];
    return true;
}

bool French::set_fields_as_next_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
#if 0
    // Weekday is the only extended field checked by default
    size_t kindex = record_size() + JEFN_wday;
    if( mask[kindex] >= 1 && mask[kindex] <
    }
#endif= 7 && jdn != f_invalid ) {
        Field knext = kday_on_or_after( Weekday( mask[kindex] - 1 ), jdn );
        if( knext != jdn ) {
            set_fields( fields, knext );
            return true;
        }
    return false;
}

bool French::set_fields_as_prev_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
#if 0
    // Weekday is the only extended field checked by default
    size_t kindex = record_size() + JEFN_wday;
    if( mask[kindex] >= 1 && mask[kindex] <= 7 && jdn != f_invalid ) {
        Field knext = kday_on_or_before( Weekday( mask[kindex] - 1 ), jdn );
        if( knext != jdn ) {
            set_fields( fields, knext );
            return true;
        }
    }
#endif
    return false;
}

void French::set_fields( Field* fields, Field jdn ) const
{
    french_from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}

Field French::get_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // Last month of year
        return 13;
    case 2: // Last day of month
        return french_last_day_in_month( fields[0], fields[1] );
    }
    return f_invalid;
}

// Return true if value adjusted or false if no change.
bool French::normalise( Field* regs, Norm norm ) const
{
    // All months are 30 day except month 13 which is 5 or 6
    if( regs[1] == 13 ) {
        Field ldim = french_last_day_in_month( regs[0], 13 );
        if( regs[2] > ldim ) {
            switch( norm )
            {
            case norm_expand:
                regs[0]++; // into next year
                regs[1] = 1;
                regs[2] = 30 - ldim;
                return true;
            case norm_expand_min:
                regs[0]++; // into next year
                regs[1] = 1;
                regs[2] = 1;
                return true;
            case norm_truncate:
                regs[2] = ldim;
                return true;
            }
        }
    }
    return false;
}
// End of src/cal/calfrench.cpp
