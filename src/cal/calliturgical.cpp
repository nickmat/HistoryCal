/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calliturgical.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base for Liturgical calendars.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     14th April 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
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

    Day numbers 1 = Monday ... 7 = Sunday

    Week numbers

         62  1st Week of Christmas      1st Sunday on or after 25 Dec
    ----------- Year Change ------------
        * 1  2nd Week of Christmas
  Blk1  * 2  Epiphany
  Blk2    3  1st Week of Epiphany
        * 4  2nd Week of Epiphany
        * 5  3rd Week of Epiphany
        * 6  4th Week of Epiphany
        * 7  5th Week of Epiphany
        * 8  6th Week of Epiphany
  Blk3    9  Septuagesima
         10  Sexagesima
         11  Quinquagesima
         12  Quadragesima
         13  2nd Week of Lent
         14  3rd Week of Lent
         15  4th Week of Lent
         16  5th (Passion)
         17  6th (Palm) (Holy Week)
         18  Easter Week
         19  Quasimodo
         20  2nd Week after Easter
         21  3rd Week after Easter
         22  4th Week after Easter
         23  Rogation
         24  1st Week after Ascension
         25  Whitsun
         26  Trinity
         27  1st Week after Trinity
         28  2nd Week after Trinity
         29  3rd Week after Trinity
         30  4th Week after Trinity
         31  5th Week after Trinity
         32  6th Week after Trinity
         33  7th Week after Trinity
         34  8th Week after Trinity
         35  9th Week after Trinity
         36  10th Week after Trinity
         37  11th Week after Trinity
         38  12th Week after Trinity
         39  13th Week after Trinity
         40  14th Week after Trinity
         41  15th Week after Trinity
         42  16th Week after Trinity
         43  17th Week after Trinity
         44  18th Week after Trinity
         45  19th Week after Trinity
         46  20th Week after Trinity
         47  21st Week after Trinity
         48  22nd Week after Trinity
        *49  23rd Week after Trinity
        *50  24th Week after Trinity
        *51  25th Week after Trinity
        *52  26th Week after Trinity
        *53  27th Week after Trinity
  Blk4a *54  4th Week before Advent
        *55  3rd Week before Advent
        *56  2nd Week before Advent
        *57  1st Week before Advent
  Blk4   58  1st Week of Advent
         59  2nd Week of Advent
         60  3rd Week of Advent
         61  4th Week of Advent
  Blk5   62  1st Week of Christmas

    ------- Next Liturgical Year --------
        * 1  2nd Week of Christmas
  Blk1  * 2  Epiphany
  Blk2    3  1st Week of Epiphany

    * Optional weeks.

    Block numbers
    Blk1  (Epiph)     If [base: year 1 6] is a Sunday
    Blk2  (Epiph 1)   First sunday after [base: year 1 6]
    Blk3  (Septuag)   Ninth Sunday before Easter [base: Easter(year) - 63]
    Blk4a (Advent -4) Nearest Sunday to [base: year ?? ??]
    Blk4  (Advent 1)  Nearest Sunday to [base: year 11 30]
*/

#include "calliturgical.h"

#include "calgregorian.h"
#include "calmath.h"
//#include "calparse.h"
#include "calrecord.h"
//#include "calscheme.h"
//#include "calschemes.h"

//#include <cassert>

using namespace Cal;
using namespace std;

enum WeekBlockNumber {
    WEEK_Blk1 = 2, WEEK_Blk2 = 3, WEEK_Blk3 = 9,
    WEEK_Blk4a = 54, WEEK_Blk4 = 58, WEEK_Blk5 = 62 
};


Field Cal::liturgical_get_week( const Base* base, Field jdn )
{
    Field sunday_jdn = kday_on_or_before( WDAY_Sunday, jdn );
//    Record rec( base, sunday_jdn );
    return f_invalid;
#if 0
    Field year = rec.get_field( m_year_index );
    Field sunday = kday_on_or_before( WDAY_Sunday, jdn );
    Field b0 = xmas1( year-1 );
    Field b1 = epiph( year );
    Field b2 = epiph1( year );
    Field b3 = septuag( year );
    Field b4 = advent( year );
    Field week;
    if( sunday < b1 ) {
        week = 1 + ( sunday - b0 ) / 7;
    } else if( sunday < b2 ) {
        week = WEEK_Blk1 + ( sunday - b1 ) / 7;
    } else if( sunday < b3 ) {
        week = WEEK_Blk2 + ( sunday - b2 ) / 7;
    } else if( sunday < b4 ) {
        week = WEEK_Blk3 + ( sunday - b3 ) / 7;
    } else {
        week = WEEK_Blk4 + ( sunday - b4 ) / 7;
    }


#if 0
    Field b2 = xmas1( year-1 );
    Field b3 = epiph( year );
    Field b4 = epiph1( year );
    Field b5 = septuag( year );
    Field b1 = advent( year );
    Field week;
    if( sunday < b3 ) {
        week = WEEK_B2 + ( sunday - b2 ) / 7;
    } else if( sunday < b4 ) {
        week = WEEK_B3 + ( sunday - b3 ) / 7;
    } else if( sunday < b5 ) {
        week = WEEK_B4 + ( sunday - b4 ) / 7;
    } else if( sunday < b1 ) {
        week = WEEK_B5 + ( sunday - b5 ) / 7;
    } else { //if( sunday < b2 ) {
        week = WEEK_B1 + ( sunday - b1 ) / 7;
    }
#endif
    fields[0] = year;
    fields[1] = week;
    fields[2] = day_of_week( jdn ) + 1;
    fields[3] = rec.get_field( m_month_index );
#endif
}




