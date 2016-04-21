/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calrecord.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Record class to hold date values.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
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

#include "calrecord.h"

#include "calbase.h"
#include "calformat.h"
#include "calgrammar.h"
#include "calgregorian.h"
#include "calparse.h"
#include "caltext.h"

#include <cassert>

using namespace Cal;
using std::string;

Record::Record( const Base* base )
    : m_base(base), m_jdn(f_invalid), m_f(base->extended_size())
{
    clear_fields();
}

Record::Record( const Base* base, Field jdn )
    : m_base(base), m_jdn(jdn), m_f(base->extended_size())
{
    set_jdn( jdn );
}

Record::Record( const Base* base, const Field* fields, size_t size )
    : m_base(base), m_jdn(f_invalid), m_f(base->extended_size())
{
    set_fields( fields, size );
}

Record::Record( const Base* base, const string& str, const string& fcode )
    : m_base(base), m_jdn(f_invalid), m_f(base->extended_size())
{
    set_str( str, fcode );
}

Record::Record( const Record& rec )
    : m_base(rec.m_base), m_jdn(rec.m_jdn), m_f(rec.m_f)
{
}

void Record::set_jdn( Field jdn )
{
    clear_fields();
    m_base->set_fields( &m_f[0], jdn );
    m_jdn = jdn;
}

void Record::set_fields( const Field* fields, size_t size )
{
    for( size_t i = 0 ; i < m_f.size() ; i++ ) {
        if( i >= size ) {
            m_f[i] = f_invalid;
        } else {
            m_f[i] = fields[i];
        }
    }
    m_jdn = get_jdn();
}

void Record::set_str( const string& str, const string& fcode )
{
    clear_fields();
    string in = full_trim( str );
    if( in == "past" ) {
        m_jdn = m_f[0] = f_minimum;
        return;
    }
    if( in == "future" ) {
        m_jdn = m_f[0] = f_maximum;
        return;
    }
    if( in == "today" ) {
        set_jdn( Gregorian::today() );
        return;
    }

    string fc;
    if( fcode.empty() ) {
        fc = m_base->get_input_fcode();
    } else {
        fc = fcode;
    }
    if( fcode == "u" ) {
        parse_units( in );
        return;
    }
    Format* fmt = m_base->get_format( fc );
    if( fmt == NULL ) {
        return;
    }
    InputFieldVec ifs(m_base->extended_size());
    parse_date( &ifs[0], ifs.size(), in, fmt );

    if( m_base->resolve_input( &m_f[0], ifs, fmt ) ) {
        m_jdn = get_jdn();
    } else {
        clear_fields();
    }
}

bool Record::set_fields_as_begin_first( const Field* mask, bool check )
{
    if( m_f[0] == f_minimum ) {
        m_jdn = f_minimum;
        return true;
    }
    bool ret = m_base->set_fields_as_begin_first( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
        if( check ) {
            int attempt = 0;
            for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
                if( mask[i] == f_invalid ) {
                    continue;
                }
                // Adjust to match extended field
                if( m_base->set_fields_as_next_optional( &m_f[0], m_jdn, mask, i ) ) {
                    Field jdn = get_jdn();
                    if( m_jdn != jdn ) {
                        m_jdn = jdn;
                        if( attempt >= 3 ) {
                            return false; // Give up.
                        }
                        attempt++;
                        i = m_base->record_size() - 1; // Start again.
                    }
                }
            }
        }
    }
    return ret;
}

bool Record::set_fields_as_next_first( const Field* mask )
{
    bool ret = m_base->set_fields_as_next_first( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
    }
    return ret;
}

bool Record::correct_fields_as_first( const Field* mask )
{
    int attempt = 0;
    for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
        if( mask[i] == f_invalid ) {
            continue;
        }
        // Adjust to match extended field
        if( m_base->set_fields_as_next_optional( &m_f[0], m_jdn, mask, i ) ) {
            Field jdn = get_jdn();
            if( m_jdn != jdn ) {
                m_jdn = jdn;
                if( attempt >= 3 ) {
                    return false; // Give up.
                }
                attempt++;
                i = m_base->record_size() - 1; // Start again.
            }
        }
    }
    return true;
}

bool Record::set_fields_as_begin_last(  const Field* mask, bool check )
{
    if( m_f[0] == f_maximum ) {
        m_jdn = f_maximum;
        return true;
    }
    bool ret = m_base->set_fields_as_begin_last( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
        if( check ) {
            int attempt = 0;
            for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
                if( mask[i] == f_invalid ) {
                    continue;
                }
                // Adjust to match extended field
                if( m_base->set_fields_as_prev_optional( &m_f[0], m_jdn, mask, i ) ) {
                    Field jdn = get_jdn();
                    if( m_jdn != jdn ) {
                        m_jdn = jdn;
                        if( attempt >= 3 ) {
                            return false; // Give up.
                        }
                        attempt++;
                        i = m_base->record_size() - 1; // Start again.
                    }
                }
            }
        }
    }
    return ret;
}

