/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calsvalue.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Class to hold all value types used by script.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     24th June 2014
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

#include "calsvalue.h"

#include "calsetmath.h"
#include "calstokenstream.h"
#include "caltext.h"

#include <cassert>

using namespace std;
using namespace Cal;

STokenStream* SValue::m_ts = NULL;

SValue::SValue( const SValue& value )
{
    m_type = value.m_type;
    switch( m_type )
    {
    case SVT_Error:
    case SVT_Str:
        m_str = value.m_str;
        break;
    case SVT_Bool:
    case SVT_Field:
    case SVT_Range:
        m_range = value.m_range;
        break;
    case SVT_Fields:
    case SVT_RList:
        m_rlist = value.m_rlist;
        break;
    }
}

void SValue::set_fields( const FieldVec& fields )
{
    m_type = SVT_Fields;
    for( size_t i = 0 ; i < fields.size() ; i++ ) {
        Range range( fields[i], f_invalid );
        m_rlist.push_back( range );
    }
}

void SValue::set_error( const string& str )
{
    m_type = SVT_Error;
    m_str = str;
    if( m_ts ) {
        m_ts->error( str );
    }
}

void SValue::set( const RangeList& rlist )
{
    if( rlist.size() == 0 ) {
        set_field( f_invalid );
    } else if( rlist.size() == 1 ) {
        Range range = rlist[0];
        if( range.jdn1 == range.jdn2 ) {
            set_field( range.jdn1 );
        } else {
            set_range( range );
        }
    } else {
        set_rlist( rlist );
    }
}

string SValue::get_str() const
{
    assert( m_type == SVT_Str || m_type == SVT_Error );
    return m_str;
}

bool SValue::get( string& str ) const
{
    str.clear();
    switch( m_type )
    {
    case SVT_Str:
        str = m_str;
        return true;
    case SVT_Bool:
        str = get_bool() ? "true" : "false";
        return true;
    case SVT_Field:
        str = field_to_str( m_range.jdn1 );
        return true;
    case SVT_Range:
        str = field_to_str( m_range.jdn1 );
        if( m_range.jdn1 != m_range.jdn2 ) {
            str += " ~ " + field_to_str( m_range.jdn2 );
        }
        return true;
    case SVT_RList:
        for( size_t i = 0 ; i < m_rlist.size() ; i++ ) {
            if( i > 0 ) {
                str += " | ";
            }
            str += field_to_str( m_rlist[i].jdn1 );
            if( m_rlist[i].jdn1 != m_rlist[i].jdn2 ) {
                str += " ~ " + field_to_str( m_rlist[i].jdn2 );
            }
        }
        return true;
    case SVT_Fields:
        str += "{";
        for( size_t i = 0 ; i < m_rlist.size() ; i++ ) {
            if( i > 0 ) {
                str += ", ";
            }
            if( m_rlist[i].jdn1 == f_invalid ) {
                str += "?";
            } else {
                str += field_to_str( m_rlist[i].jdn1 );
            }
        }
        str += "}";
        return true;
    }
    return false;
}

bool SValue::get( Field& field ) const
{
    switch( m_type )
    {
    case SVT_RList:
        if( m_rlist.size() == 1 && m_rlist[0].jdn1 == m_rlist[0].jdn2 ) {
            field = m_rlist[0].jdn1;
            return true;
        }
        return false;
    case SVT_Range:
        if( m_range.jdn1 != m_range.jdn2 ) {
            return false;
        }
        // Fall thru
    case SVT_Field:
        field = m_range.jdn1;
        return true;
    }
    return false;
}

bool SValue::get_bool() const
{
    assert( m_type == SVT_Bool );
    return m_range.jdn1 == 0 ? false : true;
}

Field SValue::get_field() const
{
    assert( m_type == SVT_Field );
    return m_range.jdn1;
}

Range SValue::get_range() const
{
    assert( m_type == SVT_Range );
    return m_range;
}

RangeList SValue::get_rlist() const
{
    assert( m_type == SVT_RList );
    return m_rlist;
}

FieldVec SValue::get_fields() const
{
    assert( m_type == SVT_Fields );
    FieldVec fields;
    for( size_t i = 0 ; i < m_rlist.size() ; i++ ) {
        fields[i] = m_rlist[i].jdn1;
    }
    return fields;
}

bool SValue::get_rlist( RangeList& rlist ) const
{
    switch( m_type )
    {
    case SVT_RList:
        rlist = m_rlist;
        return true;
    case SVT_Null:
        rlist.clear();
        return true;
    case SVT_Range:
        rlist.clear();
        if( m_range.jdn1 != f_invalid ) {
            rlist.push_back( m_range );
        }
        return true;
    case SVT_Field:
        rlist.clear();
        if( m_range.jdn1 != f_invalid ) {
            Range range( m_range.jdn1, m_range.jdn1 );
            rlist.push_back( range );
        }
        return true;
    }
    return false;
}

bool SValue::propagate_error( const SValue& value )
{
    if( is_error() ) {
        return true;
    }
    if( value.is_error() ) {
        set_error( value.get_str() );
        return true;
    }
    return false;
}

