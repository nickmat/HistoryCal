/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformattext.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     FormatText class to control formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st March 2016
 * Copyright:   Copyright (c) 2016 ~ 2018, Nick Matthews.
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

#include "calformattext.h"

#include "calbase.h"
#include "calelement.h"
#include "calgrammar.h"
#include "calparse.h"
#include "calrecord.h"
#include "calsetmath.h"
#include "caltext.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;


FormatText::FormatText( const std::string& code, Grammar* gmr )
: Format( code, gmr ), m_separators(":,")
{
}

std::string FormatText::range_to_string( Base* base, Range range ) const
{
    if ( range.jdn1 == range.jdn2 ) {
        return jdn_to_string( base, range.jdn1 );
    }
    string str1, str2;
    Record rec1( base, range.jdn1 );
    Record rec2( base, range.jdn2 );

    StringVec ranknames = get_rankout_fieldnames();
    size_t rank_size = ranknames.empty() ? base->record_size() : ranknames.size();
    XRefVec xref( rank_size );
    if ( ranknames.empty() ) {
        for ( size_t i = 0; i < rank_size; i++ ) {
            xref[i] = i;
        }
    } else {
        for ( size_t i = 0; i < rank_size; i++ ) {
            xref[i] = base->get_fieldname_index( ranknames[i] );
        }
    }

    BoolVec mask = rec1.mark_balanced_fields( rec2, xref );
    str1 = get_masked_output( rec1, &mask );
    str2 = get_masked_output( rec2, &mask );
    if ( str1 == str2 ) {
        return str1;
    }
    return str1 + " ~ " + str2;
}

string FormatText::get_masked_output( const Record& record, const BoolVec* mask ) const
{
    Element ele;
    string output, fieldout, value;
    enum State { ignore, dooutput, doelement };
    State state = dooutput;
    for ( string::const_iterator it = m_control.begin(); it != m_control.end(); it++ ) {
        switch ( state )
        {
        case ignore:
            if ( *it == '|' ) {
                state = dooutput;
            }
            break;
        case dooutput:
            if ( *it == '|' ) {
                output += fieldout;
                fieldout.clear();
            } else if ( *it == '(' ) {
                state = doelement;
            } else {
                fieldout += *it;
            }
            break;
        case doelement:
            if ( *it == ')' ) {
                Field f = get_field( record, ele.get_field_name(), mask );
                if ( ele.has_dual_field_name() ) {
                    Field d = get_field( record, ele.get_dual_field_name(), mask );
                    value = dual_fields_to_str( f, d );
                } else {
                    value = ele.get_formatted_element( get_calenders(), f );
                }
                if ( value.empty() ) {
                    fieldout.clear();
                    state = ignore;
                } else {
                    fieldout += value;
                    state = dooutput;
                }
                ele.clear();
            } else {
                ele.add_char( *it );
            }
            break;
        }
    }
    return output + fieldout;
}

RangeList FormatText::string_to_rlist( Base* base, const string& input ) const
{
    if(  input.find( '~' ) != string::npos || input.find( '|' ) != string::npos ) {
        return multirange_str_to_rlist( base, input );
    }
    return bare_str_to_rlist( base, input );
}

bool FormatText::set_input( Record* record, const std::string& input, Boundary rb ) const
{
    const Base* base = record->get_base();
    InputFieldVec ifs(base->extended_size());
    parse_date( &ifs[0], ifs.size(), input );
    bool ret = resolve_input( base, record->get_field_ptr(), ifs );
    if( !ret || rb == RB_none ) {
        return ret;
    }
    Record rec( *record );
    if( rb == RB_begin ) {
        ret = record->set_fields_as_begin_first( rec.get_field_ptr(), false );
    }
    if( rb == RB_end ) {
        ret = record->set_fields_as_begin_last( rec.get_field_ptr(), false );
        bool ret2 = ret;
        while ( ret2 ) {
            ret2 = record->set_fields_as_next_last( rec.get_field_ptr() );
        }
    }
    return ret;
}

