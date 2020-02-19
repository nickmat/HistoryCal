/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformat.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Format class to control the formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th November 2013
 * Copyright:   Copyright (c) 2013 ~ 2020, Nick Matthews.
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

#include "calformat.h"

#include "calbase.h"
#include "calgrammar.h"
#include "calparse.h"
#include "calrecord.h"
#include "caltext.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;

Format::Format( const std::string& code, Grammar* gmr ) 
    : m_code(code), m_owner(gmr), m_priority(gmr->next_format_priority()),
    m_style(FMT_STYLE_Default)
{
}

Format::~Format()
{
}

void Format::get_info( Format_info* info ) const
{
    info->code = m_code;
    info->gmr_code = m_owner->code();
    info->input_str = m_input_str;
    info->output_str = m_output_str;
    info->priority = m_priority;
    info->type = get_format_type();
    info->style = get_style();
}

string Format::rlist_to_string( const Base* base, const RangeList& ranges ) const
{
    string str;
    for( size_t i = 0 ; i < ranges.size() ; i++ ) {
        if( i > 0 ) {
            str += " | ";
        }
        str += range_to_string( base, ranges[i] );
    }
    return str;
}

string Format::range_to_string( const Base* base, Range range ) const
{
    if( range.jdn1 == range.jdn2 ) {
        return jdn_to_string( base, range.jdn1 );
    }
    return jdn_to_string( base, range.jdn1 ) + " ~ " + jdn_to_string( base, range.jdn2 );
}

string Format::jdn_to_string( const Base* base, Field jdn ) const
{
    if( jdn == f_minimum ) {
        return "past";
    }
    if( jdn == f_maximum ) {
        return "future";
    }
    Record rec( base, jdn );
    return get_output( rec );
}

// End of src/cal/calformat.cpp file
