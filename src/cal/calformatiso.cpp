/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformatiso.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     FormatIso class for ISO 8601 input and output.
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

#include "calformatiso.h"

#include "calgregorian.h"
#include "calisoordinal.h"
#include "calisoweek.h"
#include "calparse.h"
#include "calrecord.h"
#include "calsetmath.h"
#include "caltext.h"

#include <cassert>

using namespace Cal;
using std::string;


FormatIso::FormatIso( const string& code, Grammar* gmr, const StringVec& rules )
    : Format( code, gmr ), m_daterep(DR_gregorian), m_extended(true),
    m_yplusminus(false), m_dateset(false), m_ydigits(4)
{
    for( size_t i = 1 ; i < rules.size() ; i++ ) {
        if( rules[i] == "caldate" ) {
            m_daterep = DR_gregorian;
        } else if( rules[i] == "week" ) {
            m_daterep = DR_week;
        } else if( rules[i] == "ordinal" ) {
            m_daterep = DR_ordinal;
        } else if( rules[i] == "basic" ) {
            m_extended = false;
        } else if( rules[i] == "extended" ) {
            m_extended = true;
        } else if( rules[i] == "sign" ) {
            m_yplusminus = true;
        } else if( rules[i] == "nosign" ) {
            m_yplusminus = false;
        } else if( rules[i] == "year4" ) {
            m_ydigits = 4;
        } else if( rules[i] == "year5" ) {
            m_ydigits = 5;
            m_yplusminus = true;
        } else if( rules[i] == "year6" ) {
            m_ydigits = 6;
            m_yplusminus = true;
        } else if( rules[i] == "dateset" ) {
            m_dateset = true;
        }
    }
    string input, output, rep, sign, sep;
    if( m_yplusminus ) {
        sign = "\302\261";
    }
    if( m_ydigits == 5 ) {
        sign += "Y";
    } else if( m_ydigits == 6 ) {
        sign += "YY";
    }
    if( m_extended ) {
        sep = "-";
    }

    if( m_daterep == DR_gregorian ) {
        output = sign + "YYYY" + sep + "MM" + sep + "DD";
        rep = "Date";
    } else if( m_daterep == DR_week ) {
        output = sign + "YYYY" + sep + "Www" + sep + "D";
        rep = "Week";
    } else { // assumed DR_ordinal
        output = sign + "YYYY" + sep + "DDD";
        rep = "Ordinal";
    }
    input = "ISO:8601 " + rep;
    if( !sign.empty() ) {
        input += " " + sign + "YYYY";
    }
    if( m_dateset ) {
        output = "[" + output + "]";
    }
    set_user_input_str( input );
    set_user_output_str( output );
}

string FormatIso::rlist_to_string( Base* base, const RangeList& ranges ) const
{
    string str;
    for( size_t i = 0 ; i < ranges.size() ; i++ ) {
        if( i > 0 ) {
            str += ( m_dateset ? "," : " | " );
        }
        str += range_to_string( base, ranges[i] );
    }
    if( m_dateset ) {
        return "[" + str + "]";
    }
    return str;
}

string FormatIso::range_to_string( Base* base, Range range ) const
{
    if( range.jdn1 == range.jdn2 ) {
        return jdn_to_string( base, range.jdn1 );
    }
    string str1, str2;
    if( range.jdn1 == f_minimum || range.jdn2 == f_maximum ) {
        str1 = jdn_to_string( base, range.jdn1 );
        str2 = jdn_to_string( base, range.jdn2 );
    } else {
        Record rec1( base, range.jdn1 );
        Record rec2( base, range.jdn2 );

        rec1.remove_balanced_fields( &rec2 );
        str1 = get_output( rec1 );
        str2 = get_output( rec2 );
        if( str1 == str2 ) {
            return str1;
        }
    }
    string sep = m_dateset ? ".." : "/";
    return str1 + sep + str2;
}

string FormatIso::jdn_to_string( Base* base, Field jdn ) const
{
    if( jdn == f_minimum || jdn == f_maximum ) {
        return "";
    }
    Record rec( base, jdn );
    return get_output( rec );
}