#if 0

Liturgical::Liturgical( Schemes* schs, const string& data )
    : m_base(NULL), Base()
{
    string body;
    string word = get_first_word( data, &body );
    Scheme* sch = schs->get_scheme( word );
    assert( sch != NULL );
    m_base = sch->get_base();
    assert( m_base != NULL );
    m_year_index = m_base->get_fieldname_index( "unshift" );
    if( m_year_index < 0 ) {
        m_year_index = m_base->get_fieldname_index( "year" );
    }
    m_month_index = m_base->get_fieldname_index( "month" );
}

Liturgical::~Liturgical()
{
}

int Liturgical::get_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "year" ) {
        return 0;
    } else if( fieldname == "week" ) {
        return 1;
    } else if( fieldname == "wday" ) {
        return 2;
    } else if( fieldname == "month" ) {
        return 3;
    }
    return get_extended_fieldname_index( fieldname );
}

string Liturgical::get_fieldname( size_t index ) const
{
    const char* fnames[] = { "year", "week", "wday", "month" };
    if( index < 4 ) {
        return fnames[index];
    }
    return get_extended_fieldname( index );
}

bool Liturgical::set_fields_as_begin_first( Field* fields, const Field* mask )
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] == f_invalid ) {
        Field yearstart[3] = { mask[0], 1, 1 };
        Record rec( m_base, &yearstart[0], 3 );
        Record rec2( this, rec.get_jdn() );
        copy_fields( fields, rec2.get_field_ptr() );
        return true;
    }
    if( mask[2] == f_invalid ) {
        fields[0] = mask[0];
        fields[1] = mask[1];
        fields[2] = 7; // Sunday
        return true;
    }
    if( mask[1] == f_invalid ) {
        return false; // "x ? x" pattern not supported yet
    }
    copy_fields( fields, mask );
    return true;
}

bool Liturgical::set_fields_as_next_first( Field* fields, const Field* mask )
{
    return false;
}

bool Liturgical::set_fields_as_begin_last( Field* fields, const Field* mask )
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] == f_invalid ) {
        Field yearstart[3] = { mask[0], 12, 31 };
        Record rec( m_base, &yearstart[0], 3 );
        Record rec2( this, rec.get_jdn() );
        copy_fields( fields, rec2.get_field_ptr() );
        return true;
    }
    if( mask[2] == f_invalid ) {
        fields[0] = mask[0];
        fields[1] = mask[1];
        fields[2] = 6; // Satarday
        return true;
    }
    if( mask[1] == f_invalid ) {
        return false; // "x ? x" pattern not supported yet
    }
    copy_fields( fields, mask );
    return true;
}

bool Liturgical::set_fields_as_next_last( Field* fields, const Field* mask )
{
    return false;
}

Field Liturgical::get_jdn( const Field* fields ) const
{
    Field year = fields[0];
    Field week = fields[1];
    Field day = fields[2];
    Field jdn;
    // First check the week starting the end of the previous year
    // and deal with it if necessary.
    if( week == WEEK_Blk5 ) {
        Field yearstart = base_get_jdn( year, 1, 1 );
        jdn = xmas1( year-1 ) + (day%7);
        if( jdn >= yearstart ) {
            return jdn;
        }
    }
    if( week < WEEK_Blk1 ) {
        jdn = xmas2( year ) + ( week - 1 ) * 7;
    } else if( week < WEEK_Blk2 ) {
        jdn = epiph( year ) + ( week - WEEK_Blk1 ) * 7;
    } else if( week < WEEK_Blk3 ) {
        jdn = epiph1( year ) + ( week - WEEK_Blk2 ) * 7;
    } else if( week < WEEK_Blk4 ) {
        jdn = septuag( year ) + ( week - WEEK_Blk3 ) * 7;
    } else { // must be block 4 or 5
        jdn = advent( year ) + ( week - WEEK_Blk4 ) * 7;
    }
    return jdn + (day%7);
}

