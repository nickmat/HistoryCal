/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calelement.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Element classes for formatting and parsing date elements.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th February 2018
 * Copyright:   Copyright (c) 2018 ~ 2020, Nick Matthews.
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

#ifndef CAL_CALELEMENT_H_GUARD
#define CAL_CALELEMENT_H_GUARD

#include "calprivate.h"

namespace Cal {

    class Calendars;
    struct ElementField;

    class Element
    {
    public:
        Element() : m_state( DO_FIELD ) {}
        virtual ~Element() {}

        void clear();
        void add_char( char ch );
        void add_string( const std::string& str );

        std::string get_field_name() const { return m_field_name; }
        std::string get_dual_field_name() const { return m_dual_field_name; }

        std::string get_formatted_element( Calendars* cal, Field field ) const;
        Field get_converted_field( Calendars* cal, const std::string& str ) const;

        bool has_dual_field_name() const { return !m_dual_field_name.empty(); }

    private:
        enum State { DO_FIELD, DO_DUAL, DO_VCODE, DO_SPEC, DO_QUAL, DO_DEFAULT };
        State m_state;
    protected:
        std::string m_field_name;
        std::string m_dual_field_name;
        std::string m_vcode;
        std::string m_spec;
        std::string m_qualifier;
        std::string m_default_text;

    };

    class ElementControl : public Element
    {
    public:
        ElementControl() : Element(), m_type( IFT_number ), m_voc( nullptr ),
            m_calc_field(nullptr), m_default_value( f_invalid ) {}

        void clear();
        bool expand_specifier( Grammar* gmr );

        std::string get_input_text() const;
        std::string get_dual_record_field_name() const { return m_dual_record_field_name; }
        std::string get_record_field_name() const { return m_record_field_name; }
        std::string get_field_output_name() const { return m_field_output_name; }
        std::string get_default_text() const { return m_default_text; }

        Vocab* get_vocab() const { return m_voc; }
        ElementField* get_calc_field() const { return m_calc_field; }
        InputFieldType get_type() const { return m_type; }
        Field get_default_value() const { return m_default_value; }

        bool has_dual_field() const { return !m_dual_record_field_name.empty(); }
        bool has_vocab() const { return m_voc != nullptr; }
        bool has_valid_default() const { return m_default_value != f_invalid; }
        bool has_calc_field() const { return m_calc_field != nullptr; }

    private:
        InputFieldType m_type;
        Vocab* m_voc;
        ElementField* m_calc_field;
        std::string m_record_field_name;
        std::string m_dual_record_field_name;
        std::string m_field_output_name;
        Field m_default_value;
    };

}

#endif // CAL_CALELEMENT_H_GUARD