bool SValue::obtain_rlists( RangeList& left, RangeList& right, const SValue& value )
{
    if( propagate_error( value ) ) {
        return false;
    }
    if( !get_rlist( left ) ) {
        set_error( "Cannot convert left hand to RList." );
        return false;
    }
    if( !value.get_rlist( right ) ) {
        set_error( "Cannot convert right hand to RList." );
        return false;
    }
    return true;
}

void SValue::logical_or( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    set_bool( get_bool() || value.get_bool() );
}

void SValue::logical_and( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    set_bool( get_bool() && value.get_bool() );
}

void SValue::equal( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case SVT_Str:
            result = ( get_str() == value.get_str() );
            break;
        case SVT_Bool:
        case SVT_Field:
            result = ( get_field() == value.get_field() );
            break;
        case SVT_Range:
            {
                Range vr = value.get_range();
                Range tr = get_range();
                result =
                    ( tr.jdn1 == vr.jdn1 ) &&
                    ( tr.jdn2 == vr.jdn2 )
                ;
            }
            break;
        case SVT_RList:
            {
                RangeList vrl = value.get_rlist();
                RangeList trl = get_rlist();
                if( trl.size() == vrl.size() ) { 
                    result = true;
                    for( size_t i = 0 ; i < trl.size() ; i++ ) {
                        if( ( trl[i].jdn1 != vrl[i].jdn1 ) ||
                            ( trl[i].jdn2 != vrl[i].jdn2 )
                        ) {
                            result = false;
                            break;
                        }
                    }
                }
            }
            break;
        default:
            result = true;
        }
        set_bool( result );
        return;
    }
    set_error( "Must compare same types." );
}
    
void SValue::greater_than( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case SVT_Str:
            result = ( get_str().compare( value.get_str() ) > 0 );
            break;
        case SVT_Bool:
            set_error( "Can only compare bool for equal or not equal." );
            return;
        case SVT_Field:
            result = ( get_field() > value.get_field() );
            break;
        case SVT_Range:
        case SVT_RList:
            set_error( "Can only compare a Range for equal or not equal." );
            return;
        default:
            result = true;
        }
        set_bool( result );
        return;
    }
    set_error( "Must compare same types." );
}

void SValue::less_than( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case SVT_Str:
            result = ( get_str().compare( value.get_str() ) < 0 );
            break;
        case SVT_Bool:
            set_error( "Can only compare bool for equal or not equal." );
            return;
        case SVT_Field:
            result = ( get_field() < value.get_field() );
            break;
        case SVT_Range:
        case SVT_RList:
            set_error( "Can only compare a Range for equal or not equal." );
            return;
        default:
            result = true;
        }
        set_bool( result );
        return;
    }
    set_error( "Must compare same types." );
}

void SValue::plus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == SVT_Str || value.type() == SVT_Str ) {
        string str1, str2;
        bool success = get( str1 );
        if( success ) {
            success = value.get( str2 );
        }
        if( success ) {
            set_str( str1 + str2 );
            return;
        }
        set_error( "Unable to add string." );
        return;
    }
    if( type() == SVT_Field || value.type() == SVT_Field ) {
        if( type() == value.type() ) {
            set_field( add( get_field(), value.get_field() ) );
        } else if( type() == SVT_Range ) {
            set_range( add( get_range(), value.get_field() ) );
        } else if( value.type() == SVT_Range ) {
            set_range( add( value.get_range(), get_field() ) );
        } else if( type() == SVT_RList ) {
            set_rlist( add( get_rlist(), value.get_field() ) );
        } else if( value.type() == SVT_RList ) {
            set_rlist( add( value.get_rlist(), get_field() ) );
        } else {
            set_error( "Unable to add number." );
        }
        return;
    }
    if( type() == SVT_Range || value.type() == SVT_Range ) {
        if( type() == value.type() ) {
            set_range( add( get_range(), value.get_range() ) );
        } else if( type() == SVT_RList ) {
            set_rlist( add( get_rlist(), value.get_range() ) );
        } else if( value.type() == SVT_RList ) {
            set_rlist( add( value.get_rlist(), get_range() ) );
        } else {
            set_error( "Unable to add range." );
        }
        return;
    }
    set_error( "Not able to add types." );
}

void SValue::minus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == SVT_Field || value.type() == SVT_Field || 
        type() == SVT_Range || value.type() == SVT_Range
    ) {
        SValue v( value );
        v.negate();
        plus( v );
        return;
    }
    set_error( "Not able to subtract types." );
}

void SValue::multiply( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    // TODO: check for f_invalid etc and for overflow.
    if( m_type == value.type() ) {
        switch( m_type )
        {
        case SVT_Field:
            set_field( multiply( get_field(), value.get_field() ) );
            return;
        default:
           set_error( "Can only multiply numbers." );
           return;
        }
    }
    set_error( "Must subtract the same types." );
}

