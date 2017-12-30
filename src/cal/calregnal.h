/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calregnal.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Regnal variant calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th March 2014
 * Copyright:   Copyright (c) 2014 ~ 2017, Nick Matthews.
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

        bool is_ok() const;
        size_t record_size() const override { return m_fieldnames.size() + 1; }
        size_t extended_size() const override { return Base::extended_size() + m_ext_fieldnames.size(); }
        int get_fieldname_index( const std::string& fieldname ) const override;

        Field get_jdn( const Field* fields ) const override;

        Field get_opt_field( const Field* fields, Field jdn, int index ) const override;

        // Give the chance to set a field to a fixed value.
        void set_fixed_fields( Field* fields ) const override;

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override;

        BoolVec mark_balanced_fields(
            Field* left, Field ljdn, Field* right, Field rjdn, const XRefVec& rank ) const override;

        void set_fields( Field* fields, Field jdn ) const override;

        bool is_tier1( const std::string& fieldname, const FormatText* fmt ) const override;
    protected:
        int get_std_fieldname_index( const std::string& fieldname ) const override;
        std::string get_std_fieldname( size_t index ) const override;

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
    };

}

#endif // CAL_CALREGNAL_H_GUARD