bool FormatText::resolve_input(
    const Base* base, Field* fields, const InputFieldVec& input ) const
{
    size_t cnt = 0;
    FieldVec fs( base->extended_size(), f_invalid );
    for ( size_t i = 0; i < m_default_names.size(); i++ ) {
        int index = base->get_fieldname_index( m_default_names[i] );
        if ( index >= 0 ) {
            fields[index] = m_default_values[i];
        }
    }
    for ( size_t i = 0; i < input.size(); i++ ) {
        if ( input[i].type == IFT_null ) {
            continue;
        }
        string fname;
        if ( input[i].type == IFT_dual2 ) {
            fname = get_1st_input_field( IFT_dual2 );
            if ( fname.empty() ) {
                continue; // Ignore if we can't find it.
            }
        }
        if ( input[i].vocab ) {
            fname = input[i].vocab->get_fieldname();
            if ( fname.empty() ) {
                fname = get_input_field( input[i].vocab );
                if ( fname.empty() ) {
                    continue; // Give up.
                }
            }
        }
        if ( fname.size() ) {
            if ( !is_input_field( fname ) ) {
                int index = base->get_fieldname_index( fname );
                if ( index < 0 ) continue;
                // Input an extended field
                fields[index] = input[i].value;
                continue;
            }
        }
        fs[cnt] = input[i].value;
        cnt++;
    }
    if ( cnt < 1 ) {
        return false;
    }
    XRefVec xref = base->get_xref_order( cnt, this );
    if ( xref.empty() ) {
        return false;
    }
    for ( size_t i = 0; i < cnt; i++ ) {
        int x = xref[i];
        if ( x >= 0 && x < (int)base->extended_size() ) {
            fields[x] = fs[i];
        }
    }
    for ( size_t i = base->record_size(); i < base->extended_size(); i++ ) {
        if ( fields[i] != f_invalid ) {
            base->resolve_opt_input( fields, i );
        }
    }
    return true;
}

void FormatText::set_control( const std::string& format, Use usefor )
{
    ElementControl ele;
    bool do_output = true;
    string fieldout, input, output;
    bool usefor_output, usefor_input, strict_input;
    switch( usefor )
    {
    case Use_output:
        usefor_output = true;
        usefor_input = false;
        strict_input = false;
        break;
    case Use_input:
        usefor_output = false;
        usefor_input = true;
        strict_input = true;
        break;
    case Use_strict:
        usefor_output = true;
        usefor_input = true;
        strict_input = true;
        break;
    case Use_inout:
    default:
        usefor_output = true;
        usefor_input = true;
        strict_input = false;
        break;
    }
    if( usefor_output ) {
        m_control = format;
    }
    for( string::const_iterator it = format.begin() ; it != format.end() ; it++ ) {
        if( do_output ) {
            if( *it == '|' ) {
                if( usefor_output ) {
                    output += fieldout;
                }
                fieldout.clear();
            } else if( *it == '(' ) {
                do_output = false;
            } else {
                fieldout += *it;
                if( strict_input ) {
                    input += *it;
                }
            }
            continue;
        }
        if ( *it == ')' ) {
            ele.expand_specifier( get_owner() );
            if ( usefor_input ) {
                if ( !strict_input && input.size() ) {
                    input += " ";
                }
                input += ele.get_input_text();
                if ( ele.has_dual_field() ) {
                    input += "/";
                    m_input_fields.push_back( ele.get_dual_record_field_name() );
                    m_vocabs.push_back( NULL );
                    m_types.push_back( IFT_dual1 );
                }
                if ( ele.has_valid_default() ) {
                    m_default_names.push_back( ele.get_record_field_name() );
                    m_default_values.push_back( ele.get_default_value() );
                } else {
                    m_input_fields.push_back( ele.get_record_field_name() );
                    m_vocabs.push_back( ele.get_vocab() );
                    m_types.push_back( ele.get_type() );
                }
            }
            fieldout += ele.get_field_output_name();
            ele.clear();
            do_output = true;
        } else if( !do_output )  {
            ele.add_char( *it );
        }
    }
    if( usefor_output ) {
        output += fieldout;
        set_user_output_str( output );
    }
    if( usefor_input ) {
        set_user_input_str( input );
        if ( !m_default_names.empty() ) {
            // We need to remove the default field from the ranking fields.
            if ( m_rank_fieldnames.empty() ) {
                m_rank_fieldnames = get_rank_fieldnames();
            }
            for ( auto it = m_rank_fieldnames.begin(); it != m_rank_fieldnames.end(); ) {
                bool matched = false;
                for ( auto name : m_default_names ) {
                    if ( name == *it ) {
                        matched = true;
                        break;
                    }
                }
                if ( matched ) {
                    it = m_rank_fieldnames.erase( it );
                } else {
                    it++;
                }
            }
        }
    }
}

