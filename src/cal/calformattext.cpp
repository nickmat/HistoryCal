/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformattext.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     FormatText class to control formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st March 2016
 * Copyright:   Copyright (c) 2016 ~ 2020, Nick Matthews.
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

#include <algorithm>
#include <cassert>

using namespace Cal;
using std::string;


FormatText::FormatText( const string& code, Grammar* gmr )
: Format( code, gmr ), m_separators(":,"), m_sig_rank_size(0), m_shorthand(true)
{
}

bool Cal::FormatText::construct()
{
    if ( !m_control_in.empty() ) {
        setup_control_in();
    }
    if ( !m_control_out.empty() ) {
        setup_control_out();
    }
    set_ok( true );
    return true;
}

void FormatText::setup_control_in()
{
    ElementControlIn ele;
    bool do_output = true;
    string input;
    for ( auto it = m_control_in.begin(); it != m_control_in.end(); it++ ) {
        if ( do_output ) {
            if ( *it == '(' ) {
                do_output = false;
            }
            continue;
        }
        if ( *it == ')' ) {
            ele.expand_specifier( get_owner() );

            if ( input.size() ) {
                input += " ";
            }
            input += ele.get_input_text();
            InputFieldType type = ele.get_type();
            if ( ele.has_dual_field() ) {
                input += "/";
                m_dual2_fieldname = ele.get_dual_record_field_name();
                type = IFT_dual1;
            }
            string fieldname = ele.get_record_field_name();
            if ( !fieldname.empty() ) {
                if ( ele.has_valid_default() ) {
                    m_default_names.push_back( fieldname );
                    m_default_values.push_back( ele.get_default_value() );
                } else {
                    m_format_order.push_back( fieldname );
                }
            }
            ele.clear();
            do_output = true;
        } else if ( !do_output ) {
            ele.add_char( *it );
        }
    }
    set_empty_user_input_str( input );
    StringVec rank_list = get_rank_fieldnames();
    for ( string rank_name : rank_list ) {
        for ( string format_name : m_format_order ) {
            if ( rank_name == format_name ) {
                m_rankin_order.push_back( rank_name );
                break;
            }
        }
    }
}

void FormatText::setup_control_out()
{
    assert( get_owner() );
    if ( m_rankout_fieldnames.empty() ) {
        m_rankout_fieldnames = get_rank_fieldnames();
    }
    m_sig_rank_size = get_sig_rank_size();

    ElementControlOut ele;
    string fieldout, output;
    bool do_output = true;

    for ( auto it = m_control_out.begin(); it != m_control_out.end(); it++ ) {
        if ( do_output ) {
            if ( *it == '|' ) {
                output += fieldout;
                fieldout.clear();
            } else if ( *it == '(' ) {
                do_output = false;
            } else {
                fieldout += *it;
            }
            continue;
        }
        if ( *it == ')' ) {
            ele.expand_specifier( get_owner() );
            fieldout += ele.get_field_output_name();
            if ( m_shorthand ) {
                string rfn = get_owner()->get_field_alias( ele.get_field_name() );
                if ( is_non_sig_record_name( rfn ) ) {
                    m_shorthand = false;
                }
            }
            ele.clear();
            do_output = true;
        } else if ( !do_output ) {
            ele.add_char( *it );
        }
    }
    output += fieldout;
    set_empty_user_output_str( output );
}

string FormatText::range_to_string( const Base* base, Range range ) const
{
    if ( range.jdn1 == range.jdn2 ) {
        return jdn_to_string( base, range.jdn1 );
    }
    string str1, str2;
    Record rec1( base, range.jdn1 );
    Record rec2( base, range.jdn2 );

    if ( m_shorthand ) {
        StringVec ranknames = get_rankout_fieldnames();
        XRefVec xref( m_sig_rank_size );
        if ( ranknames.empty() ) {
            for ( size_t i = 0; i < xref.size(); i++ ) {
                xref[i] = i;
            }
        } else {
            size_t size = std::min( xref.size(), ranknames.size() );
            for ( size_t i = 0; i < size; i++ ) {
                xref[i] = base->get_fieldname_index( ranknames[i] );
            }
        }
        BoolVec reveal = rec1.mark_balanced_fields( rec2, xref );
        for ( size_t i = xref.size(); i < ranknames.size(); i++ ) {
            reveal.push_back( true );
        }
        str1 = get_revealed_output( rec1, &reveal );
        str2 = get_revealed_output( rec2, &reveal );
    } else {
        str1 = get_output( rec1 );
        str2 = get_output( rec2 );
    }
    if ( str1 == str2 ) {
        return str1;
    }
    return str1 + " .. " + str2;
}

