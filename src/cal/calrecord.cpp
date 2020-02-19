/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calrecord.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Record class to hold date values.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
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

#include "calrecord.h"

#include "calbase.h"
#include "calformat.h"
#include "calgrammar.h"
#include "calgregorian.h"
#include "calparse.h"
#include "caltext.h"

#include <algorithm>
#include <cassert>

using namespace Cal;
using std::string;

Record::Record( const Base* base )
    : m_base(base), m_jdn(f_invalid), m_f(base->extended_size())
{
    clear_fields();
}

Record::Record( const Base* base, Field jdn )
    : m_base(base), m_jdn(jdn), m_f(base->extended_size())
{
    set_jdn( jdn );
}

Record::Record( const Base* base, const Field* fields, size_t size )
    : m_base(base), m_jdn(f_invalid), m_f(base->extended_size())
{
    set_fields( fields, size );
}

Record::Record( const Base* base, const string& str, const string& fcode, Boundary rb )
    : m_base(base), m_jdn(f_invalid), m_f(base->extended_size())
{
    set_str( str, fcode, rb );
}

Record::Record( const Record& rec )
    : m_base(rec.m_base), m_jdn(rec.m_jdn), m_f(rec.m_f)
{
}

void Record::set_jdn( Field jdn )
{
    clear_fields();
    if( jdn == f_minimum || jdn == f_maximum ) {
        m_f[0] = jdn;
    } else if( jdn != f_invalid ) {
        m_base->set_fields( &m_f[0], jdn );
    }
    m_jdn = jdn;
}

void Record::set_fields( const Field* fields, size_t size )
{
    for( size_t i = 0 ; i < m_f.size() ; i++ ) {
        if( i >= size ) {
            m_f[i] = f_invalid;
        } else {
            m_f[i] = fields[i];
        }
    }
    m_jdn = get_jdn();
}

void Record::set_str( const string& str, const string& fcode, Boundary rb )
{
    assert( !fcode.empty() );
    clear_fields();
    string in = full_trim( str );
    if( in == "past" ) {
        m_jdn = m_f[0] = f_minimum;
        return;
    }
    if( in == "future" ) {
        m_jdn = m_f[0] = f_maximum;
        return;
    }
    if( in == "today" ) {
        set_jdn( Gregorian::today() );
        return;
    }

    Format* fmt = m_base->get_format( fcode );
    if( fmt == nullptr ) {
        return;
    }

    if( fmt->set_input( this, in, rb ) ) {
        m_jdn = get_jdn();
    } else {
        clear_fields();
    }
}

bool Record::set_fields_as_begin_first( const Field* mask, bool check )
{
    if( m_f[0] == f_minimum ) {
        m_jdn = f_minimum;
        return true;
    }
    bool ret = m_base->set_fields_as_begin_first( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
        if( check ) {
            int attempt = 0;
            for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
                if( mask[i] == f_invalid ) {
                    continue;
                }
                // Adjust to match extended field
                if( m_base->set_fields_as_next_optional( &m_f[0], m_jdn, mask, i ) ) {
                    Field jdn = get_jdn();
                    if( m_jdn != jdn ) {
                        m_jdn = jdn;
                        if( attempt >= 3 ) {
                            return false; // Give up.
                        }
                        attempt++;
                        i = m_base->record_size() - 1; // Start again.
                    }
                }
            }
        }
    }
    return ret;
}

bool Record::set_fields_as_next_first( const Field* mask )
{
    bool ret = m_base->set_fields_as_next_first( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
    }
    return ret;
}

bool Record::correct_fields_as_first( const Field* mask )
{
    int attempt = 0;
    for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
        if( mask[i] == f_invalid ) {
            continue;
        }
        // Adjust to match extended field
        if( m_base->set_fields_as_next_optional( &m_f[0], m_jdn, mask, i ) ) {
            Field jdn = get_jdn();
            if( m_jdn != jdn ) {
                m_jdn = jdn;
                if( attempt >= 3 ) {
                    return false; // Give up.
                }
                attempt++;
                i = m_base->record_size() - 1; // Start again.
            }
        }
    }
    return true;
}

