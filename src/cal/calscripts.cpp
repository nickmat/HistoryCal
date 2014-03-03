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
    "vocab w {name Weekday names; lang en; style-name Weekday WDay;"
    " tokens {"
    "  1 Monday Mon; 2 Tuesday Tue; 3 Wednesday Wed;"
    "  4 Thursday Thur; 5 Friday Fri; 6 Saturday Sat;"
    "  7 Sunday Sun;"
    " };"
    "};\n"

    "grammar d {"
    " alias field { Day day; WDay wday; };"
    " alias format-number-code { WDay w; Day ddddd; };"
    " vocabs w;"
    " format pref @(1:Day);"
    " format @(1:Day)@ (2:WDay:w.a);"
    "};\n"

    "scheme jdn {name Julian Day Number; base jdn; grammar d;};\n"

    "vocab m {"
    " name Month names; lang en; style-name Month Mon;"
    " tokens {"
    "  1 January Jan; 2 Febuary Feb; 3 March Mar;"
    "  4 April Apr; 5 May; 6 June Jun;"
    "  7 July Jul; 8 August Aug; 9 September Sep;"
    "  10 October Oct; 11 November Nov; 12 December Dec;"
    " };"
    "};\n"

    "grammar j {"
    " alias field { WDay wday; Day day; Month month; Year year; };"
    " alias format-number-code { WDay w; Day dd; Month mm; Year yyyy; };"
    " alias unit { d day; m month; y year; w week; };"
    " vocabs m w;"
    " format pref @(3:Day) @(2:Month:m.a) @(1:Year);"
    " format @(4:WDay:w.a) @(3:Day) @(2:Month:m.a) @(1:Year);"
    " format @(3:Day) @(2:Month:m) @(1:Year);"
    " format @(4:WDay:w) @(3:Day) @(2:Month:m) @(1:Year);"
    " format @(2:Month:m.a) @(3:Day), @(1:Year);"
    " format @(1:Year)@:(2:Month)@:(3:Day);"
    "};\n"

    "scheme jb {name Julian Base; base julian; grammar j;};\n"
    "scheme g {name Gregorian; base gregorian; grammar j;};\n"

    "grammar j.sh {"
    " alias field { Day day; Month month; Year s.year; Julian-Year unshift; };"
    " alias field { Day day; Month month; Year year; };"
    " vocabs m;"
    " format pref @(3:Day) @(2:Month:m.a) @(1:Year/Julian-Year);"
    " format @(3:Day) @(2:Month:m.a) @(1:Year);"
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

    "vocab er {name British Regnal Names; lang en; style-name Reign Rn;"
    " tokens {"
    "  0 \"Pre Regnal\" Pre;"
    "  1 Anne;"
    "  2 \"George I\" \"Geo I\";"
    "  3 \"George II\" \"Geo II\";"
    "  4 \"George III\" \"Geo III\";"
    "  5 \"George IV\" \"Geo IV\";"
    "  6 \"William IV\" \"Wm IV\";"
    "  7 Victora Vic;"
    "  8 \"Edward VII\" \"Ed VII\";"
    "  9 \"George V\" \"Geo V\";"
    "  10 \"Edward VIII\" \"Ed VIII\";"
    "  11 \"George VI\" \"Geo VI\";"
    "  12 \"Elizabeth II\" \"Eliz II\";"
    " };"
    "};\n"

    "grammar er {"
    " alias field {Monarch era; Day day; Month month; Year year;};"
    " vocabs er m;"
    " format pref @(4:Day) @(3:Month:m.a) @(2:Year) @(1:Monarch:er);"
    " format @(2:Year) @(1:Monarch:er) @(4:Day) @(3:Month:m.a);"
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
