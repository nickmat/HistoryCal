/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calsvalue.h
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

#ifndef CAL_CALSVALUE_H_GUARD
#define CAL_CALSVALUE_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class STokenStream;

    class SValue
    {
    public:
        enum Type { 
            SVT_Error, SVT_Null, 
            SVT_Str, SVT_Bool, SVT_Field, SVT_Range, SVT_RList, SVT_Record
        };
        SValue() : m_type(SVT_Null) {}
        SValue( const SValue& value );
        SValue( const std::string& str ) : m_type(SVT_Str), m_str(str) {}
        SValue( bool b ) : m_type(SVT_Bool) { m_range.jdn1 = b ? 1 : 0; }
        SValue( Field field ) : m_type(SVT_Field) { m_range.jdn1 = field; }
        SValue( Range range ) : m_type(SVT_Range), m_range(range) {}
        SValue( const RangeList& rlist ) : m_type(SVT_RList), m_rlist(rlist) {}
        SValue( const std::string& scode, const FieldVec& fields ) { set_record( scode, fields ); }

        void set_str( const std::string& str ) { m_type = SVT_Str; m_str = str; }
        void set_bool( bool b ) { m_type = SVT_Bool; m_range.jdn1 = b ? 1 : 0; }
        void set_field( Field field ) { m_type = SVT_Field; m_range.jdn1 = field; }
        void set_range( Range range );
        void set_rlist( RangeList rlist );
        void set_record( const std::string& scode, const FieldVec& fields );

        void set_error( const std::string& str );

        void set( const RangeList& rlist ); 

        std::string get_str() const;
        bool get_bool() const;
        Field get_field() const;
        Range get_range() const;
        RangeList get_rlist() const;
        FieldVec get_record() const;
        std::string get_record_scode() const;

        bool get( std::string& str ) const;
        bool get( Field& field ) const;
        bool get( bool& flag ) const;
        bool get_rlist( RangeList& rlist ) const;

        bool is_error() const { return m_type == SVT_Error; }
        bool propagate_error( const SValue& value );
        bool obtain_rlists( RangeList& left, RangeList& right, const SValue& value );

        void logical_or( const SValue& value );
        void logical_and( const SValue& value );
        void equal( const SValue& value );
        void greater_than( const SValue& value );
        void less_than( const SValue& value );
        void plus( const SValue& value );
        void minus( const SValue& value );
        void multiply( const SValue& value );
        void divide( const SValue& value );
        void modulus( const SValue& value );

        void rlist_union( const SValue& value );
        void intersection( const SValue& value );
        void rel_complement( const SValue& value );
        void sym_difference( const SValue& value );

        void range_op( const SValue& value );
        void subscript_op( const SValue& value );
        void property_op( const SValue& value );

        void negate(); // Unitary minus
        void logical_not();
        void compliment();

        static STokenStream* set_token_stream( STokenStream* ts );

        Type type() const { return m_type; }
    private:
        Field add( Field left, Field right ) const;
        FieldVec add( const FieldVec& left, const FieldVec& right ) const;
        Range add( Range range, Field field ) const;
        Range add( Range left, Range right ) const;
        RangeList add( RangeList rlist, Field field ) const;
        RangeList add( RangeList rlist, Range range ) const;
        Field multiply( Field left, Field right ) const;
        Field divide( Field left, Field right ) const;
        Field modulus( Field left, Field right ) const;

        Type        m_type;
        std::string m_str;
        Range       m_range;
        RangeList   m_rlist;
        static STokenStream* m_ts;
    };

}

#endif // CAL_CALSVALUE_H_GUARD