bool Record::set_fields_as_begin_last(  const Field* mask, bool check )
{
    if( m_f[0] == f_maximum ) {
        m_jdn = f_maximum;
        return true;
    }
    bool ret = m_base->set_fields_as_begin_last( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
        if( check ) {
            int attempt = 0;
            for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
                if( mask[i] == f_invalid ) {
                    continue;
                }
                // Adjust to match extended field
                if( m_base->set_fields_as_prev_optional( &m_f[0], m_jdn, mask, i ) ) {
                    Field jdn = get_jdn();
                    if( m_jdn != jdn ) {
                        m_jdn = jdn;
                        if( attempt >= 3 ) {
                            return false; // Give up.
                        }
                        attempt++;
                        i = m_base->record_size() - 1; // Start again.
                    }
                }
            }
        }
    }
    return ret;
}

bool Record::set_fields_as_next_last( const Field* mask )
{
    bool ret = m_base->set_fields_as_next_last( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
    }
    return ret;
}

bool Record::correct_fields_as_last( const Field* mask )
{
    int attempt = 0;
    for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
        if( mask[i] == f_invalid ) {
            continue;
        }
        // Adjust to match extended field
        if( m_base->set_fields_as_prev_optional( &m_f[0], m_jdn, mask, i ) ) {
            Field jdn = get_jdn();
            if( m_jdn != jdn ) {
                m_jdn = jdn;
                if( attempt >= 3 ) {
                    return false; // Give up.
                }
                attempt++;
                i = m_base->record_size() - 1; // Start again.
            }
        }
    }
    return true;
}

// Treat the record as a mask and generate rlist
RangeList Record::get_rlist_from_mask() const
{
    RangeList rlist;
    Range range;
    bool ret = set_range_as_begin( &range );
    while( range.jdn1 != f_invalid ) {
        if( ret ) {
            if( rlist.size() && rlist[rlist.size()-1].jdn2+1 >= range.jdn1 ) {
                rlist[rlist.size()-1].jdn2 = range.jdn2;
            } else {
                rlist.push_back( range );
            }
        }
        ret = set_range_as_next( &range );
    }
    return rlist;
}

// Sets range and returns true if the masks can create a valid range.
// Sets up range but returns false if the masks can create a valid range only
// by ignoring optional fields.
// Sets range to invalid and returns false if the masks cannot create a valid
// range.
bool Record::set_range_as_begin( Range* range ) const
{
    assert( range != NULL );
    Record rec1(m_base);
    Record rec2(m_base);
    bool ret1 = rec1.set_fields_as_begin_first( &m_f[0], false );
    bool ret2 = rec2.set_fields_as_begin_last( &m_f[0], false );
    if( !ret1 || !ret2 ) {
        range->jdn1 = f_invalid;
        return false;
    }
    range->jdn1 = rec1.get_jdn();
    range->jdn2 = rec2.get_jdn();
    if( range->jdn1 == f_invalid || range->jdn2 == f_invalid ) {
        range->jdn1 = f_invalid;
        return false;
    }
    // Adjust for extended fields restricting the range.
    ret1 = rec1.set_fields_as_begin_first( &m_f[0], true );
    ret2 = rec2.set_fields_as_begin_last( &m_f[0], true );
    if( ret1 && ret2 ) {
        Range r;
        r.jdn1 = rec1.get_jdn();
        r.jdn2 = rec2.get_jdn();
        if( r.jdn1 != range->jdn1 || r.jdn2 != range->jdn2 ) {
            if( r.jdn1 > r.jdn2 ) {
                return false;
            }
            *range = r;
            return true;
        }
    }
    return ret1 && ret2;
}

// Using the initial value of range, attempts to adjust the value of range
// to the next value.
// If it fails, it sets the range invalid and returns false.
// If it succeeds, it will attempt to correct for optional fields - if this
// fails, the uncorrected range is set and the the function returns false.
bool Record::set_range_as_next( Range* range ) const
{
    assert( range != NULL );
    Record rec1( m_base, range->jdn1 );
    Record rec2( m_base, range->jdn2 );
    bool ret1 = rec1.set_fields_as_next_first( &m_f[0] );
    bool ret2 = rec2.set_fields_as_next_last( &m_f[0] );
    if( !ret1 || !ret2 ) {
        range->jdn1 = f_invalid;
        return false;
    }
    range->jdn1 = rec1.get_jdn();
    range->jdn2 = rec2.get_jdn();
    if( range->jdn1 > range->jdn2 ||
        range->jdn1 == f_invalid || range->jdn2 == f_invalid )
    {
        range->jdn1 = f_invalid;
        return false;
    }
    ret1 = rec1.correct_fields_as_first( &m_f[0] );
    ret2 = rec2.correct_fields_as_last( &m_f[0] );
    if( ret1 && ret2 ) {
        Range r;
        r.jdn1 = rec1.get_jdn();
        r.jdn2 = rec2.get_jdn();
        if( r.jdn1 != range->jdn1 || r.jdn2 != range->jdn2 ) {
            if( r.jdn1 > r.jdn2 ) {
                return false;
            }
            *range = r;
            return true;
        }
    }
    return ret1 && ret2;
}

