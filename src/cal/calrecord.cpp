/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calrecord.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Record class to hold date values.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
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

#include "calrecord.h"

#include "calbase.h"
#include "calgrammar.h"
#include "calgregorian.h"
#include "calparse.h"

#include <cassert>

using namespace Cal;
using namespace std;

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

Record::Record( const Base* base, const string& str, const string& fmt )
    : m_base(base), m_jdn(f_invalid), m_f(base->extended_size())
{
    set_str( str, fmt );
}

Record::Record( const Record& rec )
    : m_base(rec.m_base), m_jdn(rec.m_jdn), m_f(rec.m_f)
{
}

void Record::set_str( const string& str, const string& fmt )
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

    InputFieldVec ifs(m_base->extended_size());
    parse_date( &ifs[0], ifs.size(), in );

    if( m_base->resolve_input( &m_f[0], ifs, fmt ) ) {
        m_jdn = get_jdn();
    } else {
        clear_fields();
    }
    return;
}

bool Record::set_fields_as_begin_first( const Field* mask )
{
    if( m_f[0] == f_minimum ) {
        m_jdn = f_minimum;
        return true;
    }
    bool ret = m_base->set_fields_as_begin_first( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
        int attempt = 0;
        for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
            if( mask[i] == f_invalid ) {
                continue;
            }
            // Adjust to match extended field
            if( m_base->set_fields_as_next_extended( &m_f[0], m_jdn, mask, i ) ) {
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

bool Record::set_fields_as_begin_last(  const Field* mask )
{
    if( m_f[0] == f_maximum ) {
        m_jdn = f_maximum;
        return true;
    }
    bool ret = m_base->set_fields_as_begin_last( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
        int attempt = 0;
        for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
            if( mask[i] == f_invalid ) {
                continue;
            }
            // Adjust to match extended field
            if( m_base->set_fields_as_prev_extended( &m_f[0], m_jdn, mask, i ) ) {
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

bool Record::set_fields_as_next( const Field* mask, Field maxjdn )
{
    if( m_f[0] == f_minimum ) {
        m_jdn = f_minimum;
        return true;
    }
    bool ret = m_base->set_fields_as_begin_first( &m_f[0], mask );
    if( ret ) {
        m_jdn = get_jdn();
        for( size_t i = m_base->record_size() ; i < m_base->extended_size() ; i++ ) {
            if( mask[i] == f_invalid ) {
                continue;
            }
            Field field = m_base->get_extended_field( &m_f[0], m_jdn, i );
            if( field != m_f[i] ) {
                // Adjust to match extended field
                // <<======<<<<
            }
        }
    }
    return ret;
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

string Record::get_str() const
{
    return get_output( m_base->get_format() );
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


void Record::clear_fields()
{
    // Clears the registers
    for( size_t i = 0 ; i < m_f.size() ; i++ ) {
        m_f[i] = f_invalid;
    }
    m_jdn = f_invalid;
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

void Record::set_jdn( Field jdn )
{
    clear_fields();
    m_base->set_fields( &m_f[0], jdn );
    m_jdn = jdn;
}

int Record::get_field_index( const string& fieldname ) const
{
    string fn = m_base->get_alias_fieldname( fieldname );
    return m_base->get_fieldname_index( fn );
}

Record::CP_Group Record::get_cp_group( string::const_iterator it )
{
    int ch = unsigned char( *it );
    if( ch == '-' ) {
// For the moment, we'll treat all hyphens as numbers
        return GRP_Digit;
//        return GRP_Hyphen;
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
    const char* seps = ":,";
    size_t i = 0;
    while( seps[i] ) {
        if( ch == seps[i] ) {
            return GRP_Sep;
        }
        i++;
    }
    return GRP_Other;
}

Field Record::get_token_value( Vocab** vocab, const string& str )
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

int Record::parse_date( InputField* ifs, size_t size, const string& str )
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
    grp = prev_grp = token_grp = get_cp_group( it );
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
            grp = get_cp_group( it );
        }
    }
    return i;
}

Field Record::get_field( int index ) const
{
    if( index >= 0 ) {
        if( index < (int) m_base->record_size() ) {
            return m_f[index];
        }
        if( index < (int) m_base->extended_size() ) {
            return m_base->get_extended_field( &m_f[0], m_jdn, index );
        }
    }
    return f_invalid;
}

bool Record::is_mask_valid( Field* mask, size_t mask_size ) const
{
    size_t size = min( mask_size, m_base->extended_size() );
    for( size_t i = 0 ; i < size ; i++ ) {
        if( mask[i] != f_invalid && mask[i] != m_f[i] ) {
            return false;
        }
    }
    return true;
}

string Record::value_from_field( int index ) const
{
    Field f = get_field( index );
    if( f == f_invalid ) {
        return "";
    }
    return field_to_str( f );
}

string Record::lookup_token(
    int index, int dual, const string& vcode, const string& abbrev ) const
{
    string result;
    bool a = ( abbrev == "a" );
    if( dual < 0 ) {
        if( index >= 0 ) {
            Field f = get_field( index );
            if( vcode.size() ) {
                result = m_base->lookup_token( f, vcode, a );
            }
            if( result.empty() ) {
                result = field_to_str( f );
            }
        }
    } else {
        // dual value
        result = value_from_field( index );
        string dualstr = value_from_field( dual );
        if( result.size() && dualstr.size() && dualstr != result ) {
            if( result.size() != dualstr.size() ) {
                return result + "/" + dualstr;
            }
            string suffix = "/";
            bool matched = true;
            for( string::iterator rit = result.begin(), dit = dualstr.begin()
                ; rit != result.end() ; rit++, dit++
            ) {
                if( matched && *rit != *dit ) {
                    matched = false;
                }
                if( ! matched ) {
                    suffix += *dit;
                }
            }
            result += suffix;
        }
    }
    return result;
}

string Record::get_output( const std::string& fmt ) const
{
    string output, prolog, fname, dname, vocab, abbrev, value;
    enum State { ignore, dooutput, doprolog, dofname, dodname, dovocab, doabbrev };
    State state = dooutput;
    for( string::const_iterator it = fmt.begin() ; it != fmt.end() ; it++ ) {
        switch( state )
        {
        case ignore:
            if( *it == '@' ) {
                state = doprolog;
            }
            break;
        case dooutput:
            if( *it == '@' ) {
                state = doprolog;
            } else {
                output += *it;
            }
            break;
        case doprolog:
            if( *it == '(' ) {
                state = dofname;
            } else {
                prolog += *it;
            }
            break;
        case dofname:
        case dodname:
        case dovocab:
        case doabbrev:
            if( *it == ')' ) {
                int i = get_field_index( fname );
                int d = -1;
                if( dname.size() ) {
                    d = get_field_index( dname );
                }
                value = lookup_token( i, d, vocab, abbrev );
                if( value.empty() ) {
                    state = ignore;
                } else {
                    output += prolog + value;
                    state = dooutput;
                }
                prolog.clear();
                fname.clear();
                dname.clear();
                vocab.clear();
                abbrev.clear();
            } else if( state == dofname && *it == ':' ) {
                state = dovocab;
            } else if( state == dofname && *it == '/' ) {
                state = dodname;
            } else if( state == dovocab && *it == '.' ) {
                state = doabbrev;
            } else {
                if( state == dofname ) {
                    fname += *it;
                } else if( state == dodname ) {
                    dname += *it;
                } else if( state == dovocab ) {
                    vocab += *it;
                } else { // doabbrev
                    abbrev += *it;
                }
            }
            break;
        }
    }
    return output;
}

// End of src/cal/calrecord.cpp file
