/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calsvalue.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Class to hold all value types used by script.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     24th June 2014
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

#include "calsvalue.h"

#include "calmath.h"
#include "calsetmath.h"
#include "calstokenstream.h"
#include "caltext.h"

#include <algorithm>
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
    case SVT_Record:
        m_str = value.m_str;
        // Fall thru.
    case SVT_RList:
        m_rlist = value.m_rlist;
        break;
    }
}

void SValue::set_record( const string& scode, const FieldVec& fields )
{
    m_type = SVT_Record;
    m_str = scode;
    m_rlist.clear();
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
    assert( m_type == SVT_Str || m_type == SVT_Record || m_type == SVT_Error );
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
        str = field_to_str( m_range.jdn1, SI_qmark );
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
    case SVT_Record:
        str += "{\"" + m_str + "\"";
        for( size_t i = 0 ; i < m_rlist.size() ; i++ ) {
            str += ", ";
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

bool SValue::get( bool& flag ) const
{
    switch( m_type )
    {
    case SVT_Bool:
        flag = m_range.jdn1 == 0 ? false : true;
        return true;
    default:
        return false;
    }
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

FieldVec SValue::get_record() const
{
    assert( m_type == SVT_Record );
    FieldVec fields;
    for( size_t i = 0 ; i < m_rlist.size() ; i++ ) {
        fields.push_back( m_rlist[i].jdn1 );
    }
    return fields;
}

string SValue::get_record_scode() const
{
    assert( m_type == SVT_Record );
    return m_str;
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
    if( type() == SVT_Record && value.type() == SVT_Record ) {
        set_record( m_str, add( get_record(), value.get_record() ) );
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
    Range lhs, rhs;
    const char* type_err_mess = "Range operator requires number types.";
    const char* empty_err_mess = "Range operator cannot use empty rlist.";
    const char* invalid_err_mess = "Range invalid.";
    switch( m_type )
    {
    case SVT_RList:
        if( m_rlist.empty() ) {
            set_error( empty_err_mess );
            return;
        }
        lhs = enclosing_range( m_rlist );
        break;
    case SVT_Field:
        lhs.jdn1 = lhs.jdn2 = m_range.jdn1;
        break;
    case SVT_Range:
        lhs = m_range;
        break;
    default:
        set_error( type_err_mess );
        return;
    }
    switch( value.m_type )
    {
    case SVT_RList:
        if( value.m_rlist.empty() ) {
            set_error( empty_err_mess );
            return;
        }
        rhs = enclosing_range( value.m_rlist );
        break;
    case SVT_Field:
        rhs.jdn1 = rhs.jdn2 = value.m_range.jdn1;
        break;
    case SVT_Range:
        rhs = value.m_range;
        break;
    default:
        set_error( type_err_mess );
        return;
    }
    if( lhs.jdn1 > rhs.jdn1 ) {
        lhs.jdn1 = rhs.jdn1;
    }
    if( lhs.jdn2 < rhs.jdn2 ) {
        lhs.jdn2 = rhs.jdn2;
    }
    if( lhs.jdn1 == f_invalid ) {
        set_error( invalid_err_mess );
        return;
    }
    set_range( lhs );
}

void SValue::subscript_op( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == SVT_RList || m_type == SVT_Record ) {
        Field index = value.get_field();
        if( index < 0 || index >= (Field) m_rlist.size() ) {
            set_error( "Subscript out of range." );
        }
        if( m_type == SVT_RList ) {
            set_range( m_rlist[index] );
        } else {
            set_field( m_rlist[index].jdn1 );
        }
        m_rlist.clear();
        return;
    }
    set_error( "Unable to use subscript." );
}

void SValue::negate()
{
    switch( m_type )
    {
    case SVT_Error:
        return;
    case SVT_Field:
        if( m_range.jdn1 != f_invalid ) {
            m_range.jdn1 = -m_range.jdn1;
        }
        return;
    case SVT_Range:
        if( m_range.jdn1 != f_invalid && m_range.jdn2 != f_invalid ) {
            Field jdn = -m_range.jdn1;
            m_range.jdn1 = -m_range.jdn2;
            m_range.jdn2 = jdn;
        }
        return;
    case SVT_RList:
        for( size_t i = 0 ; i < m_rlist.size() ; i++ ) {
            if( m_rlist[i].jdn1 != f_invalid && m_rlist[i].jdn2 != f_invalid ) {
                Field jdn = -m_rlist[i].jdn1;
                m_rlist[i].jdn1 = -m_rlist[i].jdn2;
                m_rlist[i].jdn2 = jdn;
            }
        }
        std::reverse( m_rlist.begin(), m_rlist.end() );
        return;
    }
    set_error( "Can only negate numbers types." );
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

FieldVec SValue::add( const FieldVec& left, const FieldVec& right ) const
{
    FieldVec v;
    for( size_t i = 0 ; i < left.size() || i < right.size() ; i++ ) {
        if( i >= left.size() ) {
            v.push_back( right[i] );
            continue;
        }
        if( i >= right.size() ) {
            v.push_back( left[i] );
            continue;
        }
        Field f = f_invalid;
        if( !( left[i] == f_invalid && right[i] == f_invalid ) ) {
            Field l = left[i] == f_invalid ? 0 : left[i];
            Field r = right[i] == f_invalid ? 0 : right[i];
            f = add( l, r );
        }
        v.push_back( f );
    }
    return v;
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
