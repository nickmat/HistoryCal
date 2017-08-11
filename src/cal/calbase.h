/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calbase.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base class header, used as base for all calendar calulations.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
 * Copyright:   Copyright (c) 2013 ~ 2017, Nick Matthews.
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

#include "calprivate.h"

namespace Cal {

    class FormatText;

    typedef std::map<int,XRefVec> XRefSet;
    typedef std::map<std::string,XRefSet> XRefMap;

    enum {
        YMD_year, YMD_month, YMD_day
    };
    enum OptFieldID {
        OFID_NULL,
        OFID_wday,      // 7 Day week Mon=1 (1 to 7)
        OFID_wsday,     // 7 Day week Sun=1 (1 to 7)
        OFID_dayinyear, // Day in year (1 to about 366, dep. on scheme)
        OFID_unshift,   // Value before being shifted, (year or day)
        // Astronomy values that return the next event (as a jdn) after a given jdn.
        OFID_nequinox,  // The jdn day of the Next Northward (Mar) Equinox.
        OFID_nsolstice, // The jdn day of the Next Northern (Jun) Solstice.
        OFID_sequinox,  // The jdn day of the Next Southward (Sep) Equinox.
        OFID_ssolstice, // The jdn day of the Next Southern (Dec) Solstice.
        OFID_newmoon,   // The jdn day of the next New Moon.
        // Julian, Gregorian
        OFID_j_litweek, // See calliturgical.cpp
        OFID_j_ce,      // 0 = BCE, 1 = CE
        OFID_j_ceyear,  // Positive CE or BCE year
        OFID_j_eastermonth, // The month easter falls for the given year (3 or 4)
        OFID_j_easterday, // The day of the month easter falls for the given year
        OFID_j_easter, // The jdn day easter falls for the given year.
        OFID_j_eastershift, // The year value if shifted to easter day new year.
        OFID_j_easterrpt, // With eastershift, set to first or second date within Easter year.
        // French Republican fields
        OFID_fr_nmonth, // Named month (1 to 12) (or invalid)
        OFID_fr_nmday,  // Named month day (1 to 30) (or invalid)
        OFID_fr_cday,   // Complementary day (1 to 6) (or invalid)
        OFID_fr_dday,   // Day within Decade (1 to 10) (or invalid)
        // Chinese fields
        OFID_c_year,    // Continuous year count.
        OFID_c_yceles,  // Year Celestial Stem (1 to 10).
        OFID_c_yterre,  // Year Terrestial Branch (1 to 12).
        OFID_c_msexag,  // Month Sexagesimal value (1 to 60).
        OFID_c_mceles,  // Month Celestial Stem (1 to 10).
        OFID_c_mterre,  // Month Terrestial Branch (1 to 12).
        OFID_c_dsexag,  // Day Sexagesimal value (1 to 60).
        OFID_c_dceles,  // Day Celestial Stem (1 to 10).
        OFID_c_dterre,  // Day Terrestial Branch (1 to 12).
        //
        OFID_MAX
    };

    class Base
    {
    public:
        Base();
        Base( const std::string& data );
        virtual ~Base();

        // Set initial data from string.
        virtual void set_data( const std::string& data );

        // Return true if in a usable state.
        virtual bool is_ok() const { return true; }
        // Return the maximum number of Fields required by the Record.
        virtual size_t record_size() const = 0;
        // Return the number of extended (read-only) Fields available.
        virtual size_t extended_size() const { return record_size() + m_opt_fields.size(); }

        // Returns the index to the named Record field, or -1 if not found.
        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;
        virtual OptFieldID get_opt_field_id( const std::string& fieldname ) const;
        virtual std::string get_opt_fieldname( OptFieldID field_id ) const;

        // Converts the Field's into a jdn and returns it.
        virtual Field get_jdn( const Field* fields ) const = 0;

        // Get an optional field value.
        virtual Field get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const;

        // Give the chance to set a field to a fixed value.
        virtual void set_fixed_fields( Field* fields ) const {}

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const = 0;

        // Calculate the optional field, indicated by index, that is on or after the jdn or mask values.
        // If calulated date is different, update the fields to match and return true, otherwise return false.
        virtual bool set_fields_as_next_optional( Field* fields, Field jdn, const Field* mask, size_t index ) const;
        // Calculate the optional field, indicated by index, that is on or before the jdn or mask values.
        // If calulated date is different, update the fields to match and return true, otherwise return false.
        virtual bool set_fields_as_prev_optional( Field* fields, Field jdn, const Field* mask, size_t index ) const;

        // to be removed
        virtual void remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const;
        // to be removed
        virtual void remove_boundary_fields( Field* left, Field* right ) const;