BoolVec Record::mark_balanced_fields( Record& rec, const XRefVec& rank )
{
    BoolVec mask( m_f.size(), true );
    // Both must have the same Base and not be identical.
    if ( m_base != rec.m_base || m_jdn == rec.get_jdn() ) {
        return mask;
    }
    if ( m_f[0] == f_minimum || rec.get_field( 0 ) == f_maximum ) {
        return m_base->mark_boundary_fields( &m_f[0], m_jdn, rec.get_field_ptr(), rec.get_jdn(), rank );
    }
    return m_base->mark_balanced_fields( &m_f[0], m_jdn, rec.get_field_ptr(), rec.get_jdn(), rank );
}

Field Record::get_jdn() const
{
    if( m_f[0] == f_minimum || m_f[0] == f_maximum ) {
        return m_f[0];
    }
    // Base should check or correct for valid content
    return m_base->get_jdn( &m_f[0] );
}

string Record::get_str( const string& fcode ) const
{
    assert( !fcode.empty() );
    Format* fmt = m_base->get_format( fcode );
    if( fmt == nullptr ) {
        return "";
    }
    return fmt->get_output( *this );
}

Field Record::is_unit_int( Unit unit ) const
{
    return m_base->unit_is_int( &m_f[0], unit );
}

bool Record::can_add_unit( Unit unit ) const
{
    return m_base->can_add_unit( &m_f[0], unit );
}

bool Record::add( Field value, Unit unit, Norm norm )
{
    bool ret = m_base->add_to_fields( &m_f[0], value, unit );
    if( ret ) {
        normalise( norm );
    }
    return ret;
}

bool Record::normalise( Norm norm )
{
    bool ret = m_base->normalise( &m_f[0], norm );
    if( ret ) {
        m_jdn = get_jdn();
    }
    return ret;
}

double Record::get_average_days( Unit unit ) const
{
    return m_base->get_average_days( &m_f[0], unit );
}

Field Record::get_field( int index, const BoolVec* mask ) const
{
    if ( index >= 0 && ( !mask || ( *mask )[index] ) ) {
        if ( index < (int)m_base->record_size() ) {
            return m_f[index];
        }
        return m_base->get_opt_field( &m_f[0], m_jdn, index );
    }
    return f_invalid;
}

Field Record::get_field( const std::string & fieldname ) const
{
    string fn = m_base->get_alias_fieldname( fieldname );
    int index = m_base->get_fieldname_index( fn );
    if ( index >= 0 ) {
        return get_field( index );
    }
    OptFieldID opt_id = m_base->get_opt_field_id( fn );
    return m_base->get_opt_field( &m_f[0], m_jdn, opt_id );
}

bool Record::is_mask_valid( Field* mask, size_t mask_size ) const
{
    size_t size = std::min( mask_size, m_base->extended_size() );
    for( size_t i = 0 ; i < size ; i++ ) {
        if( mask[i] != f_invalid && mask[i] != m_f[i] ) {
            return false;
        }
    }
    return true;
}

int Record::get_field_index( const string& fieldname ) const
{
    string fn = m_base->get_alias_fieldname( fieldname );
    return m_base->get_fieldname_index( fn );
}

int Record::get_unit_index( const string& unitname ) const
{
    string fn = m_base->get_alias_fieldname( unitname );
    return m_base->get_fieldname_index( fn );
}

void Record::clear_fields()
{
    // Clears the registers
    for( size_t i = 0 ; i < m_f.size() ; i++ ) {
        m_f[i] = f_invalid;
    }
    m_jdn = f_invalid;
}

// End of src/cal/calrecord.cpp file
