/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calmulti.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Multi variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     2nd October 2013
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

#ifndef CAL_CALMULTI_H_GUARD
#define CAL_CALMULTI_H_GUARD

#include "calbase.h"

namespace Cal {

    class Schemes;

    class Multi : public Base
    {
    public:
        Multi( Schemes* schemes, const std::string& data );
        ~Multi();

        virtual size_t record_size() const { return m_rec_size; }

        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;

        virtual Field get_jdn( const Field* fields ) const;

        virtual bool check_usable( const Field* fields ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask );

        // This will only lead to confusion
        virtual void remove_fields_if_first( Field* fields ) const {};
        virtual void remove_fields_if_last( Field* fields ) const {};

        virtual void set_fields( Field* fields, Field jdn ) const;

    private:
        // Using the jdn from the first base, complete the remaining fields
        void fill_fields( Field* fields ) const;

        std::vector<Base*> m_bases;
        std::vector<std::string> m_sname;
        size_t  m_rec_size;
    };

}

#endif // CAL_CALMULTI_H_GUARD