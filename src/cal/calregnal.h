/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calregnal.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Regnal variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th March 2014
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

#ifndef CAL_CALREGNAL_H_GUARD
#define CAL_CALREGNAL_H_GUARD

#include "calbase.h"

namespace Cal {

    class Schemes;
    class Scheme;
    class Record;

    struct RegnalEra {
        RegnalEra() : begin(f_minimum), end(f_maximum), base(NULL), scheme(NULL) {}

        Field   begin;
        Field   end;
        XRefVec xref;
        Base*   base;
        Scheme* scheme;
    };

    class Regnal : public Base
    {
    public:
        Regnal( Schemes* schemes, const std::string& code, const std::string& data );
        ~Regnal();

        virtual size_t record_size() const { return m_rec_size; }
        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;

        virtual Field get_jdn( const Field* fields ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask );

        virtual void remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

    private:
        void create_fieldnames( const std::string& names );
        void create_default_scheme( Schemes* schs, const std::string& code );
        void create_schemes( Schemes* schs, const std::string& data );
        void add_scheme( RegnalEra* era, Schemes* schs, const std::string& data );
        // Adjust and copy Era date fields to base date fields
        FieldVec get_base_fields( const Field* fields ) const;
        // Adjust the given Record to Era fields
        bool make_regnal_fields( Field* fields, Field era, Record& rec ) const;

        std::vector<RegnalEra> m_eras;
        StringVec   m_fieldnames;
        size_t      m_rec_size;
    };

}

#endif // CAL_CALREGNAL_H_GUARD