string FormatIso::get_output( const Record& record ) const
{
    // This format only works with Gregorian, ISO Week or ISO Ordinal schemes
    // so one of the following will valid and the other two NULL.
    const Gregorian* greg = dynamic_cast<const Gregorian*>( record.get_base() );
    const IsoWeek* isow = NULL;
    const IsoOrdinal* isoo = NULL;
    if( greg == NULL ) {
        isow = dynamic_cast<const IsoWeek*>( record.get_base() );
        if( isow == NULL ) {
            isoo = dynamic_cast<const IsoOrdinal*>( record.get_base() );
            if( isoo == NULL ) {
                return false;
            }
        }
    }
    string str = output_year( record.get_field( 0 ) );
    if( str.empty() ) {
        return "";
    }
    Field jdn = f_invalid;
    if( m_daterep == DR_ordinal ) {
        Field oday = f_invalid;
        if( isoo == NULL ) {
            jdn = record.get_jdn();
            IsoOrdinal::from_jdn( NULL, &oday, jdn );
        } else {
            oday = record.get_field( 1 );
        }
        if( oday != f_invalid ) {
            if( m_extended ) {
                str += "-";
            }
            str += get_left_padded( oday, "0", 3 );
        }
        return str;
    }
    if( m_daterep == DR_week ) {
        Field week = f_invalid, wday = f_invalid;
        if( isow == NULL ) {
            jdn = record.get_jdn();
            IsoWeek::from_jdn( NULL, &week, &wday, jdn );
        } else {
            week = record.get_field( 1 );
            wday = record.get_field( 2 );
        }
        if( week != f_invalid ) {
            if( m_extended ) {
                str += "-";
            }
            str += "W" + get_left_padded( week, "0", 2 );
            if( wday != f_invalid ) {
                if( m_extended ) {
                    str += "-";
                }
                str += get_left_padded( wday, "0", 1 );
            }
        }
        return str;
    }
    if( m_daterep != DR_gregorian ) {
        return "";
    }
    Field month = f_invalid, day = f_invalid;
    if( greg == NULL ) {
        jdn = record.get_jdn();
        Gregorian::from_jdn( NULL, &month, &day, jdn );
    } else {
        month = record.get_field( 1 );
        day = record.get_field( 2 );
    }
    if( month != f_invalid ) {
        if( m_extended ) {
            str += "-";
        }
        str += get_left_padded( month, "0", 2 );
        if( day != f_invalid ) {
            if( m_extended ) {
                str += "-";
            }
            str += get_left_padded( day, "0", 2 );
        }
    }
    return str;
}

RangeList FormatIso::string_to_rlist( Base* base, const string& input ) const
{
    if( input[0] == '[' ) {
        return string_set_to_rlist( base, input );
    }
    RangeList rlist;
    Record rec1( base ), rec2( base );
    if( set_input( &rec1, input, RB_begin ) && set_input( &rec2, input, RB_end ) ) {
        Range range( rec1.get_jdn(), rec2.get_jdn() );
        if( range.jdn1 != f_invalid && range.jdn2 != f_invalid ) {
            rlist.push_back( range );
        }
    }
    return rlist;
}

bool FormatIso::set_input( Record* record, const string& input, Boundary rb ) const
{
    // This format only works with Gregorian, ISO Week or ISO Ordinal schemes
    // so one of the following will valid and the other two NULL.
    const Gregorian* greg = dynamic_cast<const Gregorian*>( record->get_base() );
    const IsoWeek* isow = NULL;
    const IsoOrdinal* isoo = NULL;
    if( greg == NULL ) {
        isow = dynamic_cast<const IsoWeek*>( record->get_base() );
        if( isow == NULL ) {
            isoo = dynamic_cast<const IsoOrdinal*>( record->get_base() );
            if( isoo == NULL ) {
                return false;
            }
        }
    }

    Field f[3] = { f_invalid, f_invalid, f_invalid };
    DateRep instyle;
    if( input[0] == 'R' ) {
        instyle = recurring_interval( &f[0], input, rb );
    } else if( input.find( '/' ) != string::npos ) {
        instyle = interval( &f[0], input, rb );
    } else {
        instyle = datetime( &f[0], input, rb );
    }

    if( instyle == DR_null ) {
        return false;
    }

    Field jdn = f_invalid;
    if( instyle == DR_ordinal ) {
        // If there is no day number, it will be treated as Gregorian
        assert( f[1] != f_invalid );
        if( isoo ) {
            record->set_fields( &f[0], 2 );
            return true;
        }
        jdn = IsoOrdinal::to_jdn( f[0], f[1] );
    }
    if( instyle == DR_week && isow == NULL ) {
        // convert to jdn
        IsoWeek wbase;
        Record rec( &wbase );
        if( rb == RB_begin ) {
            rec.set_fields_as_begin_first( &f[0], false );
        } else if( rb == RB_end ) {
            rec.set_fields_as_begin_last( &f[0], false );
        } else {
            rec.set_fields( &f[0], 3 );
        }
        jdn = rec.get_jdn();
        if( jdn == f_invalid ) {
            return false;
        }
    }
    if( instyle == DR_gregorian && greg == NULL ) {
        // convert to jdn
        Gregorian gbase;
        Record rec( &gbase );
        if( rb == RB_begin ) {
            rec.set_fields_as_begin_first( &f[0], false );
        } else if( rb == RB_end ) {
            rec.set_fields_as_begin_last( &f[0], false );
        } else {
            rec.set_fields( &f[0], 3 );
        }
        jdn = rec.get_jdn();
        if( jdn == f_invalid ) {
            return false;
        }
    }

    if( jdn != f_invalid ) {
        record->set_jdn( jdn );
        return true;
    }

    if( rb == RB_begin ) {
        return record->set_fields_as_begin_first( &f[0], false );
    } else if( rb == RB_end ) {
        return record->set_fields_as_begin_last( &f[0], false );
    }
    record->set_fields( &f[0], 3 );
    return true;
}

