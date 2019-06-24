/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caleaster.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Julian Easter calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     16th June 2019
 * Copyright:   Copyright (c) 2019, Nick Matthews.
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

#ifndef CAL_CALEASTER_H_GUARD
#define CAL_CALEASTER_H_GUARD

#include "caljulian.h"

namespace Cal {

    class Easter : public Julian
    {
    public:
        Easter( const std::string& data );

        void set_data( const std::string& data ) override;
        
        size_t record_size() const override { return 4; }

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override;

        Field get_jdn( const Field* fields ) const override;

        void set_fields( Field* fields, Field jdn ) const override;

    protected:
        int get_std_fieldname_index( const std::string& fieldname ) const override;
        std::string get_std_fieldname( size_t index ) const override;

    private:
        enum { YMD_hist = YMD_day + 1 };
        enum hist_rel_year {
            R_invalid,   // No valid conversion.
            R_equal,     // hist = year
            R_increase,  // hist = year + 1
            R_ambiguous  // hist = year or year + 1
        };

        hist_rel_year GetHistRelation( const Field* fields ) const;

        Field m_offset;
    };

}

#endif // CAL_CALEASTER_H_GUARD