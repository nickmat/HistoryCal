/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calfrench.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     French Republican calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th December 2014
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
    int ret = get_ymd_fieldname_index( fieldname );
    if( ret >= 0 ) {
        return ret;
    }
    if( fieldname == "wday" ) { // 7 Day week Mon=1 (1 to 7)
        return FFN_wday;
    }
    if( fieldname == "dday" ) { // Day within Decade (1 to 10)
        return FFN_dday;
    }
    if( fieldname == "cday" ) { // Complementary day (1 to 6)
        return FFN_cday;
    }
    if( fieldname == "nmonth" ) { // Named month (1 to 12)
        return FFN_nmonth;
    }
    if( fieldname == "nmday" ) { // Named month day (1 to 30)
        return FFN_nmday;
    }
    return -1;
}

string French::get_fieldname( size_t index ) const
{
    if( index < sizeof_ymd_fieldnames() ) {
        return get_ymd_fieldname( index );
    }
    switch( index )
    {
    case FFN_wday:
        return "wday";
    case FFN_dday:
        return "dday";
    case FFN_cday:
        return "cday";
    case FFN_nmonth:
        return "nmonth";
    case FFN_nmday:
        return "nmday";
    }
    return "";
}

Field French::get_jdn( const Field* fields ) const
{
    return french_to_jdn( fields[0], fields[1], fields[2] );
}

Field French::get_extended_field( const Field* fields, Field jdn, size_t index ) const
{
    switch( index )
    {
    case FFN_wday:
        return day_of_week( jdn ) + 1; // Mon=1, Sun=7
    case FFN_dday:
        return fields[1] == 13 ? f_invalid : ( ( fields[2] - 1 ) % 10 ) + 1;
    case FFN_cday:
        return fields[1] == 13 ? fields[2] : f_invalid;
    case FFN_nmonth:
        return fields[1] == 13 ? f_invalid : fields[1];
    case FFN_nmday:
        return fields[1] == 13 ? f_invalid : fields[2];
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

bool French::set_fields_as_next_extended(
    Field* fields, Field jdn, const Field* mask, size_t index ) const
{
    assert( fields[FFN_year] != f_invalid );
    assert( fields[FFN_month] != f_invalid );
    assert( fields[FFN_year] != f_invalid );
    assert( jdn != f_invalid );
    if( index == FFN_nmonth ) {
        Field nmonth = mask[index];
        if( nmonth != mask[FFN_month] ) {
            if( nmonth >= 1 && nmonth <= 12 ) {
                if( mask[index] < nmonth ) {
                    fields[FFN_year]++;
                }
                if( fields[FFN_month] < nmonth ) {
                    fields[FFN_day] = 1;
                }
                fields[FFN_month] = nmonth;
            }
            return true;
        }
    } else if( index == FFN_nmday ) {
        Field nmday = mask[index];
        if( nmday != fields[FFN_day] ) {
            if( nmday > fields[FFN_day] ) {
                if( fields[FFN_month] == 12 ) {
                    fields[FFN_month] = 1;
                    fields[FFN_year]++;
                } else {
                    fields[FFN_month]++;
                }
            }
            fields[FFN_day] = nmday;
            return true;
        }
    } else if( index == FFN_cday ) { // Complementary day
        Field cday = mask[index];
        if( fields[FFN_month] != 13 || cday != fields[FFN_day] ) {
            if( cday < fields[FFN_day] ) {
                fields[FFN_year]++;
            }
            fields[FFN_month] = 13;
            fields[FFN_day] = cday;
            return true;
        }
    }
    // TODO: extended fields FEFN_decade and FEFN_dday
    return false;
}

bool French::set_fields_as_prev_extended( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
    assert( fields[FFN_year] != f_invalid );
    assert( fields[FFN_month] != f_invalid );
    assert( fields[FFN_year] != f_invalid );
    assert( jdn != f_invalid );
    if( index == FFN_nmonth ) {
        Field nmonth = mask[index];
        if( nmonth != mask[FFN_month] ) {
            if( nmonth >= 1 && nmonth <= 12 ) {
                if( mask[index] > nmonth ) {
                    --fields[FFN_year];
                }
                if( fields[FFN_month] > nmonth ) {
                    fields[FFN_day] = 30;
                }
                fields[FFN_month] = nmonth;
            }
            return true;
        }
    } else if( index == FFN_nmday ) {
        Field nmday = mask[index];
        if( nmday != fields[FFN_day] ) {
            if( nmday < fields[FFN_day] ) {
                if( fields[FFN_month] == 1 ) {
                    fields[FFN_month] = 12;
                    --fields[FFN_year];
                } else {
                    --fields[FFN_month];
                }
            }
            fields[FFN_day] = nmday;
            return true;
        }
    } else if( index == FFN_cday ) { // Complementary day
        Field cday = mask[index];
        if( fields[FFN_month] != 13 || cday != fields[FFN_day] ) {
            if( cday > fields[FFN_day] ) {
                --fields[FFN_year];
            }
            fields[FFN_month] = 13;
            fields[FFN_day] = cday;
            return true;
        }
    }
    // TODO: extended fields FEFN_decade and FEFN_dday
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

bool French::resolve_input( 
    Field* fields, const InputFieldVec& input, Format* fmt ) const
{
    bool ret = Base::resolve_input( fields, input, fmt );
    if( ret ) {
        if( fields[FFN_month] == f_invalid && fields[FFN_nmonth] != f_invalid ) {
            fields[FFN_month] = fields[FFN_nmonth];
        }
        if( fields[FFN_day] == f_invalid && fields[FFN_nmday] != f_invalid ) {
            fields[FFN_day] = fields[FFN_nmday];
        }
        if( fields[FFN_cday] != f_invalid && fields[FFN_day] == f_invalid ) {
            fields[FFN_day] = fields[FFN_cday];
            fields[FFN_month] = 13;
        }
    }
    return ret;
}

// End of src/cal/calfrench.cpp