string FormatIso::output_year( Field year ) const
{
    string sign;
    if( m_yplusminus ) {
        if( year < 0 ) {
            sign = '-';
            year = -year;
        } else {
            sign = "+";
        }
    } else {
        if( year < 0 ) {
            return "";
        }
    }
    string value = get_left_padded( year, "0", m_ydigits );
    if( value.length() != m_ydigits ) {
        return "";
    }
    return sign + value;
}

FormatIso::DateRep FormatIso::recurring_interval( Field* fields, const string& input, Boundary rb ) const
{
    return DR_null;
}

FormatIso::DateRep FormatIso::interval( Field* fields, const string& input, Boundary rb ) const
{
    size_t pos = input.find( '/' );
    string in;
    if( rb == RB_begin ) {
        in = input.substr( 0, pos );
    } else if( rb == RB_end ) {
        in = input.substr( pos + 1 );
    }
    if( in.empty() ) {
        return DR_null;
    }
    return datetime( fields, in, rb );
}

FormatIso::DateRep FormatIso::datetime( Field* fields, const string& input, Boundary rb ) const
{
    // Remove any time elements.
    size_t pos = input.find( 'T' );
    char start = input[0];
    bool neg = ( start == '-' );

    string str;
    if( neg || start == '+' ) {
        str = input.substr( 1, pos );
    } else {
        str = input.substr( 0, pos );
    }

    if( str.length() == m_ydigits - 2 ) {
        if( rb == RB_begin ) {
            str += "00";
        } else if( rb == RB_end ) {
            str += "99";
        }
    }
    if( str.length() == m_ydigits - 1 ) {
        if( rb == RB_begin ) {
            str += "0";
        } else if( rb == RB_end ) {
            str += "9";
        }
    }
    if( str.length() < m_ydigits ) {
        return DR_null;
    }
    
    fields[0] = str_to_field( str.substr( 0, m_ydigits ) );
    if( neg ) {
        fields[0] *= -1;
    }
    str = str.substr( m_ydigits );

    if( str[0] == '-' ) {
        str = str.substr( 1 );
    }

    if( str[0] == 'W' ) {
        return weekdate( fields, str, rb );
    }
    if( str.length() == 3 ) {
        // Ordinal day number
        fields[1] = str_to_field( str );
        return DR_ordinal;
    }

    if( str.length() >= 2 ) {
        fields[1] = str_to_field( str.substr( 0, 2 ) );
        str = str.substr( 2 );
    }

    if( str[0] == '-' ) {
        str = str.substr( 1 );
    }
    if( str.length() == 2 ) {
        fields[2] = str_to_field( str );
        str = str.substr( 2 );
    }
    if( str.empty() ) {
        return DR_gregorian;
    }
    return DR_null;
}

// Note, fields[0] must already be filled in with the year and
// str starts with a 'W' character.
FormatIso::DateRep FormatIso::weekdate( Field* fields, const string& input, Boundary rb ) const
{
    string str = input.substr( 1 ); // Remove the 'W'.

    if( str.length() >= 2 ) {
        fields[1] = str_to_field( str.substr( 0, 2 ) );
        str = str.substr( 2 );
    }

    if( str[0] == '-' ) {
        str = str.substr( 1 );
    }
    if( str.length() == 1 ) {
        fields[2] = str_to_field( str );
        str = str.substr( 1 );
    }
    if( str.empty() ) {
        return DR_week;
    }
    return DR_null;
}

RangeList FormatIso::string_set_to_rlist( Base* base, const string& input ) const
{
    RangeList rlist;
    if( input.size() < 6 ) {
        return rlist;
    }
    string str = input.substr( 1, input.size() - 2 ); // Strip '[' .. ']'
    string rangestr, begval, endval;
    size_t pos1;
    do {
        pos1 = str.find( ',' );
        rangestr = str.substr( 0, pos1 );
        size_t pos2 = rangestr.find( ".." );
        if( pos2 == string::npos ) {
            // single value
            begval = endval = rangestr;
        } else {
            // start and end
            begval = rangestr.substr( 0, pos2 );
            endval = rangestr.substr( pos2 + 2 );
        }
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
        str = str.substr( pos1 + 1 );
    } while( pos1 != string::npos );
    return op_set_well_order( rlist );
}

// End of src/cal/calformatiso.cpp file