void SValue::divide( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    // TODO: check for f_invalid etc and for overflow.
    if( m_type == value.type() ) {
        switch( m_type )
        {
        case SVT_Field:
            {
                Field right = value.get_field();
                if( right == 0 ) {
                    set_error( "Division by zero." );
                    return;
                }
                set_field( divide( get_field(), value.get_field() ) );
            }
            return;
        default:
           set_error( "Can only divide numbers." );
           return;
        }
    }
    set_error( "Must divide the same types." );
}

void SValue::rlist_union( const SValue& value )
{
    RangeList left, right;
    if( obtain_rlists( left, right, value ) ) {
        set_rlist( op_set_union( left, right ) );
    }
}

void SValue::intersection( const SValue& value )
{
    RangeList left, right;
    if( obtain_rlists( left, right, value ) ) {
        set_rlist( op_set_intersection( left, right ) );
    }
}

void SValue::rel_complement( const SValue& value )
{
    RangeList left, right;
    if( obtain_rlists( left, right, value ) ) {
        set_rlist( op_set_rel_complement( left, right ) );
    }
}

void SValue::sym_difference( const SValue& value )
{
    RangeList left, right;
    if( obtain_rlists( left, right, value ) ) {
        set_rlist( op_set_sym_difference( left, right ) );
    }
}

void SValue::range_op( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == SVT_Field || m_type == SVT_Range ) {
        Range range( get_field(), value.get_field() );
        set_range( range );
        return;
    }
    set_error( "Can only set range with numbers." );
}

void SValue::negate()
{
    if( is_error() ) {
        return;
    }
    switch( m_type )
    {
    case SVT_Field:
        if( m_range.jdn1 != f_invalid ) {
            m_range.jdn1 = add( -m_range.jdn1, 0 );
        }
        return;
    }
    set_error( "Can only negate numbers." );
}

void SValue::logical_not()
{
    if( is_error() ) {
        return;
    }
    set_bool( !get_bool() );
}

void SValue::compliment()
{
    if( is_error() ) {
        return;
    }
    RangeList rlist;
    if( get_rlist( rlist ) ) {
        set_rlist( op_set_complement( rlist ) );
        return;
    }
    set_error( "Cannot convert to RList." );
}

STokenStream* SValue::set_token_stream( STokenStream* ts )
{
    STokenStream* old = m_ts;
    m_ts = ts;
    return old;
}


Field SValue::add( Field left, Field right ) const
{
    // Checks for f_invalid and for overflow.
    if( left == f_invalid || right == f_invalid ) {
        return f_invalid;
    }
    if( left == f_minimum || right == f_minimum ) {
        if( left == f_maximum || right == f_maximum ) {
            return f_invalid; // past + future = invalid
        }
        return f_minimum;
    }
    if( left == f_maximum || right == f_maximum ) {
        return f_maximum;
    }

    LongField lf = (LongField) left + (LongField) right;
    if( lf < (LongField) f_minimum ) {
        return f_minimum;
    }
    if( lf > (LongField) f_maximum ) {
        return f_maximum;
    }
    return (Field) lf;
}

Range SValue::add( Range range, Field field ) const
{
    return Range( add( range.jdn1, field ), add( range.jdn2, field ) );
}

Range SValue::add( Range left, Range right ) const
{
    return Range( 
        add( left.jdn1, right.jdn1 ),
        add( left.jdn2, right.jdn2 )
    );
}

RangeList SValue::add( RangeList rlist, Field field ) const
{
    RangeList rl;
    for( size_t i = 0 ; i < rlist.size() ; i++ ) {
        Range range = add( rlist[i], field );
        rl.push_back( range );
    }
    return rl;
}

RangeList SValue::add( RangeList rlist, Range range ) const
{
    RangeList rl;
    for( size_t i = 0 ; i < rlist.size() ; i++ ) {
        Range r = add( rlist[i], range );
        rl.push_back( r );
    }
    return rl;
}

Field SValue::multiply( Field left, Field right ) const
{
    // Checks for f_invalid and for overflow.
    if( left == f_invalid || right == f_invalid ) {
        return f_invalid;
    }
    if( left == f_minimum || right == f_minimum ) {
        if( left == f_maximum || right == f_maximum ) {
            return f_invalid; // past * future = invalid
        }
        return f_minimum;
    }
    if( left == f_maximum || right == f_maximum ) {
        return f_maximum;
    }

    LongField lf = (LongField) left * (LongField) right;
    if( lf < (LongField) f_minimum ) {
        return f_minimum;
    }
    if( lf > (LongField) f_maximum ) {
        return f_maximum;
    }
    return (Field) lf;
}

Field SValue::divide( Field left, Field right ) const
{
    // Checks for f_invalid and for overflow.
    if( left == f_invalid || right == f_invalid ) {
        return f_invalid;
    }
    if( left == f_minimum || right == f_minimum ) {
        if( left == f_maximum || right == f_maximum ) {
            return f_invalid; // past / future = invalid
        }
        return f_minimum;
    }
    if( left == f_maximum || right == f_maximum ) {
        return f_maximum;
    }
    if( right == 0 ) {
        // This should really be checked before reaching this far.
        return f_invalid;
    }
    return left / right; // Can't overflow
}

// End of src/cal/calsvalue.cpp file
