/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calliturgical.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Liturgical calendars functions.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     14th April 2014
 * Copyright:   Copyright (c) 2014 - 2015, Nick Matthews.
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

#include "calmath.h"
#include "calrecord.h"

using namespace Cal;

namespace {

    enum WeekBlockNumber {
        WEEK_Blk1 = 2, WEEK_Blk2 = 3, WEEK_Blk3 = 9,
        WEEK_Blk4a = 54, WEEK_Blk4 = 58, WEEK_Blk5 = 62 
    };

    Field xmas1( const Julian* base, Field year )
    {
        return kday_on_or_after( WDAY_Sunday, base->jdn( year, 12, 25 ) );
    }

    Field xmas2( const Julian* base, Field year )
    {
        return kday_on_or_after( WDAY_Sunday, base->jdn( year, 1, 1 ) );
    }

    Field epiph( const Julian* base, Field year )
    {
        return kday_on_or_after( WDAY_Sunday, base->jdn( year, 1, 6 ) );
    }

    Field epiph1( const Julian* base, Field year )
    {
        return kday_after( WDAY_Sunday, base->jdn( year, 1, 6 ) );
    }

    Field septuag( const Julian* base, Field year )
    {
        return base->easter( year ) - 63;
    }

    Field advent( const Julian* base, Field year )
    {
        return kday_nearest( WDAY_Sunday, base->jdn( year, 11, 30 ) );
    }

}

Field Cal::liturgical_get_litweek( const Julian* base, Field jdn )
{
    const int year_index = 0;
    Field sunday = kday_on_or_before( WDAY_Sunday, jdn );
    Record rec( base, sunday );
    Field year = rec.get_field( year_index );
    Field b0 = xmas1( base, year-1 );
    Field b1 = epiph( base, year );
    Field b2 = epiph1( base, year );
    Field b3 = septuag( base, year );
    Field b4 = advent( base, year );
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
    return week;
}

Field Cal::liturgical_get_jdn( const Julian* base, Field year, Field litweek )
{
    Field jdn = f_invalid;
    // First check the week starting the end of the previous year
    // and deal with it if necessary.
    if( litweek == WEEK_Blk5 ) {
        Field yearstart = base->jdn( year, 1, 1 );
        jdn = xmas1( base, year-1 );
        if( jdn >= yearstart ) {
            return jdn;
        }
    }
    if( litweek < WEEK_Blk1 ) {
        jdn = xmas2( base, year ) + ( litweek - 1 ) * 7;
    } else if( litweek < WEEK_Blk2 ) {
        jdn = epiph( base, year ) + ( litweek - WEEK_Blk1 ) * 7;
    } else if( litweek < WEEK_Blk3 ) {
        jdn = epiph1( base, year ) + ( litweek - WEEK_Blk2 ) * 7;
    } else if( litweek < WEEK_Blk4 ) {
        jdn = septuag( base, year ) + ( litweek - WEEK_Blk3 ) * 7;
    } else { // must be block 4 or 5
        jdn = advent( base, year ) + ( litweek - WEEK_Blk4 ) * 7;
    }
    return jdn;
}

// End of src/cal/calliturgical.cpp file