string FormatText::get_revealed_output( const Record& record, const BoolVec* reveal ) const
{
    Element ele;
    string output, fieldout, value;
    enum State { ignore, dooutput, doelement };
    State state = dooutput;
    for ( auto it = m_control_out.begin(); it != m_control_out.end(); it++ ) {
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
                Field f = get_field( record, ele.get_field_name(), reveal );
                if ( ele.has_dual_field_name() ) {
                    Field d = get_field( record, ele.get_dual_field_name(), reveal );
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

RangeList FormatText::string_to_rlist( const Base* base, const string& input ) const
{
    if(  input.find( '~' ) != string::npos ||
        input.find( "..") != string::npos ||
        input.find( '|' ) != string::npos ) {
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

size_t Cal::FormatText::get_sig_rank_size() const
{
    if ( m_sig_rank_size == 0 ) {
        return get_owner()->get_sig_rank_size();
    }
    return m_sig_rank_size;
}

bool FormatText::is_input_field( const std::string& fieldname ) const
{
    for( size_t i = 0 ; i < m_format_order.size() ; i++ ) {
        if( fieldname == m_format_order[i] ) {
            return true;
        }
    }
    return false;
}

bool FormatText::is_non_sig_record_name( const string& fieldname ) const
{
    for ( size_t i = m_sig_rank_size; i < m_rankout_fieldnames.size(); i++ ) {
        if ( fieldname == m_rankout_fieldnames[i] ) {
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
    const Record& record, const std::string& fname, const BoolVec* reveal ) const
{
    int index = record.get_field_index( fname );
    if ( index < 0 || ( reveal && size_t( index ) >= reveal->size() ) ) {
        return f_invalid;
    }
    Field field;
    if ( get_owner()->get_element( &field, record, fname, reveal ) ) {
        return field;
    }
    if ( reveal && !( *reveal )[index] ) {
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
                        dual = true;
                    } else if( dual ) {
                        assert( i >= 1 );
                        ifs[i].value = str_to_dual2( ifs[i-1].value, token );
                        ifs[i].type = IFT_dual2;
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

bool FormatText::resolve_input(
    const Base* base, Field* fields, InputFieldVec& input ) const
{
    size_t size = base->extended_size();
    FieldVec fs( size, f_invalid );
    for ( size_t i = 0; i < m_default_names.size(); i++ ) {
        int index = base->get_fieldname_index( m_default_names[i] );
        if ( index >= 0 ) {
            fields[index] = m_default_values[i];
        }
    }
    FieldVec element_list;
    bool has_elements = false;;
    for ( size_t i = 0; i < input.size(); i++ ) {
        if ( input[i].type == IFT_dual2 ) {
            int index = base->get_fieldname_index( m_dual2_fieldname );
            if ( index >= 0 ) {
                fields[index] = input[i].value;
            }
            continue;
        }
        if ( input[i].type == IFT_vocab ) {
            string vname = input[i].vocab->get_fieldname();
            if ( !is_input_field( vname ) ) {
                int index = base->get_fieldname_index( vname );
                if ( index >= 0 ) {
                    fields[index] = input[i].value;
                    continue;
                } else {
                    // Assume field is an element
                    input[i].type = IFT_calc;
                    has_elements = true;
                    continue;
                }
            }
        }
        if ( input[i].type != IFT_null ) {
            element_list.push_back( input[i].value );
        }
    }
    StringVec format_order;
    size_t element_size = element_list.size();
    for ( string fname : m_format_order ) {
        for ( size_t i = 0; i < element_size; i++ ) {
            if ( fname == m_rankin_order[i] ) {
                format_order.push_back( fname );
                break;
            }
        }
    }
    if ( element_size != format_order.size() ) {
        return false;
    }
    for ( size_t i = 0; i < element_size; i++ ) {
        int index = base->get_fieldname_index( format_order[i] );
        if ( index >= 0 ) {
            fields[index] = element_list[i];
        } else {
            return false;
        }
    }
    if ( has_elements ) {
        Record record( base, fields, size );
        for ( auto infield : input ) {
            if ( infield.type == IFT_calc ) {
                get_owner()->set_element( &record, infield.vocab->get_fieldname(), infield.value );
            }
        }
        record.get_fields( fields, size );
    }
    for ( size_t i = base->record_size(); i < base->extended_size(); i++ ) {
        if ( fields[i] != f_invalid ) {
            base->resolve_opt_input( fields, i );
        }
    }
    return true;
}

RangeList FormatText::multirange_str_to_rlist( const Base* base, const string& input ) const
{
    RangeList rlist;
    string str = input;
    string rangestr, begval, endval;
    size_t pos1;
    for( ;;) {
        pos1 = str.find( '|' );
        rangestr = str.substr( 0, pos1 );
        size_t pos2 = rangestr.find( '~' );
        if ( pos2 == string::npos ) {
            pos2 = rangestr.find( ".." );
            if ( pos2 != string::npos ) {
                pos2++; // Step over second dot.
            }
        }
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

RangeList FormatText::bare_str_to_rlist( const Base* base, const string& input ) const
{
    Record mask( base, input, get_code(), RB_none );
    return mask.get_rlist_from_mask();
}

// End of src/cal/calformattext.cpp file
