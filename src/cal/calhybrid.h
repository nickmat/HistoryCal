/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calhybrid.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Hybrid variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     23rd September 2013
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

#ifndef CAL_CALHYBRID_H_GUARD
#define CAL_CALHYBRID_H_GUARD

#include "calbase.h"

#include <string>
#include <vector>

namespace Cal {

    class Schemes;

    class Hybrid : public Base
    {
    public:
        Hybrid( Schemes* schemes, const std::string& data );
        ~Hybrid();

        virtual size_t record_size() const { return m_rec_size; }

        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;

        virtual Field get_jdn( const Field* fields ) const;

        virtual Field get_field_last( const Field* fields, size_t index ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask );
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask );
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask );

//        virtual void remove_fields_if_first( Field* fields ) const;
//        virtual void remove_fields_if_last( Field* fields ) const;

//        virtual bool balance_fields( Field* firsts, Field* lasts ) const;
        virtual void remove_balanced_fields( Field* left, Field* right ) const;
        virtual bool set_field_first( Field* fields, size_t index ) const;
        virtual bool set_field_last( Field* fields, size_t index ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

    protected:
        virtual XRefVec get_default_xref_order( int count );

    private:
        void create_fieldnames( const std::string& names );
        void add_scheme( Schemes* schs, const std::string& def );

        FieldVec get_xref( const Field* fields, Field sch ) const;
        void set_xref( Field* fields, const Field* mask, Field sch ) const;
        bool is_in_scheme( Field jdn, Field scheme ) const;
        Field find_scheme( Field jdn ) const;
        void set_hybrid_fields( Field* fields, const Field* mask, Field scheme ) const;

        // Note: m_bases.size() == m_xref_fields.size() == m_dates.size() - 1 
        std::vector<Base*>   m_bases;
        // m_xref_fields gives the index into the bases fields that match the m_fieldnames
        std::vector<XRefVec> m_xref_fields;
        FieldVec             m_dates;
        // Note: m_rec_size == m_fieldnames.size() + 1
        StringVec            m_fieldnames;
        size_t               m_rec_size;
        size_t               m_max_child_size;
    };

}

#endif // CAL_CALHYBRID_H_GUARD