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
    if ( mask[YMD_year] == f_invalid || mask[YMD_month] == f_invalid || mask[YMD_day] == f_invalid ) {
        return false;
    }
    Field hist = mask[YMD_hist];
    if ( hist == f_invalid ) {
        hist_rel_year rel = GetHistRelation( mask );
        if ( rel == R_invalid ) {
            return false;
        }
        hist = mask[YMD_year];
        if ( rel == R_increase ) {
            hist++;
        }
    }
    FieldVec vec = fields_to_vec( mask );
    vec[YMD_year] = hist;
    if ( !Julian::set_fields_as_begin_first( fields, &vec[0] ) ) {
        return false;
    }
    fields[YMD_hist] = hist;
    fields[YMD_year] = mask[YMD_year];
    return true;
}

bool Easter::set_fields_as_next_first( Field* fields, const Field* mask ) const
{
    hist_rel_year rel = GetHistRelation( fields );
    if ( rel != R_ambiguous ) {
        return false;
    }
    if ( fields[YMD_year] == fields[YMD_hist] ) {
        fields[YMD_hist]++;
        return true;
    }
    return false;
}

bool Easter::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    if ( mask[YMD_year] == f_invalid || mask[YMD_month] == f_invalid || mask[YMD_day] == f_invalid ) {
        return false;
    }
    Field hist = mask[YMD_hist];
    if ( hist == f_invalid ) {
        hist_rel_year rel = GetHistRelation( mask );
        if ( rel == R_invalid ) {
            return false;
        }
        hist = mask[YMD_year];
        if ( rel == R_increase ) {
            hist++;
        }
    }
    FieldVec vec = fields_to_vec( mask );
    vec[YMD_year] = hist;
    if ( !Julian::set_fields_as_begin_last( fields, &vec[0] ) ) {
        return false;
    }
    fields[YMD_hist] = hist;
    fields[YMD_year] = mask[YMD_year];
    return true;
}

bool Easter::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    hist_rel_year rel = GetHistRelation( fields );
    if ( rel != R_ambiguous ) {
        return false;
    }
    if ( fields[YMD_year] == fields[YMD_hist] ) {
        fields[YMD_hist]++;
        return true;
    }
    return false;
}

Field Easter::get_jdn( const Field* fields ) const
{
    if ( fields[YMD_hist] != f_invalid ) {
        return jdn( fields[YMD_hist], fields[YMD_month], fields[YMD_day] );
    }
    hist_rel_year rel = GetHistRelation( fields );
    if ( rel == R_equal ) {
        return jdn( fields[YMD_year], fields[YMD_month], fields[YMD_day] );
    }
    if ( rel == R_increase ) {
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

Easter::hist_rel_year Easter::GetHistRelation( const Field* fields ) const
{
    Field d0 = jdn( fields[0], fields[1], fields[2] );
    if ( d0 == f_invalid ) {
        return R_invalid;
    }
    Field e0 = easter( fields[0] ) + m_offset;
    Field d1 = jdn( fields[0] + 1, fields[1], fields[2] );
    Field e1 = easter( fields[0] + 1 ) + m_offset;
    if ( d0 >= e0 && d1 >= e1 ) return R_equal;
    if ( d0 < e0 && d1 < e1 ) return R_increase;
    if ( d0 >= e0 && d1 < e1 ) return R_ambiguous;
    return R_invalid;
}

// End of src/cal/caleaster.cpp file
