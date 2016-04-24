/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformattext.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     FormatText class to control formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st March 2016
 * Copyright:   Copyright (c) 2016, Nick Matthews.
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

string FormatText::get_output( const Record& record ) const
{
    string output, fieldout, fname, dname, vocab, abbrev, value;
    enum State { ignore, dooutput, dofname, dodname, dovocab, doabbrev };
    State state = dooutput;
    for( string::const_iterator it = m_format.begin() ; it != m_format.end() ; it++ ) {
        switch( state )
        {
        case ignore:
            if( *it == '|' ) {
                state = dooutput;
            }
            break;
        case dooutput:
            if( *it == '|' ) {
                output += fieldout;
                fieldout.clear();
            } else if( *it == '(' ) {
                state = dofname;
            } else {
                fieldout += *it;
            }
            break;
        case dofname:
        case dodname:
        case dovocab:
        case doabbrev:
            if( *it == ')' ) {
                Field f = get_field( record, fname );
                if( dname.size() ) {
                    Field d = get_field( record, dname );
                    value = dual_fields_to_str( f, d );
                } else {
                    value = formatted_str( f, vocab, abbrev );
                }
                if( value.empty() ) {
                    fieldout.clear();
                    state = ignore;
                } else {
                    fieldout += value;
                    state = dooutput;
                }
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
    return output+fieldout;
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
    }
    return ret;
}

bool FormatText::resolve_input(
    const Base* base, Field* fields, const InputFieldVec& input ) const
{
    size_t cnt = 0;
    FieldVec fs( base->extended_size(), f_invalid );
    for( size_t i = 0 ; i < input.size() ; i++ ) {
        if( input[i].type == IFT_null ) {
            continue;
        }
        string fname;
        if( input[i].type == IFT_dual2 ) {
            fname = get_1st_input_field( IFT_dual2 );
            if( fname.empty() ) {
                continue; // Ignore if we can't find it.
            }
        }
        if( input[i].vocab ) {
            fname = input[i].vocab->get_fieldname();
            if( fname.empty() ) {
                fname = get_input_field( input[i].vocab );
                if( fname.empty() ) {
                    continue; // Give up.
                }
            }
        }
        if( fname.size() ) {
            if( !base->is_tier1( fname, this ) ) {
                int index = base->get_fieldname_index( fname );
                // Input an extended field
                fields[index] = input[i].value;
                continue;
            }
        }
        fs[cnt] = input[i].value;
        cnt++;
    }
    if( cnt < 1 ) {
        return false;
    }
    XRefVec xref = base->get_xref_order( cnt, this );
    if( xref.empty() ) {
        return false;
    }
    for( size_t i = 0 ; i < cnt ; i++ ) {
        int x = xref[i];
        if( x >= 0 && x < (int) base->extended_size() ) {
            fields[x] = fs[i];
        }
    }
    for( size_t i = base->record_size() ; i < base->extended_size() ; i++ ) {
        if( fields[i] != f_invalid ) {
            base->resolve_opt_input( fields, i );
        }
    }
    return true;
}

void FormatText::set_format( const std::string& format, Use usefor )
{
    assert( get_owner() );
    enum State { dooutput, dofname, dodname, dovocab, doabbrev };
    State state = dooutput;
    string fieldout, fname, dname, vocab, abbrev, input, output;
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
        m_format = format;
    }
    for( string::const_iterator it = format.begin() ; it != format.end() ; it++ ) {
        if( state == dooutput ) {
            if( *it == '|' ) {
                if( usefor_output ) {
                    output += fieldout;
                }
                fieldout.clear();
            } else if( *it == '(' ) {
                state = dofname;
            } else {
                fieldout += *it;
                if( strict_input ) {
                    input += *it;
                }
            }
            continue;
        }
        if( *it == ')' ) {
            if( !strict_input && input.size() ) {
                input += " ";
            }
            if( usefor_input ) {
                input += fname;
            }
            InputFieldType type = IFT_number;
            string fieldname = get_owner()->get_field_alias( fname );
            string foname = get_owner()->get_num_code_alias( fname );
            Vocab* voc = get_owner()->find_vocab( vocab );
            if( voc ) {
                type = IFT_vocab;
                if( abbrev == "a" ) {
                    foname = voc->get_style_name( Vocab::style_abbrev );
                } else {
                    foname = voc->get_style_name( Vocab::style_full );
                }
            } else if( vocab.size() ) {
                // Look for universal number formatting.
                char ch = *vocab.begin();
                if( ch == '!' ) {
                    if( vocab == "!os" ) {
                        StringStyle ss = ( abbrev == "u" ) ? SS_uppercase : SS_undefined;
                        foname = get_ordinal_suffix_style( ss );
                    } else if( vocab == "!rn" ) {
                        StringStyle ss = ( abbrev == "l" ) ? SS_lowercase : SS_undefined;
                        foname += get_roman_numerals_style( ss );
                    } else if( vocab == "!lp" ) {
                        foname = get_left_pad_style( foname, abbrev );
                    }
                } else if( ch == '+' ) {
                    if( vocab == "+os" ) {
                        StringStyle ss = ( abbrev == "u" ) ? SS_uppercase : SS_undefined;
                        foname += get_ordinal_suffix_style( ss );
                    }
                }
            }
            fieldout += foname;

            if( dname.size() ) {
                fieldout += "/" + get_owner()->get_num_code_alias( dname );

                if( usefor_input ) {
                    input += "/";
                    m_input_fields.push_back( fieldname );
                    m_vocabs.push_back( NULL );
                    m_types.push_back( IFT_dual1 );
                }

                fieldname = get_owner()->get_field_alias( dname );
                type = IFT_dual2;
            }
            if( usefor_input ) {
                m_input_fields.push_back( fieldname );
                m_vocabs.push_back( voc );
                m_types.push_back( type );
            }

            fname.clear();
            dname.clear();
            vocab.clear();
            abbrev.clear();
            state = dooutput;
        } else if( state == dofname && *it == '/' ) {
            state = dodname;
        } else if( state == dofname && *it == ':' ) {
            state = dovocab;
        } else if( state == dovocab && *it == '.' ) {
            state = doabbrev;
        } else {
            switch( state )
            {
            case dofname:  fname += *it;  break;
            case dodname:  dname += *it;  break;
            case dovocab:  vocab += *it;  break;
            case doabbrev: abbrev += *it; break;
            default: break;
            }
        }
    }
    if( usefor_output ) {
        output += fieldout;
    }
    set_user_input_str( input );
    set_user_output_str( output );
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
    if( m_rank_fieldnames.empty() ) {
        return get_owner()->get_rank_fieldnames();
    }
    return m_rank_fieldnames;
}

bool FormatText::is_tier1( const std::string& fieldname ) const
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

Field FormatText::get_field( const Record& record, const std::string& fname ) const
{
    string fn = get_owner()->get_field_alias( fname );
    int index = record.get_base()->get_fieldname_index( fn );
    return record.get_field( index );
}

string FormatText::formatted_str( 
    Field field, const string& format, const string& specifier ) const
{
    string result;
    if( !format.empty() ) {
        if( *format.begin() == '!' ) {
            if( format == "!os" ) {
                StringStyle ss = ( specifier == "u" ) ? SS_uppercase : SS_undefined;
                result = get_ordinal_suffix( field, ss );
            } else if( format == "!rn" ) {
                StringStyle ss = ( specifier == "l" ) ? SS_lowercase : SS_undefined;
                result = get_roman_numerals( field, ss );
            } else if( format == "!lp" ) {
                result = get_left_padded( field, specifier );
            }
        } else {
            result = get_owner()->lookup_token( field, format, (specifier == "a") );
        }
    }
    if( result.empty() ) {
        result = field_to_str( field );
        if( format == "+os" && result.size() ) {
            StringStyle ss = ( specifier == "u" ) ? SS_uppercase : SS_undefined;
            result += get_ordinal_suffix( field, ss );
        }
    }
    return result;
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
            Record rec1( base ), rec2( base );
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
    RangeList ranges;
    Record mask( base, input, get_code(), RB_none );
    Range range;
    bool ret = set_range_as_begin( &range, mask );
    while( range.jdn1 != f_invalid ) {
        if( ret ) {
            if( ranges.size() && ranges[ranges.size()-1].jdn2+1 >= range.jdn1 ) {
                ranges[ranges.size()-1].jdn2 = range.jdn2;
            } else {
                ranges.push_back( range );
            }
        }
        ret = set_range_as_next( &range, mask );
    }
    return ranges;
}

// Sets range and returns true if the masks can create a valid range.
// Sets up range but returns false if the masks can create a valid range only
// by ignoring optional fields.
// Sets range to invalid and returns false if the masks cannot create a valid
// range.
bool FormatText::set_range_as_begin( Range* range, const Record& mask ) const
{
    Record rec1( mask.get_base() );
    Record rec2( mask.get_base() );
    bool ret1 = rec1.set_fields_as_begin_first( mask.get_field_ptr(), false );
    bool ret2 = rec2.set_fields_as_begin_last( mask.get_field_ptr(), false );
    if( !ret1 || !ret2 ) {
        range->jdn1 = f_invalid;
        return false;
    }
    range->jdn1 = rec1.get_jdn();
    range->jdn2 = rec2.get_jdn();
    if( range->jdn1 == f_invalid || range->jdn2 == f_invalid ) {
        range->jdn1 = f_invalid;
        return false;
    }
    ret1 = rec1.set_fields_as_begin_first( mask.get_field_ptr(), true );
    ret2 = rec2.set_fields_as_begin_last( mask.get_field_ptr(), true );
    if( ret1 && ret2 ) {
        Range r;
        r.jdn1 = rec1.get_jdn();
        r.jdn2 = rec2.get_jdn();
        if( r.jdn1 != range->jdn1 || r.jdn2 != range->jdn2 ) {
            if( r.jdn1 > r.jdn2 ) {
                return false;
            }
            *range = r;
            return true;
        }
    }
    return ret1 && ret2;
}

// Using the initial value of range, attempts to adjust the value of range
// to the next value.
// If it fails, it sets the range invalid and returns false.
// If it succeeds, it will attempt to correct for optional fields - if this
// fails, the uncorrected range is set and the the function returns false.
bool FormatText::set_range_as_next( Range* range, const Record& mask ) const
{
    Record rec1( mask.get_base(), range->jdn1 );
    Record rec2( mask.get_base(), range->jdn2 );
    bool ret1 = rec1.set_fields_as_next_first( mask.get_field_ptr() );
    bool ret2 = rec2.set_fields_as_next_last( mask.get_field_ptr() );
    if( !ret1 || !ret2 ) {
        range->jdn1 = f_invalid;
        return false;
    }
    range->jdn1 = rec1.get_jdn();
    range->jdn2 = rec2.get_jdn();
    if( range->jdn1 > range->jdn2 ||
        range->jdn1 == f_invalid || range->jdn2 == f_invalid )
    {
        range->jdn1 = f_invalid;
        return false;
    }
    ret1 = rec1.correct_fields_as_first( mask.get_field_ptr() );
    ret2 = rec2.correct_fields_as_last( mask.get_field_ptr() );
    if( ret1 && ret2 ) {
        Range r;
        r.jdn1 = rec1.get_jdn();
        r.jdn2 = rec2.get_jdn();
        if( r.jdn1 != range->jdn1 || r.jdn2 != range->jdn2 ) {
            if( r.jdn1 > r.jdn2 ) {
                return false;
            }
            *range = r;
            return true;
        }
    }
    return ret1 && ret2;
}

// End of src/cal/calformattext.cpp file