bool Record::set_fields_as_next_last( const Field* mask )
{
    bool ret = m_base->set_fields_as_next_last( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
    }
    return ret;
}

bool Record::correct_fields_as_last( const Field* mask )
{
    int attempt = 0;
    for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
        if( mask[i] == f_invalid ) {
            continue;
        }
        // Adjust to match extended field
        if( m_base->set_fields_as_prev_optional( &m_f[0], m_jdn, mask, i ) ) {
            Field jdn = get_jdn();
            if( m_jdn != jdn ) {
                m_jdn = jdn;
                if( attempt >= 3 ) {
                    return false; // Give up.
                }
                attempt++;
                i = m_base->record_size() - 1; // Start again.
            }
        }
    }
    return true;
}

void Record::remove_balanced_fields( Record* rec )
{
    // Both must have the same Base and not be identical.
    if( m_base == rec->m_base && m_jdn != rec->get_jdn() ) {
        m_base->remove_balanced_fields( &m_f[0], m_jdn, rec->get_field_ptr(), rec->get_jdn() );
    }
}

Field Record::get_jdn() const
{
    if( m_f[0] == f_minimum || m_f[0] == f_maximum ) {
        return m_f[0];
    }
    // Base should check or correct for valid content
    return m_base->get_jdn( &m_f[0] );
}

string Record::get_str( const std::string& fcode ) const
{
    string fc = fcode.empty() ? m_base->get_output_fcode() : fcode;
    Format* fmt = m_base->get_format( fc );
    if( fmt == NULL ) {
        return "";
    }
    return fmt->get_output( *this );
}

Field Record::is_unit_int( Unit unit ) const
{
    return m_base->unit_is_int( &m_f[0], unit );
}

bool Record::can_add_unit( Unit unit ) const
{
    return m_base->can_add_unit( &m_f[0], unit );
}

bool Record::add( Field value, Unit unit, Norm norm )
{
    bool ret = m_base->add_to_fields( &m_f[0], value, unit );
    if( ret ) {
        normalise( norm );
    }
    return ret;
}

bool Record::normalise( Norm norm )
{
    bool ret = m_base->normalise( &m_f[0], norm );
    if( ret ) {
        m_jdn = get_jdn();
    }
    return ret;
}

double Record::get_average_days( Unit unit ) const
{
    return m_base->get_average_days( &m_f[0], unit );
}

Field Record::get_field( int index ) const
{
    if( index >= 0 ) {
        if( index < (int) m_base->record_size() ) {
            return m_f[index];
        }
        if( index < (int) m_base->extended_size() ) {
            OptFieldID id = m_base->opt_index_to_id( index );
            return m_base->get_opt_field( &m_f[0], m_jdn, id );
        }
    }
    return f_invalid;
}

bool Record::is_mask_valid( Field* mask, size_t mask_size ) const
{
    size_t size = std::min( mask_size, m_base->extended_size() );
    for( size_t i = 0 ; i < size ; i++ ) {
        if( mask[i] != f_invalid && mask[i] != m_f[i] ) {
            return false;
        }
    }
    return true;
}

int Record::get_field_index( const string& fieldname ) const
{
    string fn = m_base->get_alias_fieldname( fieldname );
    return m_base->get_fieldname_index( fn );
}

int Record::get_unit_index( const string& unitname ) const
{
    string fn = m_base->get_alias_fieldname( unitname );
    return m_base->get_fieldname_index( fn );
}

void Record::clear_fields()
{
    // Clears the registers
    for( size_t i = 0 ; i < m_f.size() ; i++ ) {
        m_f[i] = f_invalid;
    }
    m_jdn = f_invalid;
}

Record::CP_Group Record::get_cp_group(
    string::const_iterator it, string::const_iterator end, Format* fmt ) const
{
    int ch = *it;
    if( ch < 0 ) {  // eliminate non-ascii 
        return GRP_Other;
    }
    string sep = fmt->get_separators();
    for( string::const_iterator sit = sep.begin() ; sit != sep.end() ; sit++ ) {
        if( *it == *sit ) {
            return GRP_Sep;
        }
    }
    if( ch == '-' ) {
        // If hyphen is followed by a digit treat as digit
		if( it+1 != end ) {
		    int ch1 = *(it+1);
			if( ch1 > 0 && isdigit( ch1 ) ) {
				return GRP_Digit;
			}
		}
        // Otherwise treat it as text
		return GRP_Other;
    }
    if( ch == '/' ) {
        return GRP_Dual;
    }
    if( ch == unknown_val ) {
        return GRP_Quest;
    }
    if( isdigit( ch ) ) {
        return GRP_Digit;
    }
    return GRP_Other;
}

