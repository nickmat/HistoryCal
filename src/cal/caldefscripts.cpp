/* ../src/cal/caldefscripts.cpp - File created by file2cpp 1.0.0 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        scripts/caldefscripts.f2c
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     file2cpp template file for default scripts.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th September 2014
 * Copyright:   Copyright (c) 2014 ~ 2020, Nick Matthews.
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

 This template is used to create the src/cal/caldefscripts.cpp file.

*/

#include "caldefscripts.h"

Cal::ScriptModule Cal::cal_default_scripts[] = {
    { "stdlib",  /* stdlib.hcs */
 "mark \"__:stdlib:__\";\n" },
    { "base",  /* base.hcs */
 "lexicon w {\n"
 "name \"Weekday names\";\n"
 "fieldname wday;\n"
 "lang en;\n"
 "pseudo Weekday, WDay;\n"
 "tokens {\n"
 "1, \"Monday\", \"Mon\";\n"
 "2, \"Tuesday\", \"Tue\";\n"
 "3, \"Wednesday\", \"Wed\";\n"
 "4, \"Thursday\", \"Thur\";\n"
 "5, \"Friday\", \"Fri\";\n"
 "6, \"Saturday\", \"Sat\";\n"
 "7, \"Sunday\", \"Sun\";\n"
 "}\n"
 "}\n"
 "lexicon ws {\n"
 "name \"Weekday names\";\n"
 "fieldname wsday;\n"
 "lang en;\n"
 "pseudo Weekday, WDay;\n"
 "tokens {\n"
 "1, \"Sunday\", \"Sun\";\n"
 "2, \"Monday\", \"Mon\";\n"
 "3, \"Tuesday\", \"Tue\";\n"
 "4, \"Wednesday\", \"Wed\";\n"
 "5, \"Thursday\", \"Thur\";\n"
 "6, \"Friday\", \"Fri\";\n"
 "7, \"Saturday\", \"Sat\";\n"
 "}\n"
 "}\n" },
    { "jdn",  /* jdn.hcs */
 "grammar d {\n"
 "fields day;\n"
 "alias pseudo {\n"
 "ddddd, day;\n"
 "}\n"
 "format d, \"(day)\";\n"
 "pref d;\n"
 "}\n"
 "scheme jdn {\n"
 "name \"Julian Day Number\";\n"
 "base jdn;\n"
 "grammar d;\n"
 "}\n" },
    { "julian",  /* julian.hcs */
 "lexicon m {\n"
 "name \"Month names\";\n"
 "fieldname month;\n"
 "lang en;\n"
 "pseudo Month, Mon;\n"
 "tokens {\n"
 "1, \"January\", \"Jan\";\n"
 "2, \"February\", \"Feb\";\n"
 "3, \"March\", \"Mar\";\n"
 "4, \"April\", \"Apr\";\n"
 "5, \"May\";\n"
 "6, \"June\", \"Jun\";\n"
 "7, \"July\", \"Jul\";\n"
 "8, \"August\", \"Aug\";\n"
 "9, \"September\", \"Sep\";\n"
 "10, \"October\", \"Oct\";\n"
 "11, \"November\", \"Nov\";\n"
 "12, \"December\", \"Dec\";\n"
 "}\n"
 "}\n"
 "grammar j {\n"
 "fields year month day;\n"
 "optional wday;\n"
 "lexicons m, w;\n"
 "alias field {\n"
 "WDay, wday;\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, year;\n"
 "}\n"
 "alias pseudo {\n"
 "w, WDay;\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "}\n"
 "alias unit {\n"
 "d, day;\n"
 "m, month;\n"
 "y, year;\n"
 "wd, wday;\n"
 "w, week;\n"
 "}\n"
 "format dmy, \"(Day) |(Month:m:a) |(Year)\";\n"
 "format wdmy, \"(WDay:w:a) |(Day) |(Month:m:a) |(Year)\";\n"
 "format out {\n"
 "output \"(Day::os) |(Month:m) |(Year)\";\n"
 "}\n"
 "format full {\n"
 "output \"(WDay:w) |(Day::os) |(Month:m) |(Year)\";\n"
 "}\n"
 "format \"dmy+\", \"(Day) |(Month:m) |(Year)\";\n"
 "format \"wdmy+\", \"(WDay:w) |(Day) |(Month:m) |(Year)\";\n"
 "format mdy, \"(Month:m:a) |(Day), |(Year)\";\n"
 "format ymd, \"(Year)|:(Month:m:a)|:(Day)\";\n"
 "pref dmy;\n"
 "}\n"
 "scheme j {\n"
 "name \"Julian\";\n"
 "base julian;\n"
 "grammar j;\n"
 "}\n"
 "grammar g {\n"
 "inherit j;\n"
 "format iso {\n"
 "rules iso8601 caldate;\n"
 "}\n"
 "format iso_set {\n"
 "rules iso8601 dateset;\n"
 "}\n"
 "pref dmy;\n"
 "}\n"
 "scheme g {\n"
 "name \"Gregorian\";\n"
 "base gregorian;\n"
 "grammar g;\n"
 "}\n"
 "set inout g;\n"
 "grammar j_sh {\n"
 "fields year month day;\n"
 "optional unshift, wday;\n"
 "lexicons m, w;\n"
 "alias field {\n"
 "WDay, wday;\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, year;\n"
 "\"Hist-Year\", unshift;\n"
 "}\n"
 "alias pseudo {\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "y, \"Hist-Year\";\n"
 "}\n"
 "format dmy, \"(Day) |(Month:m:a) |(Year)\";\n"
 "format dmyh, \"(Day) |(Month:m:a) |(Year/Hist-Year)\";\n"
 "format out {\n"
 "output \"(Day::os) |(Month:m) |(Year/Hist-Year)\";\n"
 "}\n"
 "format full {\n"
 "output \"(WDay:w) |(Day::os) |(Month:m) |(Year/Hist-Year)\";\n"
 "}\n"
 "format wdmyh, \"(WDay:w:a) |(Day) |(Month:m:a) |(Year/Hist-Year)\";\n"
 "pref dmyh;\n"
 "}\n"
 "scheme ja {\n"
 "name \"Julian Annunciation\";\n"
 "epoch 1721507 julian;\n"
 "grammar j_sh;\n"
 "}\n"
 "scheme jap {\n"
 "name \"Julian Annunciation Pisa\";\n"
 "epoch 1721142 julian;\n"
 "grammar j_sh;\n"
 "}\n"
 "scheme jn {\n"
 "name \"Julian Nativity\";\n"
 "epoch 1721417 julian;\n"
 "grammar j_sh;\n"
 "}\n"
 "scheme se {\n"
 "name \"Spanish Era\";\n"
 "epoch 1707544 julian;\n"
 "grammar j;\n"
 "}\n" },
    { "julian_hybrid",  /* julian_hybrid.hcs */
 "lexicon jg {\n"
 "name \"Scheme names\";\n"
 "fieldname scheme;\n"
 "lang en;\n"
 "pseudo Scheme, Sch;\n"
 "tokens {\n"
 "0, \"Julian\", \"j\";\n"
 "1, \"Gregorian\", \"g\";\n"
 "}\n"
 "}\n"
 "grammar jg {\n"
 "inherit j;\n"
 "fields year month day scheme;\n"
 "lexicons jg;\n"
 "alias field {\n"
 "Scheme, scheme;\n"
 "}\n"
 "alias pseudo {\n"
 "s, Scheme;\n"
 "}\n"
 "format dmys, \"(Day) |(Month:m:a) |(Year) |(Scheme:jg:a)\";\n"
 "format \"dmys+\", \"(Day) |(Month:m) |(Year) |(Scheme:jg)\";\n"
 "pref dmy;\n"
 "}\n"
 "scheme ay {\n"
 "name \"Astronomical Year\";\n"
 "hybrid {\n"
 "fields year, month, day;\n"
 "scheme j;\n"
 "change 2299161;\n"
 "scheme g;\n"
 "}\n"
 "grammar jg;\n"
 "}\n"
 "lexicon jajg {\n"
 "name \"Scheme names\";\n"
 "fieldname scheme;\n"
 "lang en;\n"
 "pseudo Scheme, Sch;\n"
 "tokens {\n"
 "0, \"Julian Ann\", \"ja\";\n"
 "1, \"Julian\", \"j\";\n"
 "2, \"Gregorian\", \"g\";\n"
 "}\n"
 "}\n"
 "grammar hy {\n"
 "fields year month day scheme;\n"
 "optional unshift, wday;\n"
 "lexicons w, m, jajg;\n"
 "alias field {\n"
 "WDay, wday;\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, year;\n"
 "\"Hist-Year\", unshift;\n"
 "Scheme, scheme;\n"
 "}\n"
 "alias pseudo {\n"
 "w, WDay;\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "y, \"Hist-Year\";\n"
 "s, Scheme;\n"
 "}\n"
 "format dmyh, \"(Day) |(Month:m:a) |(Year/Hist-Year)\";\n"
 "format dmy, \"(Day) |(Month:m:a) |(Year)\";\n"
 "format dmys, \"(Day) |(Month:m:a) |(Year) |(Scheme:jajg:a)\";\n"
 "format wdmyh, \"(WDay:w:a) |(Day) |(Month:m:a) |(Year/Hist-Year)\";\n"
 "format wdmyhs,\"(WDay:w:a) |(Day) |(Month:m:a) |(Year/Hist-Year) |(Scheme:jajg)\";\n"
 "pref dmyh;\n"
 "}\n"
 "scheme eng {\n"
 "name \"English Hybrid\";\n"
 "hybrid {\n"
 "fields year, month, day;\n"
 "scheme ja;\n"
 "change 2360976;\n"
 "scheme j;\n"
 "change 2361222;\n"
 "scheme g;\n"
 "}\n"
 "grammar hy;\n"
 "}\n"
 "scheme scot {\n"
 "name \"Scottish Hybrid\";\n"
 "hybrid {\n"
 "fields year, month, day;\n"
 "scheme ja;\n"
 "change 2305458;\n"
 "scheme j;\n"
 "change 2361222;\n"
 "scheme g;\n"
 "}\n"
 "grammar hy;\n"
 "}\n" },
    { "easter",  /* easter.hcs */
 "lexicon rpt {\n"
 "name \"Repeated Date\";\n"
 "fieldname repeated;\n"
 "lang en;\n"
 "pseudo Repeated, Rpt;\n"
 "tokens {\n"
 "0, \"First Occurrence\", \"First\";\n"
 "1, \"Repeated Occurrence\", \"Repeated\";\n"
 "}\n"
 "}\n"
 "grammar je {\n"
 "fields year month day hist;\n"
 "optional wday repeated;\n"
 "calculate Rpt {\n"
 "output @if(repeated=0,?,1);\n"
 "pseudo \"[Repeated]\";\n"
 "}\n"
 "alias field {\n"
 "WDay, wday;\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, year;\n"
 "Rpt, repeated;\n"
 "Hist, hist;\n"
 "}\n"
 "alias pseudo {\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "y, Hist;\n"
 "r, Rpt;\n"
 "}\n"
 "lexicons m, w, rpt;\n"
 "format dmy \"(Day) |(Month:m:a) |(Year/Hist)| (Rpt:rpt:a)\";\n"
 "format \"dmy+\" \"(Day) |(Month:m) |(Year/Hist)| (Rpt:rpt)\";\n"
 "format mdy \"(Month:m:a) |(Day), |(Year/Hist)| (Rpt:rpt:a)\";\n"
 "format \"mdy+\" \"(Month:m) |(Day), |(Year/Hist)| (Rpt:rpt)\";\n"
 "pref dmy;\n"
 "}\n"
 "scheme je {\n"
 "name \"Julian Easter\";\n"
 "base easter;\n"
 "grammar je;\n"
 "}\n" },
    { "liturgical",  /* liturgical.hcs */
 "lexicon lit {\n"
 "name \"Liturgy Weeks\";\n"
 "fieldname litweek;\n"
 "lang en;\n"
 "pseudo Week, Wk;\n"
 "tokens {\n"
 "1, \"Second Week of Christmas\", \"Xmas II\";\n"
 "2, \"Epiphany Week\", \"Epiph\";\n"
 "3, \"First Week of Epiphany\", \"Epiph I\";\n"
 "4, \"Second Week of Epiphany\", \"Epiph II\";\n"
 "5, \"Third Week of Epiphany\", \"Epiph III\";\n"
 "6, \"Fourth Week of Epiphany\", \"Epiph IV\";\n"
 "7, \"Fifth Week of Epiphany\", \"Epiph V\";\n"
 "8, \"Sixth Week of Epiphany\", \"Epiph VI\";\n"
 "9, \"Septuagesima Week\", \"Septuag\";\n"
 "10, \"Sexagesima Week\", \"Sexages\";\n"
 "11, \"Quinquagesima Week\", \"Quinquag\";\n"
 "12, \"Quadragesima Week\", \"Quadrag\";\n"
 "13, \"Second Week of Lent\", \"Lent II\";\n"
 "14, \"Third Week of Lent\", \"Lent III\";\n"
 "15, \"Fourth Week of Lent\", \"Lent IV\";\n"
 "16, \"Passion Week\", \"Passion\";\n"
 "17, \"Holy Week\", \"Holy\";\n"
 "18, \"Easter Week\", \"Easter\";\n"
 "19, \"Quasimodo Week\", \"Quasimodo\";\n"
 "20, \"Second Week after Easter\", \"Easter II\";\n"
 "21, \"Third Week after Easter\", \"Easter III\";\n"
 "22, \"Fourth Week after Easter\", \"Easter IV\";\n"
 "23, \"Rogation Week\", \"Rogation\";\n"
 "24, \"First Week after Ascension\", \"Ascen I\";\n"
 "25, \"Whitsun Week\", \"Whitsun\";\n"
 "26, \"Trinity Week\", \"Trinity\";\n"
 "27, \"First Week after Trinity\", \"Trinity I\";\n"
 "28, \"Second Week after Trinity\", \"Trinity II\";\n"
 "29, \"Third Week after Trinity\", \"Trinity III\";\n"
 "30, \"Fourth Week after Trinity\", \"Trinity IV\";\n"
 "31, \"Fifth Week after Trinity\", \"Trinity V\";\n"
 "32, \"Sixth Week after Trinity\", \"Trinity VI\";\n"
 "33, \"Seventh Week after Trinity\", \"Trinity VII\";\n"
 "34, \"Eighth Week after Trinity\", \"Trinity VIII\";\n"
 "35, \"Ninth Week after Trinity\", \"Trinity IX\";\n"
 "36, \"Tenth Week after Trinity\", \"Trinity X\";\n"
 "37, \"Eleventh Week after Trinity\", \"Trinity XI\";\n"
 "38, \"Twelfth Week after Trinity\", \"Trinity XII\";\n"
 "39, \"Thirteenth Week after Trinity\", \"Trinity XIII\";\n"
 "40, \"Fourteenth Week after Trinity\", \"Trinity XIV\";\n"
 "41, \"Fifteenth Week after Trinity\", \"Trinity XV\";\n"
 "42, \"Sixteenth Week after Trinity\", \"Trinity XVI\";\n"
 "43, \"Seventeenth Week after Trinity\", \"Trinity XVII\";\n"
 "44, \"Eighteenth Week after Trinity\", \"Trinity XVIII\";\n"
 "45, \"Nineteenth Week after Trinity\", \"Trinity XIX\";\n"
 "46, \"Twentieth Week after Trinity\", \"Trinity XX\";\n"
 "47, \"Twenty First Week after Trinity\", \"Trinity XXI\";\n"
 "48, \"Twenty Second Week after Trinity\", \"Trinity XXII\";\n"
 "49, \"Twenty Third Week after Trinity\", \"Trinity XXIII\";\n"
 "50, \"Twenty Fourth Week after Trinity\", \"Trinity XXIV\";\n"
 "51, \"Twenty Fifth Week after Trinity\", \"Trinity XXV\";\n"
 "52, \"Twenty Sixth Week after Trinity\", \"Trinity XXVI\";\n"
 "53, \"Twenty Seventh Week after Trinity\", \"Trinity XXVII\";\n"
 "54, \"Fourth Week before Advent\", \"IV bef Advent\";\n"
 "55, \"Third Week before Advent\", \"III bef Advent\";\n"
 "56, \"Second Week before Advent\", \"II bef Advent\";\n"
 "57, \"First Week before Advent\", \"I bef Advent\";\n"
 "58, \"First Week of Advent\", \"Advent I\";\n"
 "59, \"Second Week of Advent\", \"Advent II\";\n"
 "60, \"Third Week of Advent\", \"Advent III\";\n"
 "61, \"Fourth Week of Advent\", \"Advent IV\";\n"
 "62, \"First Week of Christmas\", \"Xmas I\";\n"
 "}\n"
 "}\n"
 "grammar lit {\n"
 "fields year month day;\n"
 "optional wsday, litweek;\n"
 "rank year, litweek, wsday;\n"
 "lexicons m, ws, lit;\n"
 "alias field {\n"
 "WDay, wsday;\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, year;\n"
 "LitWeek, litweek;\n"
 "}\n"
 "alias pseudo {\n"
 "w, WDay;\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "}\n"
 "alias unit {\n"
 "d, day;\n"
 "m, month;\n"
 "y, year;\n"
 "w, week;\n"
 "}\n"
 "format wdmyl, \"(WDay:ws:a) |(Day) |(Month:m:a) |(Year)| (LitWeek:lit:a)\";\n"
 "format \"wdmyl+\", \"(WDay:ws) |(Day) |(Month:m) |(Year)| (LitWeek:lit)\";\n"
 "format wly, \"(WDay:ws:a), |(LitWeek:lit:a) |(Year)\";\n"
 "format \"wly+\", \"(WDay:ws), |(LitWeek:lit) |(Year)\";\n"
 "pref wdmyl;\n"
 "}\n"
 "scheme jlit {\n"
 "name \"Julian Liturgical\";\n"
 "base julian;\n"
 "grammar lit;\n"
 "}\n"
 "scheme glit {\n"
 "name \"Gregorian Liturgical\";\n"
 "base gregorian;\n"
 "grammar lit;\n"
 "}\n" },
    { "ce_bce",  /* ce-bce.hcs */
 "lexicon jce {\n"
 "name \"Historic Era\";\n"
 "fieldname ce;\n"
 "lang en;\n"
 "pseudo CommonEra, CE;\n"
 "tokens {\n"
 "0, \"Before Common Era\", \"BCE\";\n"
 "1, \"Common Era\", \"CE\";\n"
 "}\n"
 "}\n"
 "grammar jce {\n"
 "fields year month day;\n"
 "optional wday, ce, ceyear;\n"
 "rank ce, ceyear, month, day;\n"
 "lexicons m, w, jce;\n"
 "calculate Era {\n"
 "output @if(year<500,:ce,?);\n"
 "pseudo \"[CE<500]\";\n"
 "}\n"
 "alias field {\n"
 "WDay, wday;\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, ceyear;\n"
 "CEra, ce;\n"
 "Era, ce;\n"
 "}\n"
 "alias pseudo {\n"
 "w, WDay;\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "}\n"
 "alias unit {\n"
 "d, day;\n"
 "m, month;\n"
 "y, year;\n"
 "w, week;\n"
 "}\n"
 "format dmy, \"(Day) |(Month:m:a) |(Year)| (Era:jce:a=CE)\";\n"
 "format dmye, \"(Day) |(Month:m:a) |(Year)| (CEra:jce:a=CE)\";\n"
 "format wdmy, \"(WDay:w:a) |(Day) |(Month:m:a) |(Year)| (Era:jce:a=CE)\";\n"
 "format out {\n"
 "output \"(Day::os) |(Month:m) |(Year) |(CEra:jce:a)\";\n"
 "}\n"
 "format full {\n"
 "output \"(WDay:w) |(Day::os) |(Month:m) |(Year) |(CEra:jce)\";\n"
 "}\n"
 "format \"dmy+\", \"(Day) |(Month:m) |(Year) |(CEra:jce=CE)\";\n"
 "format \"wdmy+\", \"(WDay:w) |(Day) |(Month:m) |(Year) |(CEra:jce=CE)\";\n"
 "format mdy, \"(Month:m:a) |(Day), |(Year) |(CEra:jce:a=CE)\";\n"
 "format ymd, \"(Year)|(CEra:jce:a=CE)|:(Month:m:a)|:(Day)\";\n"
 "pref dmy;\n"
 "}\n"
 "scheme jce {\n"
 "name \"Julian Common Era\";\n"
 "base julian;\n"
 "grammar jce;\n"
 "}\n"
 "scheme gce {\n"
 "name \"Gregorian Common Era\";\n"
 "base gregorian;\n"
 "grammar jce;\n"
 "}\n" },
  /*  { "english_reg", @(cp.cpp)"english_reg.hcs" }, */
  /*  { "scottish_reg", @(cp.cpp)"scottish_reg.hcs" }, */
    { "iso",  /* iso.hcs */
 "grammar isow {\n"
 "fields year week wday;\n"
 "lexicons w;\n"
 "alias field {\n"
 "Day, wday;\n"
 "Week, week;\n"
 "Year, year;\n"
 "}\n"
 "alias pseudo {\n"
 "d, Day;\n"
 "ww, Week;\n"
 "yyyy, Year;\n"
 "}\n"
 "alias unit {\n"
 "d, day;\n"
 "y, year;\n"
 "w, week;\n"
 "}\n"
 "format ywd {\n"
 "rules iso8601 week;\n"
 "}\n"
 "format hc_ywd, \"(Year)| (Week)| (Day:w:a)\";\n"
 "format \"hc_ywd+\", \"(Year)| (Week)| (Day:w)\";\n"
 "format hc_iso, \"(Year)| (Week)| (Day)\";\n"
 "pref ywd;\n"
 "}\n"
 "scheme isow {\n"
 "name \"ISO:8601 Standard Week\";\n"
 "base isoweek;\n"
 "grammar isow;\n"
 "}\n"
 "grammar isoo {\n"
 "fields year day;\n"
 "alias field {\n"
 "Day, day;\n"
 "Year, year;\n"
 "}\n"
 "alias pseudo {\n"
 "ddd, Day;\n"
 "yyyy, Year;\n"
 "}\n"
 "alias unit {\n"
 "d, day;\n"
 "y, year;\n"
 "}\n"
 "format yd {\n"
 "rules iso8601 ordinal;\n"
 "}\n"
 "format hc_yd, \"(Year)| (Day)\";\n"
 "pref yd;\n"
 "}\n"
 "scheme isoo {\n"
 "name \"ISO:8601 Standard Ordinal\";\n"
 "base isoordinal;\n"
 "grammar isoo;\n"
 "}\n" },
    { "french_rep",  /* french_rep.hcs */
 "lexicon frm {\n"
 "name \"Month names\";\n"
 "fieldname month;\n"
 "lang en;\n"
 "pseudo Month, Mon;\n"
 "tokens {\n"
 "1, \"Vend\303\251miaire\", \"Vend\";\n"
 "2, \"Brumaire\", \"Brum\";\n"
 "3, \"Frimaire\", \"Frim\";\n"
 "4, \"Niv\303\264se\", \"Nivo\";\n"
 "5, \"Pluvi\303\264se\", \"Pluv\";\n"
 "6, \"Vent\303\264se\", \"Vent\";\n"
 "7, \"Germinal\", \"Germ\";\n"
 "8, \"Flor\303\251al\", \"Flor\";\n"
 "9, \"Prairial\", \"Prai\";\n"
 "10, \"Messidor\", \"Mess\";\n"
 "11, \"Thermidor\", \"Ther\";\n"
 "12, \"Fructidor\", \"Fruc\";\n"
 "13, \"Compl\303\251mentaires\", \"Comp\";\n"
 "}\n"
 "}\n"
 "lexicon frdd {\n"
 "name \"Decade Day\";\n"
 "fieldname dday;\n"
 "lang en;\n"
 "pseudo DecDay, DDay;\n"
 "tokens {\n"
 "1, \"Primidi\", \"Prim\";\n"
 "2, \"Duodi\", \"Duo\";\n"
 "3, \"Tridi\", \"Tri\";\n"
 "4, \"Quartidi\", \"Quar\";\n"
 "5, \"Quintidi\", \"Quin\";\n"
 "6, \"Sextidi\", \"Sext\";\n"
 "7, \"Septidi\", \"Sept\";\n"
 "8, \"Octidi\", \"Oct\";\n"
 "9, \"Nonidi\", \"Non\";\n"
 "10, \"D\303\251cadi\", \"Dec\";\n"
 "}\n"
 "}\n"
 "lexicon frcomp {\n"
 "name \"Complementary Days\";\n"
 "fieldname CDay;\n"
 "lang en;\n"
 "pseudo CompDay, CDay;\n"
 "tokens {\n"
 "1, \"F\303\252te de la Vertu\", \"Ver\";\n"
 "2, \"F\303\252te du G\303\251nie\", \"Gen\";\n"
 "3, \"F\303\252te du Travail\", \"Trav\";\n"
 "4, \"F\303\252te de l'Opinion\", \"Opin\";\n"
 "5, \"F\303\252te des R\303\251compenses\", \"Rec\";\n"
 "6, \"F\303\252te de la R\303\251volution\", \"Rev\";\n"
 "}\n"
 "}\n"
 "grammar fr {\n"
 "fields year month day;\n"
 "optional dday, wday;\n"
 "lexicons frm, frcomp, frdd, w;\n"
 "alias field {\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, year;\n"
 "DecDay, dday;\n"
 "WDay, wday;\n"
 "}\n"
 "alias pseudo {\n"
 "dd, Day;\n"
 "dd, MDay;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "mm, NMonth;\n"
 "dd, NMDay;\n"
 "w, WDay;\n"
 "d, DecDay;\n"
 "}\n"
 "alias unit {\n"
 "d, day;\n"
 "dec, decade;\n"
 "w, week;\n"
 "m, month;\n"
 "y, year;\n"
 "}\n"
 "calculate CDay {\n"
 "output @if( month=13, day, ? );\n"
 "input { \"\", , 13, CDay };\n"
 "}\n"
 "calculate MDay {\n"
 "output @if( month<>13, day, ? );\n"
 "alias day;\n"
 "}\n"
 "calculate NMonth {\n"
 "output @if( month<>13 or day=?, month, ? );\n"
 "alias month;\n"
 "}\n"
 "format dmy \"(Day) |(Month:frm:a) |(Year)\";\n"
 "format \"dmy+\" \"(Day) |(Month:frm) |(Year)\";\n"
 "format ymd \"(Year)|,(Month:frm:a)|,(Day)\";\n"
 "format cdmy {\n"
 "inout \"(CDay:frcomp) |(MDay) |(NMonth:frm) |(Year)\";\n"
 "instring \"Day Month Year\";\n"
 "}\n"
 "format out {\n"
 "output \"(CDay:frcomp) |(MDay) |(NMonth:frm) |an (Year::rn)\";\n"
 "}\n"
 "format dcdmy {\n"
 "inout \"(DecDay:frdd) |(CDay:frcomp) |(MDay) |(NMonth:frm) |(Year)\";\n"
 "instring \"Day Month Year\";\n"
 "}\n"
 "format wcdmy {\n"
 "inout \"(WDay:w), |(CDay:frcomp) |(MDay) |(NMonth:frm) |(Year)\";\n"
 "instring \"Day Month Year\";\n"
 "}\n"
 "format \"cdmy-\" {\n"
 "inout \"(CDay:frcomp:a) |(MDay) |(NMonth:frm:a) |(Year)\";\n"
 "instring \"Day Month Year\";\n"
 "}\n"
 "format \"dcdmy-\" {\n"
 "inout \"(DecDay:frdd:a) |(CDay:frcomp:a) |(MDay) |(NMonth:frm:a) |(Year)\";\n"
 "instring \"Day Month Year\";\n"
 "}\n"
 "format \"wcdmy-\" {\n"
 "inout \"(WDay:w:a), |(CDay:frcomp:a) |(MDay) |(NMonth:frm:a) |(Year)\";\n"
 "instring \"Day Month Year\";\n"
 "}\n"
 "pref cdmy;\n"
 "}\n"
 "scheme fr {\n"
 "name \"French Republic\";\n"
 "base french;\n"
 "grammar fr;\n"
 "}\n" },
    { "hebrew",  /* hebrew.hcs */
 "lexicon hm {\n"
 "name \"Month names\";\n"
 "fieldname month;\n"
 "lang en;\n"
 "pseudo Month, Mon;\n"
 "tokens {\n"
 "1, \"Nisan\", \"Nsn\";\n"
 "2, \"Iyyar\", \"Iyr\";\n"
 "3, \"Sivan\", \"Svn\";\n"
 "4, \"Tammuz\", \"Tmz\";\n"
 "5, \"Av\", \"Aav\";\n"
 "6, \"Elul\", \"Ell\";\n"
 "7, \"Tishri\", \"Tsh\";\n"
 "8, \"Marheshvan\", \"Csh\";\n"
 "9, \"Kislev\", \"Ksl\";\n"
 "10, \"Tevet\", \"Tvt\";\n"
 "11, \"Shevat\", \"Shv\";\n"
 "12, \"Adar\", \"Adr\";\n"
 "13, \"Adar II\", \"Ads\";\n"
 "}\n"
 "}\n"
 "lexicon hw {\n"
 "name \"Weekday names\";\n"
 "fieldname wsday;\n"
 "lang en;\n"
 "pseudo Weekday, WDay;\n"
 "tokens {\n"
 "1, \"Yom Rishon\", \"Rish\";\n"
 "2, \"Yom Sheni\", \"Shen\";\n"
 "3, \"Yom Shelishi\", \"Shel\";\n"
 "4, \"Yom Revi'i\", \"Revi\";\n"
 "5, \"Yom Hamishi\", \"Hami\";\n"
 "6, \"Yom Shishi\", \"Shis\";\n"
 "7, \"Yom Shabbat\", \"Shab\";\n"
 "}\n"
 "}\n"
 "grammar h {\n"
 "fields year month day;\n"
 "optional wsday;\n"
 "lexicons hm, hw;\n"
 "alias field {\n"
 "WDay, wsday;\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, year;\n"
 "}\n"
 "alias pseudo {\n"
 "w, WDay;\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "}\n"
 "alias unit {\n"
 "d, day;\n"
 "m, month;\n"
 "y, year;\n"
 "w, week;\n"
 "}\n"
 "format dmy, \"(Day) |(Month:hm) |(Year)\";\n"
 "format \"dmy.a\", \"(Day) |(Month:hm:a) |(Year)\";\n"
 "format \"dmy.d\", \"(Day) |(Month) |(Year)\";\n"
 "format wdmy, \"(WDay:hw) |(Day) |(Month:hm) |(Year)\";\n"
 "format \"wdmy.a\", \"(WDay:hw:a) |(Day) |(Month:hm:a) |(Year)\";\n"
 "format ymd, \"(Year)|,(Month:hm:a)|,(Day)\";\n"
 "pref dmy;\n"
 "}\n"
 "scheme h {\n"
 "name \"Hebrew\";\n"
 "base hebrew;\n"
 "grammar h;\n"
 "}\n" },
    { "islamic",  /* islamic.hcs */
 "lexicon im {\n"
 "name \"Month names\";\n"
 "fieldname month;\n"
 "lang en;\n"
 "pseudo Month, Mon;\n"
 "tokens {\n"
 "1, \"Mu\341\270\245arram\", \"Muharram\";\n"
 "2, \"\341\271\242afar\", \"Safar\";\n"
 "3, \"Rab\304\253\342\200\230 al-Awwal\", \"Rabi' I\";\n"
 "4, \"Rab\304\253\342\200\230 al-\304\200khir\", \"Rabi' II\";\n"
 "5, \"Jum\304\201d\303\241 al-\305\252l\303\241\", \"Jumada I\";\n"
 "6, \"Jum\304\201d\303\241 al-\304\200khira\", \"Jumada II\";\n"
 "7, \"Rajab\";\n"
 "8, \"Sha\342\200\230b\304\201n\", \"Sha'aban\";\n"
 "9, \"Rama\341\270\215\304\201n\", \"Ramadan\";\n"
 "10, \"Shaww\304\201l\", \"Shawwal\";\n"
 "11, \"Dh\305\253 al-Qa\342\200\230dah\", \"Dhu al-Qi'dah\";\n"
 "12, \"Dh\305\253 al-\341\270\244ijjah\", \"Dhu al-Hijjah\";\n"
 "}\n"
 "}\n"
 "lexicon iw {\n"
 "name \"Weekday names\";\n"
 "fieldname wsday;\n"
 "lang en;\n"
 "pseudo Weekday, WDay;\n"
 "tokens {\n"
 "1, \"Yawm al-A\341\270\245ad\", \"Ahad\";\n"
 "2, \"Yawm al-Ithnayn\", \"Ithn\";\n"
 "3, \"Yawm ath-Thul\304\201th\304\201\312\274\", \"Thul\";\n"
 "4, \"Yawm al-Arbi\342\200\230\304\201\342\200\231\", \"Arbi\";\n"
 "5, \"Yawm al-Kham\304\253s\", \"Kham\";\n"
 "6, \"Yawm al-Jumu\342\200\230ah\", \"Jumu\";\n"
 "7, \"Yawm as-Sabt\", \"Sabt\";\n"
 "}\n"
 "}\n"
 "grammar i {\n"
 "fields year month day;\n"
 "optional wsday;\n"
 "lexicons im, iw;\n"
 "alias field {\n"
 "WDay, wsday;\n"
 "Day, day;\n"
 "Month, month;\n"
 "Year, year;\n"
 "}\n"
 "alias pseudo {\n"
 "w, WDay;\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "yyyy, Year;\n"
 "}\n"
 "alias unit {\n"
 "d, day;\n"
 "m, month;\n"
 "y, year;\n"
 "w, week;\n"
 "}\n"
 "format dmy, \"(Day) |(Month:im) |(Year)\";\n"
 "format \"dmy.a\", \"(Day) |(Month:im:a) |(Year)\";\n"
 "format \"dmy.d\", \"(Day) |(Month) |(Year)\";\n"
 "format wdmy, \"(WDay:iw) |(Day) |(Month:im) |(Year)\";\n"
 "format \"wdmy.a\", \"(WDay:iw:a) |(Day) |(Month:im:a) |(Year)\";\n"
 "format ymd, \"(Year)|,(Month:im:a)|,(Day)\";\n"
 "pref dmy;\n"
 "}\n"
 "scheme i {\n"
 "name \"Islamic Tabular\";\n"
 "base islamic IIc;\n"
 "grammar i;\n"
 "}\n"
 "scheme ims {\n"
 "name \"Islamic Microsoft Hijri\";\n"
 "base islamic IIa;\n"
 "grammar i;\n"
 "}\n"
 "scheme \"if\" {\n"
 "name \"Islamic F\304\201timid\";\n"
 "base islamic IIIa;\n"
 "grammar i;\n"
 "}\n" },
    { "chinese",  /* chinese.hcs */
 "lexicon cyear {\n"
 "name \"Year names\";\n"
 "fieldname cyear;\n"
 "lang en;\n"
 "pseudo YearName, YrN;\n"
 "tokens {\n"
 "1, \"Ji\307\216-z\307\220\", \"Jia-zi\";\n"
 "2, \"Y\307\220-ch\307\222u\", \"Yi-chou\";\n"
 "3, \"B\307\220ng-y\303\255n\", \"Bing-yin\";\n"
 "4, \"D\304\253ng-m\307\216o\", \"Ding-mao\";\n"
 "5, \"W\303\271-ch\303\251n\", \"Wu-chen\";\n"
 "6, \"J\307\220-s\303\254\", \"Ji-si\";\n"
 "7, \"G\304\223ng-w\307\224\", \"Geng-wu\";\n"
 "8, \"X\304\253n-w\303\250i\", \"Xin-wei\";\n"
 "9, \"R\303\251n-sh\304\223n\", \"Ren-shen\";\n"
 "10, \"Gu\303\254-y\307\222u\", \"Gui-you\";\n"
 "11, \"Ji\307\216-x\305\253\", \"Jia-xu\";\n"
 "12, \"Y\307\220-h\303\240i\", \"Yi-hai\";\n"
 "13, \"B\307\220ng-z\307\220\", \"Bing-zi\";\n"
 "14, \"D\304\253ng-ch\307\222u\", \"Ding-chou\";\n"
 "15, \"W\303\271-y\303\255n\", \"Wu-yin\";\n"
 "16, \"J\307\220-m\307\216o\", \"Ji-mao\";\n"
 "17, \"G\304\223ng-ch\303\251n\", \"Geng-chen\";\n"
 "18, \"X\304\253n-s\303\254\", \"Xin-si\";\n"
 "19, \"R\303\251n-w\307\224\", \"Ren-wu\";\n"
 "20, \"Gu\303\254-w\303\250i\", \"Gui-wei\";\n"
 "21, \"Ji\307\216-sh\304\223n\", \"Jia-shen\";\n"
 "22, \"Y\307\220-y\307\222u\", \"Yi-you\";\n"
 "23, \"B\307\220ng-x\305\253\", \"Bing-xu\";\n"
 "24, \"D\304\253ng-h\303\240i\", \"Ding-hai\";\n"
 "25, \"W\303\271-z\307\220\", \"Wu-zi\";\n"
 "26, \"J\307\220-ch\307\222u\", \"Ji-chou\";\n"
 "27, \"G\304\223ng-y\303\255n\", \"Geng-yin\";\n"
 "28, \"X\304\253n-m\307\216o\", \"Xin-mao\";\n"
 "29, \"R\303\251n-ch\303\251n\", \"Ren-chen\";\n"
 "30, \"Gu\303\254-s\303\254\", \"Gui-si\";\n"
 "31, \"Ji\307\216-w\307\224\", \"Jia-wu\";\n"
 "32, \"Y\307\220-w\303\250i\", \"Yi-wei\";\n"
 "33, \"B\307\220ng-sh\304\223n\", \"Bing-shen\";\n"
 "34, \"D\304\253ng-y\307\222u\", \"Ding-you\";\n"
 "35, \"W\303\271-x\305\253\", \"Wu-xu\";\n"
 "36, \"J\307\220-h\303\240i\", \"Ji-hai\";\n"
 "37, \"G\304\223ng-z\307\220\", \"Geng-zi\";\n"
 "38, \"X\304\253n-ch\307\222u\", \"Xin-chou\";\n"
 "39, \"R\303\251n-y\303\255n\", \"Ren-yin\";\n"
 "40, \"Gu\303\254-m\307\216o\", \"Gui-mao\";\n"
 "41, \"Ji\307\216-ch\303\251n\", \"Jia-chen\";\n"
 "42, \"Y\307\220-s\303\254\", \"Yi-si\";\n"
 "43, \"B\307\220ng-w\307\224\", \"Bing-wu\";\n"
 "44, \"D\304\253ng-w\303\250i\", \"Ding-wei\";\n"
 "45, \"W\303\271-sh\304\223n\", \"Wu-shen\";\n"
 "46, \"J\307\220-y\307\222u\", \"Ji-you\";\n"
 "47, \"G\304\223ng-x\305\253\", \"Geng-xu\";\n"
 "48, \"X\304\253n-h\303\240i\", \"Xin-hai\";\n"
 "49, \"R\303\251n-z\307\220\", \"Ren-zi\";\n"
 "50, \"Gu\303\254-ch\307\222u\", \"Gui-chou\";\n"
 "51, \"Ji\307\216-y\303\255n\", \"Jia-yin\";\n"
 "52, \"Y\307\220-m\307\216o\", \"Yi-mao\";\n"
 "53, \"B\307\220ng-ch\303\251n\", \"Bing-chen\";\n"
 "54, \"D\304\253ng-s\303\254\", \"Ding-si\";\n"
 "55, \"W\303\271-w\307\224\", \"Wu-wu\";\n"
 "56, \"J\307\220-w\303\250i\", \"Ji-wei\";\n"
 "57, \"G\304\223ng-sh\304\223n\", \"Geng-shen\";\n"
 "58, \"X\304\253n-y\307\222u\", \"Xin-you\";\n"
 "59, \"R\303\251n-x\305\253\", \"Ren-xu\";\n"
 "60, \"Gu\303\254-h\303\240i\", \"Gui-hai\";\n"
 "}\n"
 "}\n"
 "lexicon clmon {\n"
 "name \"Leap month\";\n"
 "fieldname lmonth;\n"
 "lang en;\n"
 "pseudo LeapMon, LM;\n"
 "tokens {\n"
 "0, \"Month\", \"Month\";\n"
 "1, \"Leap-month\", \"Leap\";\n"
 "}\n"
 "}\n"
 "grammar c {\n"
 "fields cycle cyear month lmonth day;\n"
 "lexicons cyear, clmon;\n"
 "calculate LM {\n"
 "output @if(lmonth=0,?,lmonth);\n"
 "pseudo \"[Leap]\";\n"
 "}\n"
 "alias field {\n"
 "Day, day;\n"
 "Leap, lmonth;\n"
 "Month, month;\n"
 "Year, cyear;\n"
 "Cycle, cycle;\n"
 "}\n"
 "alias pseudo {\n"
 "dd, Day;\n"
 "mm, Month;\n"
 "lm, LMonth;\n"
 "yy, Year;\n"
 "cc, Cycle;\n"
 "}\n"
 "format cymd, \"(Cycle),| (Year),| (LM:clmon:a=Month)| (Month),| (Day)\";\n"
 "format \"cymd+\", \"(Cycle),| (Year:cyear),| (LM:clmon:a=Month)| (Month),| (Day)\";\n"
 "format full {\n"
 "output \"Cycle (Cycle),| Year (Year),| (Leap:clmon)| (Month),| Day (Day)\";\n"
 "}\n"
 "format yname {\n"
 "output \"Cycle (Cycle),| Year (Year:cyear),| (Leap:clmon)| (Month),| Day (Day)\";\n"
 "}\n"
 "pref full;\n"
 "}\n"
 "scheme c {\n"
 "name \"Chinese\";\n"
 "base chinese;\n"
 "grammar c;\n"
 "}\n" },
};

size_t Cal::cal_default_scripts_size = sizeof(Cal::cal_default_scripts) / sizeof(Cal::ScriptModule);

// End of scripts/caldefscripts.f2c file