        virtual BoolVec mark_balanced_fields(
            Field* left, Field ljdn, Field* right, Field rjdn, const XRefVec& rank ) const;
        virtual BoolVec mark_boundary_fields(
            Field* left, Field ljdn, Field* right, Field rjdn, const XRefVec& rank ) const;

        Field get_field( const Field* fields, Field jdn, size_t index ) const;
        Field get_field_first( const Field* fields, Field jdn, size_t index ) const;
        Field get_field_last( const Field* fields, Field jdn, size_t index ) const;
        virtual Field get_rec_field_first( const Field* fields, size_t index ) const;
        virtual Field get_rec_field_last( const Field* fields, size_t index ) const;
        virtual Field get_opt_field_first( const Field* fields, Field jdn, OptFieldID id ) const;
        virtual Field get_opt_field_last( const Field* fields, Field jdn, OptFieldID id ) const;

        // Converts the given jdn into the Records' Fields.
        virtual void set_fields( Field* fields, Field jdn ) const = 0;

        // Check for consistant fields. Used when one field is used as an index and
        // must be within certain bounds.
        virtual bool fields_ok( const Field* fields ) const { return true; }

        // If the unit is a fixed integer (ie week) then return the value,
        // otherwise, if the unit is variable, a double or invalid for Record, return 0.
        virtual Field unit_is_int( const Field* fields, Unit unit ) const;
        // If the unit is of a type that can be added to to a Record without the
        // possibility of creating an invalid Record, return true.
        virtual bool can_add_unit( const Field* fields, Unit unit ) const;
        // Add value * units to the Records fields. Value may be negative.
        // Exactly what this means depends on the Calendar Scheme.
        // Returns true if it succeeds, otherwise false;
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
        virtual void resolve_opt_input( Field* fields, size_t index ) const {}

        std::string lookup_token( Field field, const std::string& vcode, bool abbrev ) const;
        std::string get_alias_fieldname( const std::string& alias ) const;

        void get_input_info( SchemeFormatInfo* info ) const;
        void get_output_info( SchemeFormatInfo* info ) const;
        std::string get_input_fcode() const;
        std::string get_output_fcode() const;
        Grammar* get_grammar() const;
        Format* get_format( const std::string& fcode ) const;

        void set_grammar( Grammar* grammar );
        void set_input_fcode( const std::string& code ) { m_input_fcode = code; }
        void set_output_fcode( const std::string& code ) { m_output_fcode = code; }

        // Add an optional field to the base.
        // The fieldname must match one of the OptFieldID enum's.
        void add_opt_field( const std::string& fieldname );

        XRefVec get_xref_order( int count, const FormatText* fmt ) const;

        FieldVec fields_to_vec( const Field* fields ) const;
        bool is_complete( const Field* fields ) const;
        void copy_fields( Field* to, const Field* from ) const;
        // return zero if first == second
        // return pos value if first comes before second, or neg if reversed
        // return f_invalid if an invalid field is encountered
        Field compare_except( const Field* first, const Field* second, size_t except = 0 ) const;

        OptFieldID opt_index_to_id( size_t index ) const { return m_opt_fields[index-record_size()]; }
        int opt_id_to_index( OptFieldID id ) const;
        virtual bool is_tier1( const std::string& fieldname, const FormatText* fmt ) const;
    protected:
        virtual int get_std_fieldname_index( const std::string& fieldname ) const { return get_ymd_fieldname_index( fieldname ); }
        virtual std::string get_std_fieldname( size_t index ) const { return get_ymd_fieldname( index ); }

        size_t opt_fields_size() const { return m_opt_fields.size(); }

        XRefVec get_default_xref_order( int count ) const;

        int get_ymd_fieldname_index( const std::string& fieldname ) const;
        std::string get_ymd_fieldname( size_t index ) const;
        size_t sizeof_ymd_fieldnames() const { return s_sizeof_ymd_fieldnames; }
        int get_opt_fieldname_index( const std::string& fieldname ) const;
        XRefVec create_xref( const StringVec& fieldnames ) const; 
        XRefSet create_input_xref_set( const FormatText* fmt ) const;
        virtual StringVec get_rank_fieldnames() const { return StringVec(0); }

    private:
        void create_default_grammar() const;

        static const char* s_ymd_fieldnames[];
        static size_t s_sizeof_ymd_fieldnames;

        // We may need to create a default grammar if none exists
        // so we may as well cashe it.
        mutable Grammar* m_grammar;
        std::string      m_input_fcode;
        std::string      m_output_fcode;
        // Cache these as they are expensive to create.
        mutable XRefMap  m_xref_inputs;

        std::vector<OptFieldID> m_opt_fields;
        LocaleData m_locale;
    };

}

#endif // CAL_CALBASE_H_GUARD
