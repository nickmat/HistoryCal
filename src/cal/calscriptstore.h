/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscriptstore.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     ScriptStore class definition header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     17th May 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
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

#ifndef CAL_CALSCRIPTSTORE_H_GUARD
#define CAL_CALSCRIPTSTORE_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class SValue
    {
    public:
        enum Type { SVT_Error, SVT_Null, 
            SVT_Str, SVT_Bool, SVT_Field, SVT_Range, SVT_RList };
        SValue() : m_type(SVT_Null) {}
        SValue( const SValue& value );
        SValue( const std::string& str ) : m_type(SVT_Str), m_str(str) {}
        SValue( bool b ) : m_type(SVT_Bool) { m_range.jdn1 = b ? 1 : 0; }
        SValue( Field field ) : m_type(SVT_Field) { m_range.jdn1 = field; }
        SValue( Range range ) : m_type(SVT_Range), m_range(range) {}
        SValue( const RangeList& rlist ) : m_type(SVT_RList), m_rlist(rlist) {}

        void set_str( const std::string& str ) { m_type = SVT_Str; m_str = str; }
        void set_bool( bool b ) { m_type = SVT_Bool; m_range.jdn1 = b ? 1 : 0; }
        void set_field( Field field ) { m_type = SVT_Field; m_range.jdn1 = field; }
        void set_range( Range range ) { m_type = SVT_Range; m_range = range; }
        void set_rlist( RangeList rlist ) { m_type = SVT_RList; m_rlist = rlist; }
        void set_error( const std::string& str ) { m_type = SVT_Error; m_str = str; }

        void set( RangeList& rlist ); 

        std::string get_str() const;
        bool get_bool() const;
        Field get_field() const;
        Range get_range() const;
        RangeList get_rlist() const;

        bool get( std::string& str ) const;
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

        void rlist_union( const SValue& value );
        void intersection( const SValue& value );
        void rel_complement( const SValue& value );
        void sym_difference( const SValue& value );

        void range_op( const SValue& value );

        void negate(); // Unitary minus
        void logical_not();
        void compliment();

    private:
        Type type() const { return m_type; }
        Field add( Field left, Field right ) const; 
        Range add( Range range, Field field ) const; 
        Range add( Range left, Range right ) const; 
        RangeList add( RangeList rlist, Field field ) const; 
        RangeList add( RangeList rlist, Range range ) const; 

        Type        m_type;
        std::string m_str;
        Range       m_range;
        RangeList   m_rlist;
    };


    class Scheme;

    typedef std::map<std::string,SValue> SValueMap;

    class ScriptStore {
    public:
        ScriptStore() : ischeme(NULL), oscheme(NULL) {}

        SValueMap table;
        Scheme*   ischeme;
        Scheme*   oscheme;
    };

}

#endif // CAL_CALSCRIPTSTORE_H_GUARD