string FormatText::get_input_field( Vocab* vocab ) const
{
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        if( vocab == m_vocabs[i] ) {
            return m_input_fields[i];
        }
    }
    return "";
}

string FormatText::get_1st_input_field( InputFieldType type ) const
{
    for( size_t i = 0 ; i < m_types.size() ; i++ ) {
        if( type == m_types[i] ) {
            return m_input_fields[i];
        }
    }
    return "";
}

StringVec FormatText::get_rank_fieldnames() const
{
    if ( m_rank_fieldnames.empty() ) {
        return get_owner()->get_rank_fieldnames();
    }
    return m_rank_fieldnames;
}

StringVec FormatText::get_rankout_fieldnames() const
{
    if ( m_rankout_fieldnames.empty() ) {
        return get_rank_fieldnames();
    }
    return m_rankout_fieldnames;
}

bool FormatText::is_input_field( const std::string& fieldname ) const
{
    for( size_t i = 0 ; i < m_input_fields.size() ; i++ ) {
        if( fieldname == m_input_fields[i] ) {
            return true;
        }
    }
    return false;
}

FormatText::CP_Group FormatText::get_cp_group(
    string::const_iterator it, string::const_iterator end ) const
{
    int ch = *it;
    if( ch < 0 ) {  // eliminate non-ascii 
        return GRP_Other;
    }
    for( string::const_iterator sit = m_separators.begin() ; sit != m_separators.end() ; sit++ ) {
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

Field FormatText::get_field(
    const Record& record, const std::string& fname, const BoolVec* mask ) const
{
    int index = record.get_field_index( fname );
    Field field;
    if ( get_owner()->get_element( &field, record, fname ) ) {
        return field;
    }
    if ( index < 0 || ( mask && !( *mask )[index] ) ) {
        return f_invalid;
    }
    return record.get_field( index );
}

int FormatText::parse_date( InputField* ifs, size_t size, const string& str ) const
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
    grp = prev_grp = token_grp = get_cp_group( it, str.end() );
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
                        ifs[i-1].value = str_to_dual2( ifs[i-2].value, token );
                        dual = false;
                    } else {
                        ifs[i].value = str_to_field( token );
                        ifs[i].type = IFT_number;
                        i++;
                    }
                }
                if( token_grp == GRP_Other ) {
                    Field f = get_owner()->find_token( &(ifs[i].vocab), token );
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
            grp = get_cp_group( it, str.end() );
        }
    }
    return i;
}

RangeList FormatText::multirange_str_to_rlist( Base* base, const string& input ) const
{
    RangeList rlist;
    string str = input;
    string rangestr, begval, endval;
    size_t pos1;
    for( ;;) {
        pos1 = str.find( '|' );
        rangestr = str.substr( 0, pos1 );
        size_t pos2 = rangestr.find( '~' );
        if( pos2 == string::npos ) {
            // single value
            RangeList rl = bare_str_to_rlist( base, rangestr );
            rlist.insert( rlist.end(), rl.begin(), rl.end() );
        } else {
            // start and end
            begval = rangestr.substr( 0, pos2 );
            endval = rangestr.substr( pos2 + 1 );
            bool ret1 = true, ret2 = true;
            Range range;
            if( begval.empty() ) {
                range.jdn1 = f_minimum;
            } else {
                Record rec( base );
                ret1 = set_input( &rec, begval, RB_begin );
                range.jdn1 = rec.get_jdn();
            }
            if( endval.empty() ) {
                range.jdn2 = f_maximum;
            } else {
                Record rec( base );
                ret2 = set_input( &rec, endval, RB_end );
                range.jdn2 = rec.get_jdn();
            }
            if( ret1 && ret2 && range.jdn1 != f_invalid && range.jdn2 != f_invalid ) {
                rlist.push_back( range );
            }
        }
        if( pos1 == string::npos ) {
            break;
        }
        str = str.substr( pos1 + 1 );
    }
    return op_set_well_order( rlist );
}

RangeList FormatText::bare_str_to_rlist( Base* base, const string& input ) const
{
    Record mask( base, input, get_code(), RB_none );
    return mask.get_rlist_from_mask();
}

// End of src/cal/calformattext.cpp file
