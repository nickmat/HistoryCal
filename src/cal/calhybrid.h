/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calhybrid.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Hybrid variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     23rd September 2013
 * Copyright:   Copyright (c) 2013 ~ 2016, Nick Matthews.
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

namespace Cal {

    class Calendars;

    struct HybridData {
        HybridData() : start(f_invalid), base(NULL), scheme(NULL) {}

        bool ok() { return ( start != f_invalid && base != NULL ); }

        Field   start;
        Base*   base;
        Scheme* scheme; // NULL if not local
    };

    class Hybrid : public Base
    {
    public:
        Hybrid( const StringVec& fields, const StringVec& ext_fields, const std::vector<HybridData>& data );
        ~Hybrid();

        virtual bool is_ok() const;
        virtual size_t record_size() const { return m_ext_size; }

        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;
        virtual OptFieldID get_opt_field_id( const std::string& fieldname ) const;
        virtual std::string get_opt_fieldname( OptFieldID field_id ) const;

        virtual Field get_jdn( const Field* fields ) const;

        virtual Field get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const;
        virtual Field get_field_last( const Field* fields, size_t index ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const;

        virtual void remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

        virtual bool fields_ok( const Field* fields ) const;

        virtual void resolve_opt_input( Field* fields, size_t index ) const;

    protected:
        virtual StringVec get_rank_fieldnames() const { return m_fieldnames; }

    private:
        FieldVec get_xref( const Field* fields, Field sch ) const;
        bool is_in_scheme( Field jdn, Field scheme ) const;
        size_t find_scheme( Field jdn ) const;
        void set_hybrid_fields( Field* fields, const Field* mask, Field scheme ) const;
        XRefSet create_input_xref_set( const Format* fmt ) const;

        // Note: m_data.size() == m_xref_fields.size()
        std::vector<HybridData> m_data;
        // m_xref_fields gives the index into the bases fields that match the m_fieldnames
        // It would be better to add this to the m_data struct's
        std::vector<XRefVec>    m_xref_fields;
        StringVec               m_fieldnames;
        StringVec               m_ext_fieldnames;
        // Note: m_rec_size == m_fieldnames.size() + 1
        size_t                  m_rec_size;
        size_t                  m_ext_size;
    };

}

#endif // CAL_CALHYBRID_H_GUARD