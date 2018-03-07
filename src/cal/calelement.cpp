/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Name:        src/cal/calelement.cpp
* Project:     Cal: Programmable Historical Calendar library.
* Purpose:     Element classes for formatting and parsing date elements.
* Author:      Nick Matthews
* Website:     http://historycal.org
* Created:     15th February 2018
* Copyright:   Copyright (c) 2018, Nick Matthews.
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

#include "calelement.h"

#include "cal/calendars.h"
#include "calgrammar.h"
#include "calparse.h"
#include "caltext.h"
#include "calvocab.h"

using namespace Cal;
using std::string;


void Element::clear()
{
    m_state = DO_FIELD;
    m_field_name.clear();
    m_dual_field_name.clear();
    m_default_text.clear();
    m_vcode.clear();
    m_spec.clear();
    m_qualifier.clear();
}

void Element::add_char( char ch )
{
    switch ( m_state )
    {
    case DO_FIELD:
        switch ( ch )
        {
        case '/': m_state = DO_DUAL;    return;
        case ':': m_state = DO_VCODE;   return;
        }
        m_field_name += ch;
        return;
    case DO_DUAL:
        m_dual_field_name += ch;
        return;
    case DO_VCODE:
        switch ( ch )
        {
        case ':': m_state = DO_SPEC;    return;
        case '=': m_state = DO_DEFAULT; return;
        }
        m_vcode += ch;
        return;
    case DO_SPEC:
        switch ( ch )
        {
        case ':': m_state = DO_QUAL;    return;
        case '=': m_state = DO_DEFAULT; return;
        }
        m_spec += ch;
        return;
    case DO_QUAL:
        if ( ch == '=' ) {
            m_state = DO_DEFAULT;
            return;
        }
        m_qualifier += ch;
        return;
    case DO_DEFAULT:
        m_default_text += ch;
        return;
    }
}

void Element::add_string( const string & str )
{
    for ( auto ch : str ) {
        add_char( ch );
    }
}

string Element::get_formatted_element( Calendars* cal, Field field ) const
{
    if ( m_vcode.empty() && m_spec.empty() ) {
        return field_to_str( field );
    }

    string result;
    if ( m_vcode.empty() ) {
        StringStyle ss = SS_undefined;
        if ( m_qualifier == "u" ) {
            ss = SS_uppercase;
        } else if ( m_qualifier == "l" ) {
            ss = SS_lowercase;
        }
        if ( m_spec == "oa" ) {
            result = get_ordinal_suffix( field, ss );
        } else if ( m_spec == "os" ) {
            result = field_to_str( field ) + get_ordinal_suffix( field, ss );
        } else if ( m_spec == "rn" ) {
            result = get_roman_numerals( field, ss );
        } else if ( m_spec == "lp" ) {
            result = get_left_padded( field, m_qualifier );
        }
    } else {
        Vocab* voc = cal->get_vocab( m_vcode );
        if ( voc ) {
            Vocab::Pseudo abbrev = ( m_spec == "a" ) ? Vocab::pseudo_abbrev : Vocab::pseudo_full;
            result = voc->lookup( field, abbrev );
        }
    }
    return result;
}

Field Element::get_converted_field( Calendars* cal, const string& str ) const
{
    if ( m_vcode.empty() && m_spec.empty() ) {
        return str_to_field( str );
    }

    if ( m_vcode.empty() ) {
        if ( m_spec == "os" ) {
            return str_to_field( str );
        }
        // Currently, can't use any other format.
        return f_invalid;
    }
    Vocab* voc = cal->get_vocab( m_vcode );
    if ( voc ) {
        return voc->find( str );
    }
    return f_invalid;
}

void ElementControl::clear()
{
    Element::clear();
    m_type = IFT_number;
    m_voc = nullptr;
    m_record_field_name.clear();
    m_dual_record_field_name.clear();
    m_field_output_name.clear();
    m_default_value = f_invalid;
}


bool ElementControl::expand_specifier( Grammar* gmr )
{
    m_voc = gmr->find_vocab( m_vcode );
    if ( !m_dual_field_name.empty() ) {
        m_type = IFT_dual2;
    }
    m_record_field_name = gmr->get_field_alias( m_field_name );
    m_field_output_name = gmr->get_element_pseudo_name( m_field_name );
    if ( m_voc ) {
        m_type = IFT_vocab;
        if ( m_field_output_name.empty() ) {
            if ( m_spec == "a" ) {
                m_field_output_name = m_voc->get_pseudo_name( Vocab::pseudo_abbrev );
            } else {
                m_field_output_name = m_voc->get_pseudo_name( Vocab::pseudo_full );
            }
        }
        m_default_value = m_voc->find( m_default_text );
    } else if ( m_vcode.empty() && !m_spec.empty() ) {
        StringStyle ss = SS_undefined;
        if ( m_qualifier == "u" ) {
            ss = SS_uppercase;
        } else if ( m_qualifier == "l" ) {
            ss = SS_lowercase;
        }
        if ( m_spec == "oa" ) {
            m_field_output_name = get_ordinal_suffix_style( ss );
        } else if ( m_spec == "os" ) {
            m_field_output_name = gmr->get_num_pseudo_alias( m_field_name ) +
                get_ordinal_suffix_style( ss );
        } else if ( m_spec == "rn" ) {
            m_field_output_name = gmr->get_num_pseudo_alias( m_field_name ) +
                get_roman_numerals_style( ss );
        } else if ( m_spec == "lp" ) {
            m_field_output_name = get_left_pad_style( m_field_output_name, m_qualifier );
        }
    } else if ( !m_dual_field_name.empty() ) {
        m_dual_record_field_name = gmr->get_field_alias( m_dual_field_name );
        m_field_output_name = gmr->get_num_pseudo_alias( m_field_name ) +
            "/" + gmr->get_num_pseudo_alias( m_dual_field_name );
    } else if( m_field_output_name.empty() ) {
        m_field_output_name = gmr->get_num_pseudo_alias( m_field_name );
    }
    return true;
}

std::string ElementControl::get_input_text() const
{
    if ( m_default_text.empty() ) {
        return m_field_name;
    }
    return "[" + m_default_text + "]";
}

// End of src/cal/calelement.cpp file
