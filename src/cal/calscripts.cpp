/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscripts.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Default scripts.
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

#include "calscripts.h"

const char* Cal::cal_default_script =
    "grammar d {"
    " alias field { Day day; };"
    " format pref @(1:Day);"
    "};\n"

    "scheme jdn {name Julian Day Number; base jdn; grammar d;};\n"

    "vocab m1 {name Short Month names; style abrev; lang en; field Mon;"
    " tokens {1 Jan; 2 Feb; 3 Mar; 4 Apr; 5 May; 6 Jun;"
    " 7 Jul; 8 Aug; 9 Sep; 10 Oct; 11 Nov; 12 Dec;};};\n"
    "vocab m2 {name Full Month names; style full; lang en; field Month;"
    " tokens {1 January; 2 Febuary; 3 March; 4 April; 5 May; 6 June;"
    " 7 July; 8 August; 9 September; 10 October; 11 November; 12 December;};};\n"

    "vocab w1 {name Short Weekday names; style abrev; lang en; field WDay;"
    " tokens {1 Mon; 2 Tue; 3 Wed; 4 Thur; 5 Fri; 6 Sat; 7 Sun;};};\n"
    "vocab w2 {name Full Weekday names; style full; lang en; field Weekday;"
    " tokens {1 Monday; 2 Tuesday; 3 Wednesday;"
    " 4 Thursday; 5 Friday; 6 Saturday; 7 Sunday;};};\n"

    "grammar j {"
    " alias field { WDay wday; Day day; Month month; Year year; };"
    " alias unit { d day; m month; y year; w week; };"
    " vocabs m2 m1 w1 w2;"
    " format pref @(3:Day) @(2:Month:m1) @(1:Year);"
    " format @(4:WDay:w1) @(3:Day) @(2:Month:m1) @(1:Year);"
    " format @(3:Day) @(2:Month:m2) @(1:Year);"
    " format @(4:WDay:w2) @(3:Day) @(2:Month:m2) @(1:Year);"
    " format @(2:Month:m1) @(3:Day), @(1:Year);"
    " format @(1:Year)@:(2:Month)@:(3:Day);"
    "};\n"

    "scheme jb {name Julian Base; base julian; grammar j;};\n"
    "scheme g {name Gregorian; base gregorian; grammar j;};\n"

    "grammar j.sh {"
    " alias field { Day day; Month month; Year s.year; Julian-Year unshift; };"
    " alias field { Day day; Month month; Year year; };"
    " vocabs m2 m1;"
    " format pref @(3:Day) @(2:Month:m1) @(1:Year/Julian-Year);"
    " format @(3:Day) @(2:Month:m1) @(1:Year);"
    "};\n"

    "scheme j325 {name Julian Lady Day; shift jb 1721142; grammar j.sh;};\n"

    "scheme eng1 {name English Hybrid;" 
    " hybrid {fields s.year year month day;"
    " scheme j325;"
    " scheme jb {begin 2360975; match {s.year year; month month; day day;};};"
    " scheme g {begin 2361222; match {s.year year; month month; day day;};};};"
    " grammar j.sh;};\n"

    "scheme eng2 {name English Julian/Gregorian Change;" 
    " hybrid {fields year month day;"
    " scheme jb;"
    " scheme g {begin 2361222; match {year year; month month; day day;};};};"
    " grammar j;};\n"


    "vocab er {name British Regnal Names; style short; lang en; field Reign;"
    " tokens {"
    "  0 Pre Regnal;"
    "  1 Anne; 2 George I; 3 George II; 4 George III; 5 George IV; 6 William IV;"
    "  7 Victora; 8 Edward VII; 9 George V; 10 Edward VIII; 11 George VI;"
    "  12 Elizabeth II;"
    " };"
    "};\n"

    "grammar er {"
    " alias field {Monarch era; Day day; Month month; Year year;};"
    " vocabs er m1 m2;"
    " format pref @(4:Day) @(3:Month:m1) @(2:Year) @(1:Monarch:er);"
    " format @(2:Year) @(1:Monarch:er) @(4:Day) @(3:Month:m1);"
    "};\n"

    "scheme er {name English Regnal;"
    " era eng2 year {"
    "  1721507 -2147483646;"                                // 0
    "  2342780 2342780; 2347309 2347309; 2352006 2352006;"  // 1 2 3
    "  2364175 2364175; 2385829 2385829; 2389630 2389630;"  // 4 5 6
    "  2392181 2392181; 2415407 2415407; 2418798 2418798;"  // 7 8 9
    "  2428188 2428188; 2428514 2428514; 2434049 2434049;"  // 10 11 12
    " };"
    " grammar er;"
    "};\n"

    "scheme test {name Test Julian Annunciation;"
    " era jb year {1721507 -2147483646;};"
    " grammar j;"
    "};\n"
;

// End of src/cal/calscripts.cpp file
