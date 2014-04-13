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
    " format pref @(Day);"
    " alias field { Day day; };"
    " alias format-number-code { Day ddddd; };"
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
    " vocabs m w;"
    " format pref @(Day) @(Month:m.a) @(Year);"
    " format @(WDay:w.a) @(Day) @(Month:m.a) @(Year);"
    " format @(Day) @(Month:m) @(Year);"
    " format @(WDay:w) @(Day) @(Month:m) @(Year);"
    " format @(Month:m.a) @(Day), @(Year);"
    " format @(Year)@:(Month)@:(Day);"
    " alias field { WDay wday; Day day; Month month; Year year; };"
    " alias format-number-code { WDay w; Day dd; Month mm; Year yyyy; };"
    " alias unit { d day; m month; y year; w week; };"
    "};\n"

    "scheme j {name Julian; base julian; grammar j;};\n"
    "scheme g {name Gregorian; base gregorian; grammar j;};\n"

    "grammar j.sh {"
    " vocabs m w;"
    " format @(Day) @(Month:m.a) @(Year);"
    " format pref @(Day) @(Month:m.a) @(Year/Hist-Year);"
    " format @(WDay:w.a) @(Day) @(Month:m.a) @(Year/Hist-Year);"
    " alias field {WDay wday; Day day; Month month; Year year; Hist-Year unshift;};"
    " alias format-number-code {Day dd; Month mm; Year yyyy; Hist-Year y;};"
    "};\n"

    "scheme j325 {name Julian Annunciation; shift j 1721507; grammar j.sh;};\n"

    "vocab ojn {"
    " name Scheme names; lang en; style-name Scheme Sch;"
    " tokens {"
    "  0 \"Old Style\" os; 1 Julian; 2 \"New Style\" ns;"
    " };"
    "};\n"

    "vocab on {"
    " name Scheme names; lang en; style-name Scheme Sch;"
    " tokens {"
    "  0 \"Old Style\" os; 1 \"New Style\" ns;"
    " };"
    "};\n"

    "grammar hy {"
    " alias field {Day day; Month month; Year year; Hist-Year unshift; Scheme scheme;};"
    " alias format-number-code {Day dd; Month mm; Year yyyy; Hist-Year y; Scheme s;};"
    " vocabs m ojn;"
    " format pref @(Day) @(Month:m.a) @(Year/Hist-Year);"
    " format @(Day) @(Month:m.a) @(Year);"
    " format @(Day) @(Month:m.a) @(Year) @(Scheme:ojn.a);"
    "};\n"

    "scheme eng {name English Hybrid;"
    " hybrid {"
    "  fields year month day unshift;"
    "  scheme j325;"
    "  scheme j {begin 2360975;};"
    "  scheme g {begin 2361222;};"
    " };"
    " grammar hy;"
    "};\n"

    "vocab er {name English Monarchs; lang en; style-name Reign Rn;"
    " tokens {"
    "  0 \"English calendar\" Eng;"
    "  1 \"William I\" \"Wm I\";"
    "  2 \"William II\" \"Wm II\";"
    "  3 \"Henry I\" \"Hy I\";"
    "  4 Stephen St;"
    "  5 \"Henry II\" \"Hy II\";"
    "  6 \"Richard I\" \"Rich I\";"
    "  7 John Jn;"
    "  8 \"Henry III\" \"Hy III\";"
    "  9 \"Edward I\" \"Ed I\";"
    "  10 \"Edward II\" \"Ed II\";"
    "  11 \"Edward III\" \"Ed III\";"
    "  12 \"Richard II\" \"Rich II\";"
    "  13 \"Henry IV\" \"Hy IV\";"
    "  14 \"Henry V\" \"Hy V\";"
    "  15 \"Henry VI\" \"Hy VI\";"
    "  16 \"Edward IV\" \"Ed IV\";"
    "  17 \"Henry VI Restoration\" \"Hy VI R\";"
    "  18 \"Edward V\" \"Ed V\";"
    "  19 \"Richard III\" \"Rich III\";"
    "  20 \"Henry VII\" \"Hy VII\";"
    "  21 \"Henry VIII\" \"Hy VIII\";"
    "  22 \"Edward VI\" \"Ed VI\";"
    "  23 Jane;"
    "  24 \"Mary I\";"
    "  25 \"Philip and Mary\" \"P & M\";"
    "  26 \"Elizabeth I\" \"Eliz I\";"
    "  27 \"James I\" \"Jm I\";"
    "  28 \"Charles I\" \"Chas I\";"
    "  29 \"The Commonwealth\" \"Com\";"
    "  30 \"Charles II\" \"Chas II\";"
    "  31 \"James II\" \"Jm II\";"
    "  32 Interregnum Int;"
    "  33 \"William and Mary\" \"W & M\";"
    "  34 \"William III\" \"Wm III\";"
    "  35 Anne;"
    "  36 \"George I\" \"Geo I\";"
    "  37 \"George II\" \"Geo II\";"
    "  38 \"George III\" \"Geo III\";"
    "  39 \"George IV\" \"Geo IV\";"
    "  40 \"William IV\" \"Wm IV\";"
    "  41 Victora Vic;"
    "  42 \"Edward VII\" \"Ed VII\";"
    "  43 \"George V\" \"Geo V\";"
    "  44 \"Edward VIII\" \"Ed VIII\";"
    "  45 \"George VI\" \"Geo VI\";"
    "  46 \"Elizabeth II\" \"Eliz II\";"
    " };"
    "};\n"

    "grammar er {"
    " alias field {Monarch era; Day day; Month month; Year year; Scheme scheme;};"
    " vocabs er m on;"
    " format pref @(Day) @(Month:m.a) @(Year) @(Monarch:er) @,(Scheme:sch);"
    " format @(Year) @(Monarch:er) @(Day) @(Month:m.a);"
    "};\n"

    "scheme t2 {name Test Shift 2; shift j 2352006; grammar j.sh;};\n"
    "scheme t3 {name Test Shift 3; shift g 2352006; grammar j.sh;};\n"
    "scheme t4 {name Test Hybrid;"
    " hybrid {"
    "  fields year month day;"
    "  scheme t2;"
    "  scheme t3 {begin 2361222;};"
    " };"
    " grammar hy;"
    "};\n"

    "scheme er {"
    " name English Regnal;"
    " regnal {"
    "  fields year month day unshift scheme;"
    "  default eng;"
    "  schemes {"
    "   {range 2110773 2118336; scheme 1 {shift j 2110773;};};"   // William I
    "   {range 2118353 2123047; scheme 2 {shift j 2118353;};};"   // William II
    "   {range 2123050 2135951; scheme 3 {shift j 2123050;};};"   // Henry I
    "   {range 2135972 2142854; scheme 4 {shift j 2135972;};};"   // Stephen
    "   {range 2142909 2155527; scheme 5 {shift j 2142909;};};"   // Henry II
    "   {range 2155586 2159088; scheme 6 {shift j 2155586;};};"   // Richard I
    "   {range 2159139 2165494; scheme 7 {shift j 2159139;};};"   // John
    "   {range 2165503 2185976; scheme 8 {shift j 2165503;};};"   // Henry III
    "   {range 2185980 2198627; scheme 9 {shift j 2185980;};};"   // Edward I
    "   {range 2198628 2205764; scheme 10 {shift j 2198628;};};"  // Edward II
    "   {range 2205769 2224179; scheme 11 {shift j 2205769;};};"  // Edward III
    "   {range 2224180 2232314; scheme 12 {shift j 2224180;};};"  // Richard II
    "   {range 2232315 2237235; scheme 13 {shift j 2232315;};};"  // Henry IV
    "   {range 2237236 2240686; scheme 14 {shift j 2237236;};};"  // Henry V
    "   {range 2240687 2254751; scheme 15 {shift j 2240687;};};"  // Henry VI
    "   {range 2254751 2262822; scheme 16 {shift j 2254751;};};"  // Edward IV
    "   {range 2258251 2258441; scheme 17 {shift j 2240687;};};"  // Henry VI Restoration
    "   {range 2262822 2262899; scheme 18 {shift j 2262822;};};"  // Edward V
    "   {range 2262900 2263688; scheme 19 {shift j 2262900;};};"  // Richard III
    "   {range 2263688 2272331; scheme 20 {shift j 2263688;};};"  // Henry VII
    "   {range 2272332 2286127; scheme 21 {shift j 2272332;};};"  // Henry VIII
    "   {range 2286127 2288478; scheme 22 {shift j 2286127;};};"  // Edward VI
    "   {range 2288478 2288491; scheme 23 {shift j 2288478;};};"  // Jane
    "   {range 2288491 2288861; scheme 24 {shift j 2288478;};};"  // Mary I
    "   {range 2288862 2290438; scheme 25 {shift j 2288862;};};"  // Philip and Mary
    "   {range 2290438 2306636; scheme 26 {shift j 2290438;};};"  // Elizabeth I
    "   {range 2306636 2314675; scheme 27 {shift j 2306636;};};"  // James I
    "   {range 2314675 2323385; scheme 28 {shift j 2314675;};};"  // Charles I
    "   {range 2323386 2327521; scheme j;};"                      // The Commonwealth
    "   {range 2327522 2336541; scheme 30 {shift j 2323385;};};"  // Charles II
    "   {range 2336541 2337945; scheme 31 {shift j 2336541;};};"  // James II
    "   {range 2337946 2338008; scheme j;};"                      // Interregnum
    "   {range 2338009 2340152; scheme 33 {shift j 2338009;};};"  // William and Mary
    "   {range 2340153 2342780; scheme 34 {shift j 2338009;};};"  // William III
    "   {range 2342780 2347309; scheme 35 {shift j 2342780;};};"  // Anne
    "   {range 2347309 2352006; scheme 36 {shift j 2347309;};};"  // George I
    "   {range 2352006 2364185; scheme t4;};"                     // George II
    "   {range 2364185 2385829; scheme 38 {shift g 2364185;};};"  // George III
    "   {range 2385829 2389630; scheme 39 {shift g 2385829;};};"  // George IV
    "   {range 2389630 2392181; scheme 40 {shift g 2389630;};};"  // William IV
    "   {range 2392181 2415407; scheme 41 {shift g 2392181;};};"  // Victoria
    "   {range 2415407 2418798; scheme 42 {shift g 2415407;};};"  // Edward VII
    "   {range 2418798 2428188; scheme 43 {shift g 2418798;};};"  // George V
    "   {range 2428188 2428514; scheme 44 {shift g 2428188;};};"  // Edward VIII
    "   {range 2428514 2434049; scheme 45 {shift g 2428514;};};"  // George VI
    "   {range 2434049 2457023; scheme 46 {shift g 2434049;};};"  // Elizabeth II
    "  };"
    " };"
    " grammar er;"
    "};\n"

    "scheme t1 {name Test Hybrid;"
    " hybrid {"
    "  fields year month day;"
    "  scheme g;"
    "  scheme j {begin 2361222;};"
    " };"
    " grammar hy;"
    "};\n"
#if 0
    "scheme t2 {name Test Shift 2; shift j 2352006; grammar j.sh;};\n"
    "scheme t3 {name Test Shift 3; shift g 2352006; grammar j.sh;};\n"
    "scheme t4 {name Test Hybrid;"
    " hybrid {"
    "  fields year month day;"
    "  scheme t2;"
    "  scheme t3 {begin 2361222;};"
    " };"
    " grammar hy;"
    "};\n"
#endif
//    "scheme s1 {name Shift 1; shift g 2428514; grammar j.sh;};\n"
;

// End of src/cal/calscripts.cpp file
