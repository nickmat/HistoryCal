/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calregnal.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Regnal variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th March 2014
 * Copyright:   Copyright (c) 2014 ~ 2016, Nick Matthews.
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

    class Calendars;
    class Scheme;
    class Record;

    struct RegnalEra {
        RegnalEra() 
            : begin(f_minimum), end(f_maximum), base(NULL), scheme(NULL), local(true)
        {}

        Field   begin;
        Field   end;
        XRefVec xref;
        Base*   base;
        SHandle scheme;
        bool    local;
    };

    class Regnal : public Base
    {
    public:
        Regnal( 
            const StringVec& fieldnames,
            const StringVec& ext_fieldnames,
            const FieldVec& fixedfields,
            const std::vector<RegnalEra>& eras );
        ~Regnal();

        virtual bool is_ok() const;
        virtual size_t record_size() const { return m_ext_size; }

        virtual Field get_jdn( const Field* fields ) const;

        // Give the chance to set a field to a fixed value.
        virtual void set_fixed_fields( Field* fields ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const;
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const;

        virtual void remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const;

        virtual void set_fields( Field* fields, Field jdn ) const;

        virtual bool is_tier1( const std::string& fieldname, const FormatText* fmt ) const;
    protected:
        virtual int get_std_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_std_fieldname( size_t index ) const;

    private:
        // Return the era index coresponding to given jdn.
        size_t get_era_index( Field jdn ) const;
        // Adjust and copy Era date fields to base date fields
        FieldVec get_base_fields( const Field* fields ) const;
        // Adjust the given Record to Era fields
        bool make_regnal_fields( Field* fields, Field era, Record& rec ) const;

        std::vector<RegnalEra> m_eras;
        StringVec   m_fieldnames;
        StringVec   m_ext_fieldnames;
        FieldVec    m_fixed_fields;
        size_t      m_rec_size;
        size_t      m_ext_size;
    };

}

#endif // CAL_CALREGNAL_H_GUARD