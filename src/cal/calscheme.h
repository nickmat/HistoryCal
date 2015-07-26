/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscheme.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Header for Scheme class.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
 * Copyright:   Copyright (c) 2013 - 2015, Nick Matthews.
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

#ifndef CAL_CALSCHEME_H_GUARD
#define CAL_CALSCHEME_H_GUARD

#include "cal/caldefs.h"
#include "calregnal.h"
#include "calhybrid.h"

namespace Cal {

    class Calendars;
    class Grammar;
    class Base;

    class Scheme
    {
    public:
        enum BaseScheme { 
            BS_NULL, BS_jdn, BS_julian, BS_gregorian, BS_french, BS_hebrew,
            BS_islamic
        };
        Scheme( const std::string& name, Base* base );
        ~Scheme();

        bool is_ok() const;
        std::string get_code() const { return m_code; }
        std::string get_name() const { return m_name; }
        Scheme_style get_style() const { return m_style; }
        std::string get_pref_input_format() const;
        void get_info( Scheme_info* info ) const; 
        void get_input( SchemeFormats* info ) const; 
        void get_output( SchemeFormats* info ) const; 
        Base* get_base() const { return m_base; }

        void set_code( const std::string& code ) { m_code = code; }
        void set_style( Scheme_style style ) { m_style = style; }
        void set_input_format( const std::string& code );
        void set_output_format( const std::string& code );

        Field fieldvec_to_jdn( const FieldVec& fieldv );
        FieldVec jdn_to_fieldvec( Field jdn );

        Field str_to_jdn( const std::string& str, const std::string& fmt );
        std::string jdn_to_str( Field jdn );
        std::string range_to_str( Range range );
        RangeList rlist_str_to_rangelist( const std::string& str );
        std::string rangelist_to_str( const RangeList& ranges );

        Field add_to_jdn( Field jdn, Field value, Unit unit, Norm norm );
        bool str_to_rel_info( const std::string& str, Rel_info* info ) const;
        RangeList rel_rangelist( const RangeList& ranges, Rel_info* info );

        static Base* create_base( BaseScheme bs );
        static Base* create_base( BaseScheme bs, const std::string& data );
        static Base* create_base_shift( Base* sbase, Field era );
        static Base* create_base_hybrid( 
            const StringVec& fieldnames,
            const StringVec& ext_fieldnames,
            const std::vector<HybridData>& data );
        static Base* create_base_regnal( 
            const StringVec& fieldnames,
            const StringVec& ext_fieldnames,
            const FieldVec& fixedfields,
            const std::vector<RegnalEra>& eras );
    private:
        Unit str_to_unit( const std::string& str ) const;

        std::string   m_name;
        std::string   m_code;
        Scheme_style  m_style;
        Base*         m_base;
    };

}


#endif // CAL_CALSCHEME_H_GUARD