/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calcyclic.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base for cyclic calendars implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th October 2013
 * Copyright:   Copyright (c) 2013-2014, Nick Matthews.
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

#include "calcyclic.h"

#include "calmath.h"

#include <sstream>

using namespace Cal;
using namespace std;

Cyclic::Cyclic( const std::string& data )
    : m_start(0), m_cycle(7), m_first(1), Base()
{
    stringstream def( data + " ;" );
    string word;
    def >> word;
    if( word == ";" ) return;
    m_start = strtol( word.c_str(), NULL, 10 );
    def >> word;
    if( word == ";" ) return;
    m_cycle = strtol( word.c_str(), NULL, 10 );
    def >> word;
    if( word == ";" ) return;
    m_first = strtol( word.c_str(), NULL, 10 );
}

Cyclic::~Cyclic()
{
}

int Cyclic::get_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "count" ) {
        return 0;
    } else if( fieldname == "day" ) {
        return 1;
    }
    return -1;
}

string Cyclic::get_fieldname( size_t index ) const
{
    const char* fnames[] = { "count", "day" };
    if( index < 2 ) {
        return fnames[index];
    }
    return "";
}

Field Cyclic::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid || fields[1] == f_invalid ) {
        return f_invalid;
    }
    return fields[0] * m_cycle + fields[1] - 1 + m_start;
}

bool Cyclic::set_fields_as_begin_first( Field* fields, const Field* mask )
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? m_first : mask[1];
    return true;
}

bool Cyclic::set_fields_as_next_first( Field* fields, const Field* mask )
{
    return false;
}

bool Cyclic::set_fields_as_begin_last( Field* fields, const Field* mask )
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? m_cycle : mask[1];
    return true;
}

bool Cyclic::set_fields_as_next_last( Field* fields, const Field* mask )
{
    return false;
}

void Cyclic::remove_fields_if_first( Field* fields ) const
{
    if( fields[1] == 1 ) {
        fields[1] = f_invalid;
    }
}

void Cyclic::remove_fields_if_last( Field* fields ) const
{
    if( fields[1] == m_cycle ) {
        fields[1] = f_invalid;
    }
}

void Cyclic::set_fields( Field* fields, Field jdn ) const
{
    fields[0] = floor_div( jdn + m_start, m_cycle );
    fields[1] = pos_mod( jdn + m_start, m_cycle ) + 1;
}


// End of src/cal/calcyclic.cpp file