Field Record::get_token_value( Vocab** vocab, const string& str ) const
{
    Grammar* gmr = m_base->get_grammar();
    if( gmr ) {
        return gmr->find_token( vocab, str );
    }
    return f_invalid;
}

Field Record::get_dual2_value( Field dual1, const string& str2 ) const
{
    string str1 = field_to_str( dual1 );
    if( str1.size() <= str2.size() ) {
        return str_to_field( str2 );
    }
    string result;
    string::const_iterator it1 = str1.end(), it2 = str2.end();
    while( it1 != str1.begin() ) {
        --it1;
        if( it2 != str2.begin() ) {
            --it2;
            result = *it2 + result;
        } else {
            result = *it1 + result;
        }
    }
    return str_to_field( result );
}

int Record::parse_date( InputField* ifs, size_t size, const string& str, Format* fmt )
{
    size_t i = 0;

    assert( ifs != NULL );
    assert( size > 0 );
    if( str.empty() ) {
        return 0;
    }
    string token;
    CP_Group grp, prev_grp, token_grp;
    string::const_iterator it = str.begin();
    grp = prev_grp = token_grp = get_cp_group( it, str.end(), fmt );
    if( grp == GRP_Quest ) {
        prev_grp = GRP_Sep;
    }
    bool done = false, ignore = false, dual = false;
    for(;;) {
        if( grp != prev_grp ) {
            token = full_trim( token );
            if( token.size() ) {
                if( token_grp == GRP_Digit ) {
                    if( grp == GRP_Dual ) {
                        ifs[i].value = str_to_field( token );
                        ifs[i].type = IFT_dual1;
                        i++;
                        // Default is dual dates are the same.
                        ifs[i].value = ifs[i-1].value;
                        ifs[i].type = IFT_dual2;
                        i++;
                        dual = true;
                    } else if( dual ) {
                        assert( i >= 2 );
                        ifs[i-1].value = get_dual2_value( ifs[i-2].value, token );
                        dual = false;
                    } else {
                        ifs[i].value = str_to_field( token );
                        ifs[i].type = IFT_number;
                        i++;
                    }
                }
                if( token_grp == GRP_Other ) {
                    Field f = get_token_value( &(ifs[i].vocab), token );
                    if( f == f_invalid ) {
                        return -1; // Unrecognised token
                    }
                    ifs[i].value = f;
                    ifs[i].type = IFT_vocab;
                    i++;
                }
                if( i == size ) {
                    break;
                }
            }
            token.clear();
            if( grp == GRP_Quest ) {
                ifs[i].value = f_invalid;
                ifs[i].type = IFT_quest;
                i++;
                if( i == size ) {
                    break;
                }
                // Question marks are not grouped
                prev_grp = GRP_Sep;
            } else {
                prev_grp = grp;
            }
            token_grp = grp;
        }
        if( done ) {
            break;
        }
        if( token_grp == GRP_Digit || token_grp == GRP_Other ) {
            token += *it;
        }
        it++;
        if( grp != GRP_Digit && grp != GRP_Dual ) dual = false; 
        if( it == str.end() ) {
            grp = GRP_Sep;
            done = true;
        } else {
            grp = get_cp_group( it, str.end(), fmt );
        }
    }
    return i;
}

void Record::set_field_by_unit( const std::string& value, const std::string& unit )
{
    Field v = str_to_field( value );
    if( v == 0 ) {
        return;
    }
    int i = get_unit_index( unit );
    if( i < 0 ) {
        return;
    }
    m_f[i] = v;
}

// A unit string is parsed as a list of a number and unit-name pairs,
// where the unit matches a fieldname or its alias.
void Record::parse_units( const string& str )
{
    enum step_t { START, VALUE, UNIT };
    step_t step = START;
    string value, unit;
    for( string::const_iterator it = str.begin() ; it != str.end() ; it++ ) {
        int ch = *it;
        if( u8_isspace( ch ) ) {
            continue;
        }
        if( step != VALUE && ( ch == '-' || u8_isdigit( ch ) ) ) {
            if( !value.empty() ) {
                set_field_by_unit( value, unit );
            }
            value.clear();
            unit.clear();
            step = VALUE;
            value += ch;
            continue;
        }
        if( step == VALUE ) {
            if( u8_isdigit( ch ) ) {
                value += ch;
                continue;
            }
            step = UNIT;
        }
        unit += ch;
    }
    set_field_by_unit( value, unit );
}

// End of src/cal/calrecord.cpp file