void Liturgical::set_fields( Field* fields, Field jdn ) const
{
    Record rec( m_base, jdn );
    Field year = rec.get_field( m_year_index );
    Field sunday = kday_on_or_before( WDAY_Sunday, jdn );
    Field b0 = xmas1( year-1 );
    Field b1 = epiph( year );
    Field b2 = epiph1( year );
    Field b3 = septuag( year );
    Field b4 = advent( year );
    Field week;
    if( sunday < b1 ) {
        week = 1 + ( sunday - b0 ) / 7;
    } else if( sunday < b2 ) {
        week = WEEK_Blk1 + ( sunday - b1 ) / 7;
    } else if( sunday < b3 ) {
        week = WEEK_Blk2 + ( sunday - b2 ) / 7;
    } else if( sunday < b4 ) {
        week = WEEK_Blk3 + ( sunday - b3 ) / 7;
    } else {
        week = WEEK_Blk4 + ( sunday - b4 ) / 7;
    }


#if 0
    Field b2 = xmas1( year-1 );
    Field b3 = epiph( year );
    Field b4 = epiph1( year );
    Field b5 = septuag( year );
    Field b1 = advent( year );
    Field week;
    if( sunday < b3 ) {
        week = WEEK_B2 + ( sunday - b2 ) / 7;
    } else if( sunday < b4 ) {
        week = WEEK_B3 + ( sunday - b3 ) / 7;
    } else if( sunday < b5 ) {
        week = WEEK_B4 + ( sunday - b4 ) / 7;
    } else if( sunday < b1 ) {
        week = WEEK_B5 + ( sunday - b5 ) / 7;
    } else { //if( sunday < b2 ) {
        week = WEEK_B1 + ( sunday - b1 ) / 7;
    }
#endif
    fields[0] = year;
    fields[1] = week;
    fields[2] = day_of_week( jdn ) + 1;
    fields[3] = rec.get_field( m_month_index );
}

Field Liturgical::base_get_jdn( Field year, Field month, Field day ) const
{
    Field fields[3] = { year, month, day };
    return m_base->get_jdn( &fields[0] );
}

Field Liturgical::julian_easter( Field year ) const
{
    return f_invalid;
}

Field Liturgical::gregorian_easter( Field year ) const
{
    Field century = year / 100 + 1;
    Field epact =
        (14 + 11*(year%19) - (3*century)/4 + (5+8*century)/25) % 30;
    if( epact == 0 || ( epact == 1 && 10 < (year%19) ) ) {
        epact++;
    }
    Field paschal_moon = gregorian_to_jdn( year, 4, 19 ) - epact;
    return kday_after( WDAY_Sunday, paschal_moon );
}

Field Liturgical::xmas1( Field year ) const
{
    switch( m_base->get_fundamental_base() )
    {
    case FUNDBASE_G:
        return kday_on_or_after( WDAY_Sunday, gregorian_to_jdn( year, 12, 25 ) );
    case FUNDBASE_J:
        return f_invalid;
    }
    return f_invalid;
}

Field Liturgical::xmas2( Field year ) const
{
    switch( m_base->get_fundamental_base() )
    {
    case FUNDBASE_G:
        return kday_on_or_after( WDAY_Sunday, gregorian_to_jdn( year, 1, 1 ) );
    case FUNDBASE_J:
        return f_invalid;
    }
    return f_invalid;
}

Field Liturgical::epiph( Field year ) const
{
    switch( m_base->get_fundamental_base() )
    {
    case FUNDBASE_G:
        return kday_on_or_after( WDAY_Sunday, gregorian_to_jdn( year, 1, 6 ) );
    case FUNDBASE_J:
        return f_invalid;
    }
    return f_invalid;
}

Field Liturgical::epiph1( Field year ) const
{
    switch( m_base->get_fundamental_base() )
    {
    case FUNDBASE_G:
        return kday_after( WDAY_Sunday, gregorian_to_jdn( year, 1, 6 ) );
    case FUNDBASE_J:
        return f_invalid;
    }
    return f_invalid;
}

Field Liturgical::septuag( Field year ) const
{
    Field easter;
    switch( m_base->get_fundamental_base() )
    {
    case FUNDBASE_G:
        easter = gregorian_easter( year );
        break;
    case FUNDBASE_J:
        easter = julian_easter( year );
        break;
    default:
        return f_invalid;
    }
    return easter - 63;
}

Field Liturgical::advent( Field year ) const
{
    switch( m_base->get_fundamental_base() )
    {
    case FUNDBASE_G:
        return kday_nearest( WDAY_Sunday, gregorian_to_jdn( year, 11, 30 ) );
    case FUNDBASE_J:
        return f_invalid;
    }
    return f_invalid;
}

#endif

// End of src/cal/calliturgical.cpp file
