/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calendars.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Main library API header
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     19th September 2013
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

#ifndef CAL_CALENDARS_H_GUARD
#define CAL_CALENDARS_H_GUARD

#include "caldefs.h"

namespace Cal {

    class Schemes;

    enum Init_schemes { Init_schemes_none, Init_schemes_default };

    class Calendars
    {
    public:
        DLLIMPEXP_CAL Calendars( Init_schemes = Init_schemes_none );
        DLLIMPEXP_CAL ~Calendars();

        DLLIMPEXP_CAL void read_script( const std::string& script );

        DLLIMPEXP_CAL static const char* version();
        DLLIMPEXP_CAL int get_scheme_count() const;
        DLLIMPEXP_CAL int get_scheme_id( const std::string& code ) const;
        DLLIMPEXP_CAL void get_scheme_info( Scheme_info* info, int scheme_id ) const; 
        DLLIMPEXP_CAL void get_scheme_input( Scheme_input* info, int scheme_id ) const; 
        DLLIMPEXP_CAL void get_scheme_output( Scheme_output* info, int scheme_id ) const;
        DLLIMPEXP_CAL bool get_vocab_info( Vocab_info* info, const std::string& code ) const; 

        DLLIMPEXP_CAL void set_scheme_order( int scheme_id, int index );
        DLLIMPEXP_CAL void set_scheme_format( int scheme_id, int index );

        DLLIMPEXP_CAL Field fieldvec_to_jdn( int scheme_id, const FieldVec& fieldv );
        DLLIMPEXP_CAL FieldVec jdn_to_fieldvec( int scheme_id, Field jdn );

        DLLIMPEXP_CAL Field str_to_jdn( int scheme_id, const std::string& str );
        DLLIMPEXP_CAL std::string jdn_to_str( int scheme_id, Field jdn );
        DLLIMPEXP_CAL Range str_to_range( int scheme_id, const std::string& str );
        DLLIMPEXP_CAL std::string range_to_str( int scheme_id, Range range );
        DLLIMPEXP_CAL RangeList str_to_rangelist( int scheme_id, const std::string& str );
        DLLIMPEXP_CAL std::string rangelist_to_str( int scheme_id, const RangeList& ranges );

        DLLIMPEXP_CAL Field add_to_jdn( int sch_id, Field jdn, Field value, Unit unit, Norm norm );

        DLLIMPEXP_CAL bool str_to_rel_info( int sch_id, const std::string& str, Rel_info* info ) const;
        DLLIMPEXP_CAL RangeList rel_rangelist( int sch_id, const RangeList& ranges, Rel_info* info );

    private:
        Schemes* m_schemes;
    };

}

#endif // CAL_CALENDARS_H_GUARD