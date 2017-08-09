/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calchinese.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Chinese Lunisolar calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th June 2016
 * Copyright:   Copyright (c) 2016 ~ 2017, Nick Matthews.
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

#ifndef CAL_CALCHINESE_H_GUARD
#define CAL_CALCHINESE_H_GUARD

#include "calbase.h"

namespace Cal {

    class Chinese : public Base
    {
    public:
        Chinese() {}
        Chinese( const std::string& data ) : Base( data ) {}

        size_t record_size() const override { return 5; }

        OptFieldID get_opt_field_id( const std::string& fieldname ) const override;
        std::string get_opt_fieldname( OptFieldID field_id ) const override;

        Field get_jdn( const Field* fields ) const override;

        Field get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const override;

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override { return false; }
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override { return false; }

        void set_fields( Field* fields, Field jdn ) const override;

        Field get_field_first( const Field* fields, size_t index ) const override;
        Field get_field_last( const Field* fields, size_t index ) const override;

        bool add_to_fields( Field* fields, Field value, Unit unit ) const override { return false; }
        bool normalise( Field* fields, Norm norm ) const override { return false; }

    protected:
        virtual int get_std_fieldname_index( const std::string& fieldname ) const override;
        virtual std::string get_std_fieldname( size_t index ) const override;

    private:
        Field get_year( const Field* fields ) const;
        Field get_month_count( const Field* fields ) const;

        static const char* s_cymld_fieldnames[];
        static size_t s_sizeof_cymld_fieldnames;
    };

}

#endif // CAL_CALCHINESE_H_GUARD