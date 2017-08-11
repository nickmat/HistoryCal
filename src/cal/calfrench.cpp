/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calfrench.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     French Republican calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th December 2014
 * Copyright:   Copyright (c) 2014 ~ 2017, Nick Matthews.
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

    bool fnyoob_min_func( Field jdn, const void* data )
    {
        double constant = *static_cast<const double*>(data);
        return constant <= solar_longitude( midnight_in_paris( jdn ) );
    }

    Field french_new_year_on_or_before( Field jdn )
    {
        double approx =
            estimate_prior_solar_longitude( autumn, midnight_in_paris( jdn ) );
        double c = autumn;
        return min_search( (Field) approx - 1, fnyoob_min_func, &c );
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


OptFieldID French::get_opt_field_id( const std::string& fieldname ) const
{
    if( fieldname == "dday" ) { // Day within Decade (1 to 10)
        return OFID_fr_dday;
    }
    if( fieldname == "cday" ) { // Complementary day (1 to 6)
        return OFID_fr_cday;
    }
    if( fieldname == "nmonth" ) { // Named month (1 to 12)
        return OFID_fr_nmonth;
    }
    if( fieldname == "nmday" ) { // Named month day (1 to 30)
        return OFID_fr_nmday;
    }
    return Base::get_opt_field_id( fieldname );
}

std::string French::get_opt_fieldname( OptFieldID field_id ) const
{
    switch( field_id )
    {
    case OFID_fr_dday:
        return "dday";
    case OFID_fr_cday:
        return "cday";
    case OFID_fr_nmonth:
        return "nmonth";
    case OFID_fr_nmday:
        return "nmday";
    default:
        return Base::get_opt_fieldname( field_id );
    }
}

Field French::get_jdn( const Field* fields ) const
{
    return french_to_jdn( fields[0], fields[1], fields[2] );
}

Field French::get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const
{
    switch( id )
    {
    case OFID_fr_dday:
        return fields[1] == 13 ? f_invalid : ( ( fields[2] - 1 ) % 10 ) + 1;
    case OFID_fr_cday:
        return fields[1] == 13 ? fields[2] : f_invalid;
    case OFID_fr_nmonth:
        return fields[1] == 13 ? f_invalid : fields[1];
    case OFID_fr_nmday:
        return fields[1] == 13 ? f_invalid : fields[2];
    default:
        return Base::get_opt_field( fields, jdn, id );
    }
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
    if( mask[2] == f_invalid ) {
        fields[2] = french_last_day_in_month( fields[0], fields[1] );
        if( fields[2] == f_invalid ) {
            return false;
        }
    } else {
        fields[2] = mask[2];
    }
    return true;
}

// Return true if fields are changed, otherwise false.
bool French::set_fields_as_next_optional(
    Field* fields, Field jdn, const Field* mask, size_t index ) const
{
    assert( fields[YMD_year] != f_invalid );
    assert( fields[YMD_month] != f_invalid );
    assert( fields[YMD_day] != f_invalid );
    assert( jdn != f_invalid );
    if( index >= (extended_size() - opt_fields_size() ) ) {
        OptFieldID id = opt_index_to_id( index );
        switch( id )
        {
        case OFID_fr_nmonth:
            {
                Field nmonth = mask[index];
                if( nmonth != mask[YMD_month] ) {
                    if( nmonth >= 1 && nmonth <= 12 ) {
                        if( mask[index] < nmonth ) {
                            fields[YMD_year]++;
                        }
                        if( fields[YMD_month] < nmonth ) {
                            fields[YMD_day] = 1;
                        }
                        fields[YMD_month] = nmonth;
                    }
                    return true;
                }
            }
            break;
        case OFID_fr_nmday:
            {
                Field nmday = mask[index];
                if( nmday != fields[YMD_day] ) {
                    if( nmday > fields[YMD_day] ) {
                        if( fields[YMD_month] == 12 ) {
                            fields[YMD_month] = 1;
                            fields[YMD_year]++;
                        } else {
                            fields[YMD_month]++;
                        }
                    }
                    fields[YMD_day] = nmday;
                    return true;
                }
            }
            break;
        case OFID_fr_cday:  // Complementary day
            {
                Field cday = mask[index];
                if( fields[YMD_month] != 13 || cday != fields[YMD_day] ) {
                    if( cday < fields[YMD_day] ) {
                        fields[YMD_year]++;
                    }
                    fields[YMD_month] = 13;
                    fields[YMD_day] = cday;
                    return true;
                }
            }
            break;
        // TODO: extended for fields OFID_fr_decade and OFID_fr_dday
	    }
        return false;
    }
    return Base::set_fields_as_next_optional( fields, jdn, mask, index );
}

bool French::set_fields_as_prev_optional( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
    assert( fields[YMD_year] != f_invalid );
    assert( fields[YMD_month] != f_invalid );
    assert( fields[YMD_day] != f_invalid );
    assert( jdn != f_invalid );

    if( index >= (extended_size() - opt_fields_size() ) ) {
        OptFieldID id = opt_index_to_id( index );
        switch( id )
        {
        case OFID_fr_nmonth:
            {
                Field nmonth = mask[index];
                if( nmonth != mask[YMD_month] ) {
                    if( nmonth >= 1 && nmonth <= 12 ) {
                        if( mask[index] > nmonth ) {
                            --fields[YMD_year];
                        }
                        if( fields[YMD_month] > nmonth ) {
                            fields[YMD_day] = 30;
                        }
                        fields[YMD_month] = nmonth;
                    }
                    return true;
                }
            }
            break;
        case OFID_fr_nmday:
            {
                Field nmday = mask[index];
                if( nmday != fields[YMD_day] ) {
                    if( nmday < fields[YMD_day] ) {
                        if( fields[YMD_month] == 1 ) {
                            fields[YMD_month] = 12;
                            --fields[YMD_year];
                        } else {
                            --fields[YMD_month];
                        }
                    }
                    fields[YMD_day] = nmday;
                    return true;
                }
            }
            break;
        case OFID_fr_cday: // Complementary day
            {
                Field cday = mask[index];
                if( fields[YMD_month] != 13 || cday != fields[YMD_day] ) {
                    if( cday > fields[YMD_day] ) {
                        --fields[YMD_year];
                    }
                    fields[YMD_month] = 13;
                    fields[YMD_day] = cday;
                    return true;
                }
            }
            break;
        }
        // TODO: extended fields FEFN_decade and FEFN_dday
        return false;
    }
    return Base::set_fields_as_prev_optional( fields, jdn, mask, index );
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

Field Cal::French::get_opt_field_first( const Field * fields, Field jdn, OptFieldID id ) const
{
    switch ( id )
    {
    case OFID_fr_nmonth:
        return ( fields[YMD_month] == 13 ) ? f_invalid : 1;
    case OFID_fr_nmday:
        return ( fields[YMD_month] == 13 ) ? f_invalid : 1;
    case OFID_fr_cday:
        return ( fields[YMD_month] == 13 ) ? 1 : f_invalid;
    case OFID_fr_dday:
        return ( fields[YMD_month] == 13 ) ? f_invalid : 1;
    }
    return Base::get_opt_field_first( fields, jdn, id );
}

Field Cal::French::get_opt_field_last( const Field * fields, Field jdn, OptFieldID id ) const
{
    switch ( id )
    {
    case OFID_fr_nmonth:
        return ( fields[YMD_month] == 13 ) ? f_invalid : 13;
    case OFID_fr_nmday:
        return ( fields[YMD_month] == 13 ) ? f_invalid : 30;
    case OFID_fr_cday:
        return ( fields[YMD_month] == 13 ) ? french_last_day_in_month( fields[YMD_year], fields[YMD_month] ) : f_invalid;
    case OFID_fr_dday:
        return ( fields[YMD_month] == 13 ) ? f_invalid : 10;
    }
    return Base::get_opt_field_last( fields, jdn, id );
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

void French::resolve_opt_input( Field* fields, size_t index ) const
{
    assert( fields[index] != f_invalid );
    OptFieldID id = opt_index_to_id( index );
    switch( id )
    {
    case OFID_fr_nmonth:
        if( fields[YMD_month] == f_invalid ) {
            fields[YMD_month] = fields[index];
        }
        break;
    case OFID_fr_nmday:
        if( fields[YMD_day] == f_invalid ) {
            fields[YMD_day] = fields[index];
        }
        break;
    case OFID_fr_cday:
        if( fields[YMD_day] == f_invalid ) {
            fields[YMD_day] = fields[index];
            fields[YMD_month] = 13;
        }
        break;
    default:
        break;
    }
}

// End of src/cal/calfrench.cpp
