/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calbase.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base class header, used as base for all calendar calulations.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
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

#ifndef CAL_CALBASE_H_GUARD
#define CAL_CALBASE_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class Grammar;

    class Base
    {
        enum BaseExtendedFieldNumber { BEFN_wday, BEFN_COUNT };
    public:
        Base();
        virtual ~Base() {}

        // Return the maximum number of Fields required by the Record.
        virtual size_t record_size() const = 0;
        // Return the number of extended (read-only) Fields available.
        virtual size_t extended_size() const { return record_size() + BEFN_COUNT; }

        // Returns the index to the named Record field, or -1 if not found.
        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;

        // Converts the Field's into a jdn and returns it.
        virtual Field get_jdn( const Field* fields ) const = 0;

        // Get an extended field value
        virtual Field get_extended_field( const Field jdn, size_t index ) const;

        // Note, these members are not const so that they may set state conditions
        // related to the pos (position). This should speed up sucessive calls.
        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const = 0;

        virtual void remove_fields_if_first( Field* fields ) const;
        virtual void remove_fields_if_last( Field* fields ) const;

        virtual void remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const;

        virtual Field get_field_first( const Field* fields, size_t index ) const;
        virtual Field get_field_last( const Field* fields, size_t index ) const;

        // Converts the given jdn into the Records' Fields.
        virtual void set_fields( Field* fields, Field jdn ) const = 0;

        // If the unit is a fixed integer (ie week) then return the value,
        // otherwise, if the unit is variable, a double or invalid for Record, return 0.
        virtual Field unit_is_int( const Field* fields, Unit unit ) const;
        // If the unit is of a type that can be added to to a Record without the
        // possibility of creating an invalid Record, return true.
        virtual bool can_add_unit( const Field* fields, Unit unit ) const;
        // Add value * units to the Records fields. Value may be negative.
        // Exactly what this means depends on the Calendar Scheme.
        virtual bool add_to_fields( Field* fields, Field value, Unit unit ) const;
        // normalise is to correct illegal reg values
        // when considered in combination.
        // Individual values are assumed valid
        // ie For Gregorian calendar, r[2] = 31 is a valid but not if r[1] = 2.
        // Return true if value adjusted or false if no change.
        virtual bool normalise( Field* fields, Norm norm ) const { return false; }
        // Get the average number of days in a the given unit as a double.
        virtual double get_average_days( const Field* fields, Unit unit ) const;
        // Return pos if left < right, neg if left > right, zero if equal or
        // f_invalid if an invalid field is encountered, when comparing minor fields
        // (less than index).
        virtual Field compare_minor_fields( const Field* left, const Field* right, size_t index );

        std::string lookup_token( Field field, const std::string& vcode, bool abbrev ) const;
        std::string get_alias_fieldname( const std::string& alias ) const;

        void get_input_formats( SchemeFormats* input ) const;
        void get_output_formats( SchemeFormats* output ) const;
        std::string get_format() const;
        Grammar* get_grammar() const { return m_grammar; }

        void set_grammar( Grammar* grammar );
        void set_input_format( const std::string& code ); // { m_input_format = code; }
        void set_output_format( const std::string& code ) { m_output_format = code; }

        XRefVec get_xref_order( int count ) const;

        FieldVec fields_to_vec( const Field* fields ) const;
        bool is_complete( const Field* fields ) const;
        void copy_fields( Field* to, const Field* from ) const;
        // return zero if first == second
        // return pos value if first comes before second, or neg if reversed
        // return f_invalid if an invalid field is encountered
        Field compare_except( const Field* first, const Field* second, size_t except = 0 ) const;

    protected:
        int get_extended_fieldname_index( const std::string& fieldname ) const;
        std::string get_extended_fieldname( size_t index ) const;
        virtual XRefVec get_default_xref_order( int count ) const;

    private:
        std::string create_default_order() const;
        std::string create_default_format() const;

        Grammar*    m_grammar;
        std::string m_input_format;
        std::string m_output_format;
        std::map<int,XRefVec>  m_xref_order;
    };

}

#endif // CAL_CALBASE_H_GUARD
