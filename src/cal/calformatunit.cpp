/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformatunit.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     FormatUnit class to read and write as units.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     4th April 2016
 * Copyright:   Copyright (c) 2016, Nick Matthews.
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

#include "calformatunit.h"

#include "calbase.h"
#include "calgrammar.h"
#include "calparse.h"
#include "calrecord.h"
#include "caltext.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;


string FormatUnit::get_output( const Record& record ) const
{
    string output;
    const Base* base = record.get_base();
    for( size_t i = 0 ; i < base->record_size() ; i++ ) {
        Field f = record.get_field( i );
        if( f != f_invalid ) {
            string fname = base->get_fieldname( i );
            string uname = get_owner()->get_unitname( fname );
            if( !output.empty() ) {
                output += " ";
            }
            output += field_to_str( f ) + uname;
        }
    }
    return output;
}

RangeList FormatUnit::string_to_rlist( Base* base, const string& input ) const
{
    RangeList rlist;
    Record rec1( base ), rec2( base );
    if( set_input( &rec1, input, RB_begin ) && set_input( &rec2, input, RB_end ) ) {
        Range range( rec1.get_jdn(), rec2.get_jdn() );
        if( range.jdn1 != f_invalid && range.jdn2 != f_invalid ) {
            rlist.push_back( range );
        }
    }
    return rlist;
}

// Ignores the boundary as we need a mask for multiple ranges.
bool FormatUnit::set_input( Record* record, const std::string& input, Boundary rb ) const
{
    record->clear_fields();
    enum step_t { START, VALUE, UNIT };
    step_t step = START;
    string value, unit;
    for( string::const_iterator it = input.begin() ; it != input.end() ; it++ ) {
        int ch = *it;
        if( u8_isspace( ch ) ) {
            continue;
        }
        if( step != VALUE && ( ch == '-' || u8_isdigit( ch ) ) ) {
            if( !value.empty() ) {
                set_field_by_unit( record, value, unit );
            }
            value.clear();
            unit.clear();
            step = VALUE;
            value += ch;
            continue;
        }
        if( step == VALUE ) {
            if( u8_isdigit( ch ) ) {
                value += ch;
                continue;
            }
            step = UNIT;
        }
        unit += ch;
    }
    set_field_by_unit( record, value, unit );
    return true;
}

void FormatUnit::set_field_by_unit( Record* record, const string& value, const string& unit ) const
{
    Field v = str_to_field( value );
    if( v == 0 ) {
        return;
    }
    Field m = 1;
    string fn = get_owner()->get_unit_fieldname( &m, unit );
    int i = record->get_base()->get_fieldname_index( fn );

    if( i < 0 ) {
        return;
    }
    record->set_field( m * v, i );
}



// End of src/cal/calformatunit.cpp file
