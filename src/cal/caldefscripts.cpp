/* ..\src\cal\caldefscripts.cpp - File created by file2cpp */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        scripts/caldefscripts.f2c
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     file2cpp template file for default scripts.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th September 2014
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

 This template is used to create the src/cal/caldefscripts.cpp file.

*/

#include "caldefscripts.h"

const char* Cal::cal_default_scripts[] = {
     /* base.hcs */
 "vocab \"w\" {\n"
 "name \"Weekday names\";\n"
 "fieldname \"wday\";\n"
 "lang \"en\";\n"
 "stylename \"Weekday\", \"WDay\";\n"
 "tokens {\n"
 "1, \"Monday\",    \"Mon\";\n"
 "2, \"Tuesday\",   \"Tue\";\n"
 "3, \"Wednesday\", \"Wed\";\n"
 "4, \"Thursday\",  \"Thur\";\n"
 "5, \"Friday\",    \"Fri\";\n"
 "6, \"Saturday\",  \"Sat\";\n"
 "7, \"Sunday\",    \"Sun\";\n"
 "}\n"
 "}\n",
     /* jdn.hcs */
 "grammar \"d\" {\n"
 "alias field {\n"
 "\"Day\", \"day\";\n"
 "}\n"
 "alias stylename {\n"
 "\"Day\", \"ddddd\";\n"
 "}\n"
 "format \"d\", \"(Day)\";\n"
 "pref \"d\";\n"
 "}\n"
 "scheme \"jdn\" {\n"
 "name \"Julian Day Number\";\n"
 "base jdn;\n"
 "grammar \"d\";\n"
 "}\n",
     /* julian.hcs */
 "vocab \"m\" {\n"
 "name \"Month names\";\n"
 "fieldname \"month\";\n"
 "lang \"en\";\n"
 "stylename \"Month\",  \"Mon\";\n"
 "tokens {\n"
 "1, \"January\",   \"Jan\";\n"
 "2, \"February\",  \"Feb\";\n"
 "3, \"March\",     \"Mar\";\n"
 "4, \"April\",     \"Apr\";\n"
 "5, \"May\";\n"
 "6, \"June\",      \"Jun\";\n"
 "7, \"July\",      \"Jul\";\n"
 "8, \"August\",    \"Aug\";\n"
 "9, \"September\", \"Sep\";\n"
 "10, \"October\",  \"Oct\";\n"
 "11, \"November\", \"Nov\";\n"
 "12, \"December\", \"Dec\";\n"
 "}\n"
 "}\n"
 "grammar \"j\" {\n"
 "optional \"wday\";\n"
 "vocabs \"m\", \"w\";\n"
 "alias field {\n"
 "\"WDay\",  \"wday\";\n"
 "\"Day\",   \"day\";\n"
 "\"Month\", \"month\";\n"
 "\"Year\",  \"year\";\n"
 "}\n"
 "alias stylename {\n"
 "\"WDay\",  \"w\";\n"
 "\"Day\",   \"dd\";\n"
 "\"Month\", \"mm\";\n"
 "\"Year\",  \"yyyy\";\n"
 "}\n"
 "alias unit {\n"
 "\"d\", \"day\";\n"
 "\"m\", \"month\";\n"
 "\"y\", \"year\";\n"
 "\"w\", \"week\";\n"
 "}\n"
 "format \"dmy\",   \"(Day) |(Month:m.a) |(Year)\";\n"
 "format \"wdmy\",  \"(WDay:w.a) |(Day) |(Month:m.a) |(Year)\";\n"
 "format \"out\" {\n"
 "output \"(Day:+os) |(Month:m) |(Year)\";\n"
 "}\n"
 "format \"full\" {\n"
 "output \"(WDay:w) |(Day:+os) |(Month:m) |(Year)\";\n"
 "}\n"
 "format \"dmy+\",  \"(Day) |(Month:m) |(Year)\";\n"
 "format \"wdmy+\", \"(WDay:w) |(Day) |(Month:m) |(Year)\";\n"
 "format \"mdy\",   \"(Month:m.a) |(Day), |(Year)\";\n"
 "format \"ymd\",   \"(Year)|:(Month:m.a)|:(Day)\";\n"
 "pref \"dmy\";\n"
 "}\n"
 "scheme \"j\" {\n"
 "name \"Julian\";\n"
 "base julian;\n"
 "grammar \"j\";\n"
 "}\n"
 "grammar \"g\" {\n"
 "inherit \"j\";\n"
 "format \"iso\" {\n"
 "separators \"-\";\n"
 "strict \"(Year:!lp.04)|-(Month:!lp.02)|-(Day:!lp.02)\";\n"
 "}\n"
 "pref \"dmy\";\n"
 "}\n"
 "scheme \"g\" {\n"
 "name \"Gregorian\";\n"
 "base gregorian;\n"
 "grammar \"g\";\n"
 "}\n"
 "grammar \"j.sh\" {\n"
 "optional \"unshift\", \"wday\";\n"
 "vocabs \"m\", \"w\";\n"
 "alias field {\n"
 "\"WDay\",      \"wday\";\n"
 "\"Day\",       \"day\";\n"
 "\"Month\",     \"month\";\n"
 "\"Year\",      \"year\";\n"
 "\"Hist-Year\", \"unshift\";\n"
 "}\n"
 "alias stylename {\n"
 "\"Day\",       \"dd\";\n"
 "\"Month\",     \"mm\";\n"
 "\"Year\",      \"yyyy\";\n"
 "\"Hist-Year\", \"y\";\n"
 "}\n"
 "format \"dmy\",   \"(Day) |(Month:m.a) |(Year)\";\n"
 "format \"dmyh\",  \"(Day) |(Month:m.a) |(Year/Hist-Year)\";\n"
 "format \"out\" {\n"
 "output \"(Day:+os) |(Month:m) |(Year/Hist-Year)\";\n"
 "}\n"
 "format \"full\" {\n"
 "output \"(WDay:w) |(Day:+os) |(Month:m) |(Year/Hist-Year)\";\n"
 "}\n"
 "format \"wdmyh\", \"(WDay:w.a) |(Day) |(Month:m.a) |(Year/Hist-Year)\";\n"
 "pref \"dmyh\";\n"
 "}\n"
 "scheme \"ja\" {\n"
 "name \"Julian Annunciation\";\n"
 "shift \"j\", 1721507;\n"
 "grammar \"j.sh\";\n"
 "}\n"
 "scheme \"jap\" {\n"
 "name \"Julian Annunciation Pisa\";\n"
 "shift \"j\", 1721142;\n"
 "grammar \"j.sh\";\n"
 "}\n"
 "scheme \"jn\" {\n"
 "name \"Julian Nativity\";\n"
 "shift \"j\", 1721417;\n"
 "grammar \"j.sh\";\n"
 "}\n"
 "scheme \"se\" {\n"
 "name \"Spanish Era\";\n"
 "shift \"j\", 1707544;\n"
 "grammar \"j\";\n"
 "}\n",
     /* julian_hybrid.hcs */
 "vocab \"jajg\" {\n"
 "name \"Scheme names\";\n"
 "fieldname \"scheme\";\n"
 "lang \"en\";\n"
 "stylename \"Scheme\", \"Sch\";\n"
 "tokens {\n"
 "0, \"Julian Ann\", \"ja\";\n"
 "1, \"Julian\",     \"j\";\n"
 "2, \"Gregorian\",  \"g\";\n"
 "}\n"
 "}\n"
 "grammar \"hy\" {\n"
 "optional \"unshift\", \"wday\";\n"
 "vocabs \"w\", \"m\", \"jajg\";\n"
 "alias field {\n"
 "\"WDay\",      \"wday\";\n"
 "\"Day\",       \"day\";\n"
 "\"Month\",     \"month\";\n"
 "\"Year\",      \"year\";\n"
 "\"Hist-Year\", \"unshift\";\n"
 "\"Scheme\",    \"scheme\";\n"
 "}\n"
 "alias stylename {\n"
 "\"WDay\",      \"w\";\n"
 "\"Day\",       \"dd\";\n"
 "\"Month\",     \"mm\";\n"
 "\"Year\",      \"yyyy\";\n"
 "\"Hist-Year\", \"y\";\n"
 "\"Scheme\",    \"s\";\n"
 "}\n"
 "format \"dmyh\",  \"(Day) |(Month:m.a) |(Year/Hist-Year)\";\n"
 "format \"dmy\",   \"(Day) |(Month:m.a) |(Year)\";\n"
 "format \"dmys\",  \"(Day) |(Month:m.a) |(Year) |(Scheme:jajg.a)\";\n"
 "format \"wdmyh\", \"(WDay:w.a) |(Day) |(Month:m.a) |(Year/Hist-Year)\";\n"
 "format \"wdmyhs\",\"(WDay:w.a) |(Day) |(Month:m.a) |(Year/Hist-Year) |(Scheme:jajg)\";\n"
 "pref \"dmyh\";\n"
 "}\n"
 "scheme \"eng\" {\n"
 "name \"English Hybrid\";\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme \"ja\";\n"
 "change 2360976;\n"
 "scheme \"j\";\n"
 "change 2361222;\n"
 "scheme \"g\";\n"
 "}\n"
 "grammar \"hy\";\n"
 "}\n"
 "scheme \"scot\" {\n"
 "name \"Scottish Hybrid\";\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme \"ja\";\n"
 "change 2305458;\n"
 "scheme \"j\";\n"
 "change 2361222;\n"
 "scheme \"g\";\n"
 "}\n"
 "grammar \"hy\";\n"
 "}\n",
     /* liturgical.hcs */
 "vocab \"lit\" {\n"
 "name \"Liturgy Weeks\";\n"
 "fieldname \"litweek\";\n"
 "lang \"en\";\n"
 "stylename \"Week\", \"Wk\";\n"
 "tokens {\n"
 "1,  \"Second Week of Christmas\",          \"Xmas II\";\n"
 "2,  \"Epiphany Week\",                     \"Epiph\";\n"
 "3,  \"First Week of Epiphany\",            \"Epiph I\";\n"
 "4,  \"Second Week of Epiphany\",           \"Epiph II\";\n"
 "5,  \"Third Week of Epiphany\",            \"Epiph III\";\n"
 "6,  \"Fourth Week of Epiphany\",           \"Epiph IV\";\n"
 "7,  \"Fifth Week of Epiphany\",            \"Epiph V\";\n"
 "8,  \"Sixth Week of Epiphany\",            \"Epiph VI\";\n"
 "9,  \"Septuagesima Week\",                 \"Septuag\";\n"
 "10, \"Sexagesima Week\",                   \"Sexages\";\n"
 "11, \"Quinquagesima Week\",                \"Quinquag\";\n"
 "12, \"Quadragesima Week\",                 \"Quadrag\";\n"
 "13, \"Second Week of Lent\",               \"Lent II\";\n"
 "14, \"Third Week of Lent\",                \"Lent III\";\n"
 "15, \"Fourth Week of Lent\",               \"Lent IV\";\n"
 "16, \"Fifth Week of Lent\",                \"Lent V\";\n"
 "17, \"Passion Week\",                      \"Passion\";\n"
 "18, \"Easter Week\",                       \"Easter\";\n"
 "19, \"Quasimodo Week\",                    \"Quasimodo\";\n"
 "20, \"Second Week after Easter\",          \"Easter II\";\n"
 "21, \"Third Week after Easter\",           \"Easter III\";\n"
 "22, \"Fourth Week after Easter\",          \"Easter IV\";\n"
 "23, \"Rogation Week\",                     \"Rogation\";\n"
 "24, \"First Week after Ascension\",        \"Ascens I\";\n"
 "25, \"Whitsun Week\",                      \"Whitsun\";\n"
 "26, \"Trinity Week\",                      \"Trinity\";\n"
 "27, \"First Week after Trinity\",          \"Trinity I\";\n"
 "28, \"Second Week after Trinity\",         \"Trinity II\";\n"
 "29, \"Third Week after Trinity\",          \"Trinity III\";\n"
 "30, \"Fourth Week after Trinity\",         \"Trinity IV\";\n"
 "31, \"Fifth Week after Trinity\",          \"Trinity V\";\n"
 "32, \"Sixth Week after Trinity\",          \"Trinity VI\";\n"
 "33, \"Seventh Week after Trinity\",        \"Trinity VII\";\n"
 "34, \"Eighth Week after Trinity\",         \"Trinity VIII\";\n"
 "35, \"Ninth Week after Trinity\",          \"Trinity IX\";\n"
 "36, \"Tenth Week after Trinity\",          \"Trinity X\";\n"
 "37, \"Eleventh Week after Trinity\",       \"Trinity XI\";\n"
 "38, \"Twelfth Week after Trinity\",        \"Trinity XII\";\n"
 "39, \"Thirteenth Week after Trinity\",     \"Trinity XIII\";\n"
 "40, \"Fourteenth Week after Trinity\",     \"Trinity XIV\";\n"
 "41, \"Fifteenth Week after Trinity\",      \"Trinity XV\";\n"
 "42, \"Sixteenth Week after Trinity\",      \"Trinity XVI\";\n"
 "43, \"Seventeenth Week after Trinity\",    \"Trinity XVII\";\n"
 "44, \"Eighteenth Week after Trinity\",     \"Trinity XVIII\";\n"
 "45, \"Nineteenth Week after Trinity\",     \"Trinity XIX\";\n"
 "46, \"Twentieth Week after Trinity\",      \"Trinity XX\";\n"
 "47, \"Twenty First Week after Trinity\",   \"Trinity XXI\";\n"
 "48, \"Twenty Second Week after Trinity\",  \"Trinity XXII\";\n"
 "49, \"Twenty Third Week after Trinity\",   \"Trinity XXIII\";\n"
 "50, \"Twenty Fourth Week after Trinity\",  \"Trinity XXIV\";\n"
 "51, \"Twenty Fifth Week after Trinity\",   \"Trinity XXV\";\n"
 "52, \"Twenty Sixth Week after Trinity\",   \"Trinity XXVI\";\n"
 "53, \"Twenty Seventh Week after Trinity\", \"Trinity XXVII\";\n"
 "54, \"Fourth Week before Advent\",         \"IV bef Advent\";\n"
 "55, \"Third Week before Advent\",          \"III bef Advent\";\n"
 "56, \"Second Week before Advent\",         \"II bef Advent\";\n"
 "57, \"First Week before Advent\",          \"I bef Advent\";\n"
 "58, \"First Week of Advent\",              \"Advent I\";\n"
 "59, \"Second Week of Advent\",             \"Advent II\";\n"
 "60, \"Third Week of Advent\",              \"Advent III\";\n"
 "61, \"Fourth Week of Advent\",             \"Advent IV\";\n"
 "62, \"First Week of Christmas\",           \"Xmas I\";\n"
 "}\n"
 "}\n"
 "grammar \"lit\" {\n"
 "vocabs \"m\", \"w\", \"lit\";\n"
 "format \"wdmyl\",  \"(WDay:w.a) |(Day) |(Month:m.a) |(Year) |(LitWeek:lit.a)\";\n"
 "format \"wdmyl+\", \"(WDay:w) |(Day) |(Month:m) |(Year) |(LitWeek:lit)\";\n"
 "pref \"wdmyl\";\n"
 "alias field {\n"
 "\"WDay\",    \"wday\";\n"
 "\"Day\",     \"day\";\n"
 "\"Month\",   \"month\";\n"
 "\"Year\",    \"year\";\n"
 "\"LitWeek\", \"litweek\";\n"
 "}\n"
 "alias stylename {\n"
 "\"WDay\",    \"w\";\n"
 "\"Day\",     \"dd\";\n"
 "\"Month\",   \"mm\";\n"
 "\"Year\",    \"yyyy\";\n"
 "}\n"
 "alias unit {\n"
 "\"d\", \"day\";\n"
 "\"m\", \"month\";\n"
 "\"y\", \"year\";\n"
 "\"w\", \"week\";\n"
 "}\n"
 "}\n"
 "scheme \"jlit\" {\n"
 "name \"Julian Liturgical\";\n"
 "base julian;\n"
 "grammar \"lit\";\n"
 "}\n"
 "scheme \"glit\" {\n"
 "name \"Gregorian Liturgical\";\n"
 "base gregorian;\n"
 "grammar \"lit\";\n"
 "}\n",
     /* ce-bce.hcs */
 "vocab \"ce\" {\n"
 "name \"Historic Era\";\n"
 "fieldname \"ce\";\n"
 "lang \"en\";\n"
 "stylename \"CommonEra\",  \"CE\";\n"
 "tokens {\n"
 "0, \"Before Common Era\",   \"BCE\";\n"
 "1, \"Common Era\",   \"CE\";\n"
 "}\n"
 "}\n"
 "grammar \"jce\" {\n"
 "optional \"wday\", \"ce\", \"ceyear\";\n"
 "rank \"ce\", \"ceyear\", \"month\", \"day\", \"wday\";\n"
 "vocabs \"m\", \"w\", \"ce\";\n"
 "alias field {\n"
 "\"WDay\",  \"wday\";\n"
 "\"Day\",   \"day\";\n"
 "\"Month\", \"month\";\n"
 "\"Year\",  \"ceyear\";\n"
 "\"CEra\",  \"ce\";\n"
 "}\n"
 "alias stylename {\n"
 "\"WDay\",  \"w\";\n"
 "\"Day\",   \"dd\";\n"
 "\"Month\", \"mm\";\n"
 "\"Year\",  \"yyyy\";\n"
 "}\n"
 "alias unit {\n"
 "\"d\", \"day\";\n"
 "\"m\", \"month\";\n"
 "\"y\", \"year\";\n"
 "\"w\", \"week\";\n"
 "}\n"
 "format \"dmy\",   \"(Day) |(Month:m.a) |(Year) |(CEra:ce.a)\";\n"
 "format \"wdmy\",  \"(WDay:w.a) |(Day) |(Month:m.a) |(Year) |(CEra:ce.a)\";\n"
 "format \"out\" {\n"
 "output \"(Day:+os) |(Month:m) |(Year) |(CEra:ce.a)\";\n"
 "}\n"
 "format \"full\" {\n"
 "output \"(WDay:w) |(Day:+os) |(Month:m) |(Year) |(CEra:ce)\";\n"
 "}\n"
 "format \"dmy+\",  \"(Day) |(Month:m) |(Year) |(CEra:ce)\";\n"
 "format \"wdmy+\", \"(WDay:w) |(Day) |(Month:m) |(Year) |(CEra:ce)\";\n"
 "format \"mdy\",   \"(Month:m.a) |(Day), |(Year) |(CEra:ce.a)\";\n"
 "format \"ymd\",   \"(Year)|(CEra:ce.a)|:(Month:m.a)|:(Day)\";\n"
 "pref \"dmy\";\n"
 "}\n"
 "scheme \"jce\" {\n"
 "name \"Julian Common Era\";\n"
 "base julian;\n"
 "grammar \"jce\";\n"
 "}\n",
     /* english_reg.hcs */
 "vocab \"er\" {\n"
 "name \"English Monarchs\";\n"
 "fieldname \"era\";\n"
 "lang \"en\";\n"
 "stylename \"Reign\",              \"Rn\";\n"
 "tokens {\n"
 "0, \"English calendar\",      \"Eng\";\n"
 "1, \"William I\",             \"Wm I\";\n"
 "2, \"William II\",            \"Wm II\";\n"
 "3, \"Henry I\",               \"Hy I\";\n"
 "4, \"Stephen\",               \"St\";\n"
 "5, \"Henry II\",              \"Hy II\";\n"
 "6, \"Richard I\",             \"Rich I\";\n"
 "7, \"John\",                  \"Jn\";\n"
 "8, \"Henry III\",             \"Hy III\";\n"
 "9, \"Edward I\",              \"Ed I\";\n"
 "10, \"Edward II\",            \"Ed II\";\n"
 "11, \"Edward III\",           \"Ed III\";\n"
 "12, \"Richard II\",           \"Rich II\";\n"
 "13, \"Henry IV\",             \"Hy IV\";\n"
 "14, \"Henry V\",              \"Hy V\";\n"
 "15, \"Henry VI\",             \"Hy VI\";\n"
 "16, \"Edward IV\",            \"Ed IV\";\n"
 "17, \"Henry VI Restoration\", \"Hy VI R\";\n"
 "18, \"Edward V\",             \"Ed V\";\n"
 "19, \"Richard III\",          \"Rich III\";\n"
 "20, \"Henry VII\",            \"Hy VII\";\n"
 "21, \"Henry VIII\",           \"Hy VIII\";\n"
 "22, \"Edward VI\",            \"Ed VI\";\n"
 "23, \"Jane\";\n"
 "24, \"Mary I\";\n"
 "25, \"Philip and Mary\",      \"P & M\";\n"
 "26, \"Elizabeth I\",          \"Eliz I\";\n"
 "27, \"James I\",              \"Jm I\";\n"
 "28, \"Charles I\",            \"Chas I\";\n"
 "29, \"The Commonwealth\",     \"Com\";\n"
 "30, \"Charles II\",           \"Chas II\";\n"
 "31, \"James II\",             \"Jm II\";\n"
 "32, \"Interregnum\",          \"Int\";\n"
 "33, \"William and Mary\",     \"W & M\";\n"
 "34, \"William III\",          \"Wm III\";\n"
 "35, \"Anne\";\n"
 "36, \"George I\",             \"Geo I\";\n"
 "37, \"George II\",            \"Geo II\";\n"
 "38, \"George III\",           \"Geo III\";\n"
 "39, \"George IV\",            \"Geo IV\";\n"
 "40, \"William IV\",           \"Wm IV\";\n"
 "41, \"Victoria\",             \"Vic\";\n"
 "42, \"Edward VII\",           \"Ed VII\";\n"
 "43, \"George V\",             \"Geo V\";\n"
 "44, \"Edward VIII\",          \"Ed VIII\";\n"
 "45, \"George VI\",            \"Geo VI\";\n"
 "46, \"Elizabeth II\",         \"Eliz II\";\n"
 "}\n"
 "}\n"
 "vocab \"on\" {\n"
 "name \"Scheme names\";\n"
 "fieldname \"scheme\";\n"
 "lang \"en\";\n"
 "stylename \"Scheme\", \"Sch\";\n"
 "tokens {\n"
 "0, \"Old Style\", \"os\";\n"
 "1, \"New Style\", \"ns\";\n"
 "}\n"
 "}\n"
 "vocab \"part\" {\n"
 "name \"Part of Reign Year\";\n"
 "fieldname \"part\";\n"
 "lang \"en\";\n"
 "stylename \"Part\", \"Part\";\n"
 "tokens {\n"
 "0, \"First\";\n"
 "1, \"Second\";\n"
 "}\n"
 "}\n"
 "grammar \"er\" {\n"
 "optional \"wday\";\n"
 "alias field {\n"
 "\"WDay\",       \"wday\";\n"
 "\"Reign\",      \"era\";\n"
 "\"Day\",        \"day\";\n"
 "\"Month\",      \"month\";\n"
 "\"Year\",       \"year\";\n"
 "\"Hist-Year\",  \"hyear\";\n"
 "\"Part\",       \"part\";\n"
 "\"Scheme\",     \"scheme\";\n"
 "}\n"
 "alias stylename {\n"
 "\"Day\",       \"dd\";\n"
 "\"Month\",     \"mm\";\n"
 "\"Year\",      \"yy\";\n"
 "\"Hist-Year\", \"y\";\n"
 "\"Scheme\",    \"s\";\n"
 "}\n"
 "vocabs \"er\", \"m\", \"w\", \"on\", \"part\";\n"
 "format \"dmyr\",  \"(Day) |(Month:m.a)|, (Part:part)| (Year/Hist-Year) |(Reign:er)|, (Scheme:on.a)\";\n"
 "format \"wdmyr\", \"(WDay:w.a) |(Day) |(Month:m.a)| (Year/Hist-Year) |(Reign:er)|, (Scheme:on.a)\";\n"
 "format \"full\" {\n"
 "output \"The (Day:+os) day of |(Month:m) |in the (Year:+os) year |of the reign of (Reign:er)|, (Scheme:on)\";\n"
 "}\n"
 "pref \"dmyr\";\n"
 "}\n"
 "scheme \"er\" {\n"
 "name \"English Regnal\";\n"
 "regnal {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "extended \"hyear\", \"part\", \"scheme\";\n"
 "era {\n"
 "scheme \"eng\";\n"
 "match \"unshift\", \"hyear\";\n"
 "}\n"
 "era {\n"
 "range 2110773~2118336;\n"
 "scheme 1 { shift \"j\", 2110773; }\n"
 "}\n"
 "era {\n"
 "range 2118353~2123047;\n"
 "scheme 2 { shift \"j\", 2118353; }\n"
 "}\n"
 "era {\n"
 "range 2123050~2135951;\n"
 "scheme 3 { shift \"j\", 2123050; }\n"
 "}\n"
 "era {\n"
 "range 2135972~2142854;\n"
 "scheme 4 { shift \"j\", 2135972; }\n"
 "}\n"
 "era {\n"
 "range 2142909~2155527;\n"
 "scheme 5 { shift \"j\", 2142909; }\n"
 "}\n"
 "era {\n"
 "range 2155586~2159088;\n"
 "scheme 6 { shift \"j\", 2155586; }\n"
 "}\n"
 "era {\n"
 "range 2159139~2165494;\n"
 "scheme 7 {\n"
 "regnal {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "extended \"john-part\";\n"
 "fixed \"year\", 1;\n"
 "era {\n"
 "scheme \"j\";\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#27May1199\" ~ date \"j:dmy#17May1200\";\n"
 "scheme 1 { shift \"j\", date \"j:dmy#27May1199\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#18May1200\" ~ date \"j:dmy#2May1201\";\n"
 "scheme 2 { shift \"j\", date \"j:dmy#18May1200\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#3May1201\" ~ date \"j:dmy#22May1202\";\n"
 "scheme 3 {\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme 0 { shift \"j\", date \"j:dmy#3May1201\"; }\n"
 "change date \"j:dmy#3May1202\";\n"
 "scheme 1 { shift \"j\", date \"j:dmy#3May1202\"; }\n"
 "}\n"
 "}\n"
 "match \"scheme\", \"john-part\";\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#23May1202\" ~ date \"j:dmy#14May1203\";\n"
 "scheme 4 { shift \"j\", date \"j:dmy#23May1202\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#15May1203\" ~ date \"j:dmy#2Jun1204\";\n"
 "scheme 5 {\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme 0 { shift \"j\", date \"j:dmy#15May1203\"; }\n"
 "change date \"j:dmy#15May1204\";\n"
 "scheme 1 { shift \"j\", date \"j:dmy#15May1204\"; }\n"
 "}\n"
 "}\n"
 "match \"scheme\", \"john-part\";\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#3Jun1204\" ~ date \"j:dmy#18May1205\";\n"
 "scheme 6 { shift \"j\", date \"j:dmy#3Jun1204\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#19May1205\" ~ date \"j:dmy#10May1206\";\n"
 "scheme 7 { shift \"j\", date \"j:dmy#19May1205\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#11May1206\" ~ date \"j:dmy#30May1207\";\n"
 "scheme 8 {\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme 0 { shift \"j\", date \"j:dmy#11May1206\"; }\n"
 "change date \"j:dmy#11May1207\";\n"
 "scheme 1 { shift \"j\", date \"j:dmy#11May1207\"; }\n"
 "}\n"
 "}\n"
 "match \"scheme\", \"john-part\";\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#31May1207\" ~ date \"j:dmy#14May1208\";\n"
 "scheme 9 { shift \"j\", date \"j:dmy#31May1207\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#15May1208\" ~ date \"j:dmy#6May1209\";\n"
 "scheme 10 { shift \"j\", date \"j:dmy#15May1208\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#7May1209\" ~ date \"j:dmy#26May1210\";\n"
 "scheme 11 {\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme 0 { shift \"j\", date \"j:dmy#7May1209\"; }\n"
 "change date \"j:dmy#7May1210\";\n"
 "scheme 1 { shift \"j\", date \"j:dmy#7May1210\"; }\n"
 "}\n"
 "}\n"
 "match \"scheme\", \"john-part\";\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#27May1210\" ~ date \"j:dmy#11May1211\";\n"
 "scheme 12 { shift \"j\", date \"j:dmy#27May1210\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#12May1211\" ~ date \"j:dmy#2May1212\";\n"
 "scheme 13 { shift \"j\", date \"j:dmy#12May1211\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#3May1212\" ~ date \"j:dmy#22May1213\";\n"
 "scheme 14 {\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme 0 { shift \"j\", date \"j:dmy#3May1212\"; }\n"
 "change date \"j:dmy#3May1213\";\n"
 "scheme 1 { shift \"j\", date \"j:dmy#3May1213\"; }\n"
 "}\n"
 "}\n"
 "match \"scheme\", \"john-part\";\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#23May1213\" ~ date \"j:dmy#7May1214\";\n"
 "scheme 15 { shift \"j\", date \"j:dmy#23May1213\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#8May1214\" ~ date \"j:dmy#27May1215\";\n"
 "scheme 16 {\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme 0 { shift \"j\", date \"j:dmy#8May1214\"; }\n"
 "change date \"j:dmy#8May1215\";\n"
 "scheme 1 { shift \"j\", date \"j:dmy#8May1215\"; }\n"
 "}\n"
 "}\n"
 "match \"scheme\", \"john-part\";\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#28May1215\" ~ date \"j:dmy#18May1216\";\n"
 "scheme 17 { shift \"j\", date \"j:dmy#28May1215\"; }\n"
 "}\n"
 "era {\n"
 "range date \"j:dmy#19May1216\" ~ date \"j:dmy#19Oct1216\";\n"
 "scheme 18 { shift \"j\", date \"j:dmy#19May1216\"; }\n"
 "}\n"
 "}\n"
 "}\n"
 "match \"era\", \"year\",  \"john-part\", \"part\";\n"
 "}\n"
 "era {\n"
 "range 2165503~2185976;\n"
 "scheme 8 { shift \"j\", 2165503; }\n"
 "}\n"
 "era {\n"
 "range 2185980~2198627;\n"
 "scheme 9 { shift \"j\", 2185980; }\n"
 "}\n"
 "era {\n"
 "range 2198628~2205764;\n"
 "scheme 10 { shift \"j\", 2198628; }\n"
 "}\n"
 "era {\n"
 "range 2205769~2224179;\n"
 "scheme 11 { shift \"j\", 2205769; }\n"
 "}\n"
 "era {\n"
 "range 2224180~2232314;\n"
 "scheme 12 { shift \"j\", 2224180; }\n"
 "}\n"
 "era {\n"
 "range 2232315~2237235;\n"
 "scheme 13 { shift \"j\", 2232315; }\n"
 "}\n"
 "era {\n"
 "range 2237236~2240686;\n"
 "scheme 14 { shift \"j\", 2237236; }\n"
 "}\n"
 "era {\n"
 "range 2240687~2254751;\n"
 "scheme 15 { shift \"j\", 2240687; }\n"
 "}\n"
 "era {\n"
 "range 2254751~2262822;\n"
 "scheme 16 { shift \"j\", 2254751; }\n"
 "}\n"
 "era {\n"
 "range 2258251~2258441;\n"
 "scheme 17 { shift \"j\", 2240687; }\n"
 "}\n"
 "era {\n"
 "range 2262822~2262899;\n"
 "scheme 18 { shift \"j\", 2262822; }\n"
 "}\n"
 "era {\n"
 "range 2262900~2263688;\n"
 "scheme 19 { shift \"j\", 2262900; }\n"
 "}\n"
 "era {\n"
 "range 2263688~2272331;\n"
 "scheme 20 { shift \"j\", 2263688; }\n"
 "}\n"
 "era {\n"
 "range 2272332~2286127;\n"
 "scheme 21 { shift \"j\", 2272332; }\n"
 "}\n"
 "era {\n"
 "range 2286127~2288478;\n"
 "scheme 22 { shift \"j\", 2286127; }\n"
 "}\n"
 "era {\n"
 "range 2288478~2288491;\n"
 "scheme 23 { shift \"j\", 2288478; }\n"
 "}\n"
 "era {\n"
 "range 2288491~2288861;\n"
 "scheme 24 { shift \"j\", 2288478; }\n"
 "}\n"
 "era {\n"
 "range 2288862~2290438;\n"
 "scheme 25 { shift \"j\", 2288862; }\n"
 "}\n"
 "era {\n"
 "range 2290438~2306636;\n"
 "scheme 26 { shift \"j\", 2290438; }\n"
 "}\n"
 "era {\n"
 "range 2306636~2314675;\n"
 "scheme 27 { shift \"j\", 2306636; }\n"
 "}\n"
 "era {\n"
 "range 2314675~2323385;\n"
 "scheme 28 { shift \"j\", 2314675; }\n"
 "}\n"
 "era {\n"
 "range 2323386~2327521;\n"
 "scheme \"ja\";\n"
 "match \"unshift\", \"hyear\";\n"
 "}\n"
 "era {\n"
 "range 2327522~2336541;\n"
 "scheme 30 { shift \"j\", 2323385; }\n"
 "}\n"
 "era {\n"
 "range 2336541~2337945;\n"
 "scheme 31 { shift \"j\", 2336541; }\n"
 "}\n"
 "era {\n"
 "range 2337946~2338008;\n"
 "scheme \"eng\";\n"
 "match \"unshift\", \"hyear\";\n"
 "}\n"
 "era {\n"
 "range 2338009~2340152;\n"
 "scheme 33 { shift \"j\", 2338009; }\n"
 "}\n"
 "era {\n"
 "range 2340153~2342780;\n"
 "scheme 34 { shift \"j\", 2337962; }\n"
 "}\n"
 "era {\n"
 "range 2342780~2347309;\n"
 "scheme 35 { shift \"j\", 2342780; }\n"
 "}\n"
 "era {\n"
 "range 2347309~2352006;\n"
 "scheme 36 { shift \"j\", 2347309; }\n"
 "}\n"
 "era {\n"
 "range 2352006~2364185;\n"
 "scheme 37 {\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "scheme 0 { shift \"j\", 2352006; }\n"
 "change 2361222;\n"
 "scheme 1 { shift \"g\", 2352006; }\n"
 "}\n"
 "}\n"
 "}\n"
 "era {\n"
 "range 2364185~2385829;\n"
 "scheme 38 { shift \"g\", 2364185; }\n"
 "}\n"
 "era {\n"
 "range 2385829~2389630;\n"
 "scheme 39 { shift \"g\", 2385829; }\n"
 "}\n"
 "era {\n"
 "range 2389630~2392181;\n"
 "scheme 40 { shift \"g\", 2389630; }\n"
 "}\n"
 "era {\n"
 "range 2392181~2415407;\n"
 "scheme 41 { shift \"g\", 2392181; }\n"
 "}\n"
 "era {\n"
 "range 2415407~2418798;\n"
 "scheme 42 { shift \"g\", 2415407; }\n"
 "}\n"
 "era {\n"
 "range 2418798~2428188;\n"
 "scheme 43 { shift \"g\", 2418798; }\n"
 "}\n"
 "era {\n"
 "range 2428188~2428514;\n"
 "scheme 44 { shift \"g\", 2428188; }\n"
 "}\n"
 "era {\n"
 "range 2428514~2434049;\n"
 "scheme 45 { shift \"g\", 2428514; }\n"
 "}\n"
 "era {\n"
 "range 2434049~future;\n"
 "scheme 46 { shift \"g\", 2434049; }\n"
 "}\n"
 "}\n"
 "grammar \"er\";\n"
 "}\n",
     /* scottish_reg.hcs */
 "vocab \"sr\" {\n"
 "name \"Scottish Monarchs\";\n"
 "fieldname \"era\";\n"
 "lang \"en\";\n"
 "stylename \"Reign\",               \"Rn\";\n"
 "tokens {\n"
 "0,  \"Scottish calendar\",      \"Scot\";\n"
 "1,  \"Malcolm II\",             \"Mal II\";\n"
 "2,  \"Duncan I\",               \"Dun I\";\n"
 "3,  \"Macbeth\",                \"Mbth\";\n"
 "4,  \"Lulach\",                 \"Lul\";\n"
 "5,  \"Malcolm III\",            \"Mal III\";\n"
 "6,  \"Donald Bane\",            \"Don B\";\n"
 "7,  \"Duncan II\",              \"Dun II\";\n"
 "8,  \"Eadgar\",                 \"Ead\";\n"
 "9,  \"Alexander I\",            \"Alex I\";\n"
 "10, \"David I\",                \"Dav I\";\n"
 "11, \"Malcolm IV\",             \"Mal IV\";\n"
 "12, \"William\",                \"Wm\";\n"
 "13, \"Alexander II\",           \"Alex II\";\n"
 "14, \"Alexander III\",          \"Alex III\";\n"
 "15, \"Margaret\",               \"Marg\";\n"
 "16, \"Interregnum I\",          \"Int I\";\n"
 "17, \"John\",                   \"Jn\";\n"
 "18, \"Interregnum II\",         \"Int II\";\n"
 "19, \"Robert I\",               \"Rob I\";\n"
 "20, \"David II\",               \"Dav II\";\n"
 "21, \"David II Alt\",           \"Dav IIa\";\n"
 "22, \"Robert II\",              \"Rob II\";\n"
 "23, \"Robert III\",             \"Rob III\";\n"
 "24, \"James I\",                \"Jm I\";\n"
 "25, \"James II\",               \"Jm II\";\n"
 "26, \"James III\",              \"Jm III\";\n"
 "27, \"James IV\",               \"Jm IV\";\n"
 "28, \"James V\",                \"Jm V\";\n"
 "29, \"Mary\";\n"
 "30, \"James VI\",               \"Jm VI\";\n"
 "}\n"
 "}\n"
 "grammar \"sr\" {\n"
 "optional \"wday\";\n"
 "alias field {\n"
 "\"WDay\",       \"wday\";\n"
 "\"Reign\",      \"era\";\n"
 "\"Day\",        \"day\";\n"
 "\"Month\",      \"month\";\n"
 "\"Year\",       \"year\";\n"
 "\"Hist-Year\",  \"hyear\";\n"
 "}\n"
 "alias stylename {\n"
 "\"Day\",       \"dd\";\n"
 "\"Month\",     \"mm\";\n"
 "\"Year\",      \"yyyy\";\n"
 "\"Hist-Year\", \"y\";\n"
 "}\n"
 "vocabs \"sr\", \"m\", \"w\";\n"
 "format \"dmyr\",  \"(Day) |(Month:m.a)| (Year/Hist-Year) |(Reign:sr)\";\n"
 "format \"wdmyr\", \"(WDay:w.a) |(Day) |(Month:m.a)| (Year/Hist-Year) |(Reign:sr)\";\n"
 "pref \"dmyr\";\n"
 "}\n"
 "scheme \"sr\" {\n"
 "name \"Scottish Regnal\";\n"
 "regnal {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "extended \"hyear\";\n"
 "era {\n"
 "scheme \"scot\";\n"
 "match \"unshift\", \"hyear\";\n"
 "}\n"
 "era {\n"
 "range 2088218 ~ 2099055;\n"
 "scheme 1 { shift \"j\", 2088218; }\n"
 "}\n"
 "era {\n"
 "range 2099055 ~ 2101144;\n"
 "scheme 2 { shift \"j\", 2099055; }\n"
 "}\n"
 "era {\n"
 "range 2101144 ~ 2107354;\n"
 "scheme 3 { shift \"j\", 2101144; }\n"
 "}\n"
 "era {\n"
 "range 2107354 ~ 2107568;\n"
 "scheme 4 { shift \"j\", 2107354; }\n"
 "}\n"
 "era {\n"
 "range 2107568 ~ 2120593;\n"
 "scheme 5 { shift \"j\", 2107568; }\n"
 "}\n"
 "era {\n"
 "range 2120593 ~ 2122018;\n"
 "scheme 6 { shift \"j\", 2120593; }\n"
 "}\n"
 "era {\n"
 "range 2120773 ~ 2120957;\n"
 "scheme 7 { shift \"j\", 2120773; }\n"
 "}\n"
 "era {\n"
 "range 2122018 ~ 2125397;\n"
 "scheme 8 { shift \"j\", 2122018; }\n"
 "}\n"
 "era {\n"
 "range 2125397 ~ 2131712;\n"
 "scheme 9 { shift \"j\", 2125397; }\n"
 "}\n"
 "era {\n"
 "range 2131712 ~ 2142335;\n"
 "scheme 10 { shift \"j\", 2131712; }\n"
 "}\n"
 "era {\n"
 "range 2142335 ~ 2146917;\n"
 "scheme 11 { shift \"j\", 2142335; }\n"
 "}\n"
 "era {\n"
 "range 2146917 ~ 2164809;\n"
 "scheme 12 { shift \"j\", 2146917; }\n"
 "}\n"
 "era {\n"
 "range 2164809 ~ 2177444;\n"
 "scheme 13 { shift \"j\", 2164809; }\n"
 "}\n"
 "era {\n"
 "range 2177444 ~ 2190847;\n"
 "scheme 14 { shift \"j\", 2177444; }\n"
 "}\n"
 "era {\n"
 "range 2190847 ~ 2192499;\n"
 "scheme 15 { shift \"j\", 2190847; }\n"
 "}\n"
 "era {\n"
 "range 2192500 ~ 2193281;\n"
 "scheme \"scot\";\n"
 "match \"unshift\", \"hyear\";\n"
 "}\n"
 "era {\n"
 "range 2193282 ~ 2194613;\n"
 "scheme 17 { shift \"j\", 2193282; }\n"
 "}\n"
 "era {\n"
 "range 2194614 ~ 2198159;\n"
 "scheme \"scot\";\n"
 "match \"unshift\", \"hyear\";\n"
 "}\n"
 "era {\n"
 "range 2198160 ~ 2206633;\n"
 "scheme 19 { shift \"j\", 2198160; }\n"
 "}\n"
 "era {\n"
 "range 2206633 ~ 2221868;\n"
 "scheme 20 { shift \"j\", 2206633; }\n"
 "}\n"
 "era {\n"
 "range 2215033 ~ 2221868;\n"
 "scheme 21 { shift \"j\", 2206268; }\n"
 "}\n"
 "era {\n"
 "range 2221868 ~ 2228864;\n"
 "scheme 22 { shift \"j\", 2221868; }\n"
 "}\n"
 "era {\n"
 "range 2228864 ~ 2234693;\n"
 "scheme 23 { shift \"j\", 2228864; }\n"
 "}\n"
 "era {\n"
 "range 2234693 ~ 2245974;\n"
 "scheme 24 { shift \"j\", 2234693; }\n"
 "}\n"
 "era {\n"
 "range 2245974 ~ 2254538;\n"
 "scheme 25 { shift \"j\", 2245974; }\n"
 "}\n"
 "era {\n"
 "range 2254538 ~ 2264712;\n"
 "scheme 26 { shift \"j\", 2254538; }\n"
 "}\n"
 "era {\n"
 "range 2264712 ~ 2273933;\n"
 "scheme 27 { shift \"j\", 2264712; }\n"
 "}\n"
 "era {\n"
 "range 2273933 ~ 2284621;\n"
 "scheme 28 { shift \"j\", 2273933; }\n"
 "}\n"
 "era {\n"
 "range 2284621 ~ 2293609;\n"
 "scheme 29 { shift \"j\", 2284621; }\n"
 "}\n"
 "era {\n"
 "range 2293609 ~ 2361221;\n"
 "scheme 29 { shift \"j\", 2293609; }\n"
 "}\n"
 "}\n"
 "grammar \"sr\";\n"
 "}\n",
     /* french_rep.hcs */
 "vocab \"frm\" {\n"
 "name \"Month names\";\n"
 "fieldname \"month\";\n"
 "lang \"en\";\n"
 "stylename \"Month\",  \"Mon\";\n"
 "tokens {\n"
 "1, \"Vend\303\251miaire\",      \"Vend\";\n"
 "2, \"Brumaire\",         \"Brum\";\n"
 "3, \"Frimaire\",         \"Frim\";\n"
 "4, \"Niv\303\264se\",           \"Nivo\";\n"
 "5, \"Pluvi\303\264se\",         \"Pluv\";\n"
 "6, \"Vent\303\264se\",          \"Vent\";\n"
 "7, \"Germinal\",         \"Germ\";\n"
 "8, \"Flor\303\251al\",          \"Flor\";\n"
 "9, \"Prairial\",         \"Prai\";\n"
 "10, \"Messidor\",        \"Mess\";\n"
 "11, \"Thermidor\",       \"Ther\";\n"
 "12, \"Fructidor\",       \"Fruc\";\n"
 "13, \"Compl\303\251mentaires\", \"Comp\";\n"
 "}\n"
 "}\n"
 "vocab \"frdd\" {\n"
 "name \"Decade Day\";\n"
 "fieldname \"dday\";\n"
 "lang \"en\";\n"
 "stylename \"DecDay\",  \"DDay\";\n"
 "tokens {\n"
 "1, \"Primidi\",  \"Prim\";\n"
 "2, \"Duodi\",    \"Duo\";\n"
 "3, \"Tridi\",    \"Tri\";\n"
 "4, \"Quartidi\", \"Quar\";\n"
 "5, \"Quintidi\", \"Quin\";\n"
 "6, \"Sextidi\",  \"Sext\";\n"
 "7, \"Septidi\",  \"Sept\";\n"
 "8, \"Octidi\",   \"Oct\";\n"
 "9, \"Nonidi\",   \"Non\";\n"
 "10, \"D\303\251cadi\",  \"Dec\";\n"
 "}\n"
 "}\n"
 "vocab \"frcomp\" {\n"
 "name \"Complementary Days\";\n"
 "fieldname \"cday\";\n"
 "lang \"en\";\n"
 "stylename \"CompDay\",           \"CDay\";\n"
 "tokens {\n"
 "1, \"F\303\252te de la Vertu\",      \"Ver\";\n"
 "2, \"F\303\252te du G\303\251nie\",         \"Gen\";\n"
 "3, \"F\303\252te du Travail\",       \"Trav\";\n"
 "4, \"F\303\252te de l'Opinion\",     \"Opin\";\n"
 "5, \"F\303\252te des R\303\251compenses\",  \"Rec\";\n"
 "6, \"F\303\252te de la R\303\251volution\", \"Rev\";\n"
 "}\n"
 "}\n"
 "grammar \"fr\" {\n"
 "optional \"nmonth\", \"nmday\", \"cday\", \"dday\", \"wday\";\n"
 "vocabs \"frm\", \"frcomp\", \"frdd\", \"w\";\n"
 "alias field {\n"
 "\"Day\",     \"day\";\n"
 "\"Month\",   \"month\";\n"
 "\"Year\",    \"year\";\n"
 "\"DecDay\",  \"dday\";\n"
 "\"WDay\",    \"wday\";\n"
 "\"NMonth\",  \"nmonth\";\n"
 "\"NMDay\",   \"nmday\";\n"
 "\"CompDay\", \"cday\";\n"
 "}\n"
 "alias stylename {\n"
 "\"Day\",     \"dd\";\n"
 "\"Month\",   \"mm\";\n"
 "\"Year\",    \"yyyy\";\n"
 "\"NMonth\",  \"mm\";\n"
 "\"NMDay\",   \"dd\";\n"
 "\"WDay\",    \"w\";\n"
 "\"DecDay\",  \"d\";\n"
 "\"CompDay\", \"c\";\n"
 "}\n"
 "alias unit {\n"
 "\"d\",   \"day\";\n"
 "\"dec\", \"decade\";\n"
 "\"w\",   \"week\";\n"
 "\"m\",   \"month\";\n"
 "\"y\",   \"year\";\n"
 "}\n"
 "format \"dmy\",    \"(Day) |(Month:frm.a) |(Year)\";\n"
 "format \"dmy+\",   \"(Day) |(Month:frm) |(Year)\";\n"
 "format \"ymd\",    \"(Year)|,(Month:frm.a)|,(Day)\";\n"
 "format \"cdmy\" {\n"
 "input \"(Day) |(Month) |(Year)\";\n"
 "output \"(CompDay:frcomp) |(NMDay) |(NMonth:frm) |(Year)\";\n"
 "}\n"
 "format \"out\" {\n"
 "input \"(Day) |(Month) |(Year)\";\n"
 "output \"(CompDay:frcomp) |(NMDay) |(NMonth:frm) |an (Year:!rn)\";\n"
 "}\n"
 "format \"dcdmy\" {\n"
 "input \"(Day) |(Month) |(Year)\";\n"
 "output \"(DecDay:frdd) |(CompDay:frcomp) |(NMDay) |(NMonth:frm) |(Year)\";\n"
 "}\n"
 "format \"wcdmy\" {\n"
 "input \"(Day) |(Month) |(Year)\";\n"
 "output \"(WDay:w), |(CompDay:frcomp) |(NMDay) |(NMonth:frm) |(Year)\";\n"
 "}\n"
 "format \"cdmy-\" {\n"
 "input \"(Day) |(Month) |(Year)\";\n"
 "output \"(CompDay:frcomp.a) |(NMDay) |(NMonth:frm.a) |(Year)\";\n"
 "}\n"
 "format \"dcdmy-\" {\n"
 "input \"(Day) |(Month) |(Year)\";\n"
 "output \"(DecDay:frdd.a) |(CompDay:frcomp.a) |(NMDay) |(NMonth:frm.a) |(Year)\";\n"
 "}\n"
 "format \"wcdmy-\" {\n"
 "input \"(Day) |(Month) |(Year)\";\n"
 "output \"(WDay:w), |(CompDay:frcomp) |(NMDay) |(NMonth:frm) |(Year)\";\n"
 "}\n"
 "pref \"cdmy\";\n"
 "}\n"
 "scheme \"fr\" {\n"
 "name \"French Republic\";\n"
 "base french;\n"
 "grammar \"fr\";\n"
 "}\n",
     /* hebrew.hcs */
 "vocab \"hm\" {\n"
 "name \"Month names\";\n"
 "fieldname \"month\";\n"
 "lang \"en\";\n"
 "stylename \"Month\",   \"Mon\";\n"
 "tokens {\n"
 "1, \"Nisan\",      \"Nsn\";\n"
 "2, \"Iyyar\",      \"Iyr\";\n"
 "3, \"Sivan\",      \"Svn\";\n"
 "4, \"Tammuz\",     \"Tmz\";\n"
 "5, \"Av\",         \"Aav\";\n"
 "6, \"Elul\",       \"Ell\";\n"
 "7, \"Tishri\",     \"Tsh\";\n"
 "8, \"Marheshvan\", \"Csh\";\n"
 "9, \"Kislev\",     \"Ksl\";\n"
 "10, \"Tevet\",     \"Tvt\";\n"
 "11, \"Shevat\",    \"Shv\";\n"
 "12, \"Adar\",      \"Adr\";\n"
 "13, \"Adar II\",   \"Ads\";\n"
 "}\n"
 "}\n"
 "vocab \"hw\" {\n"
 "name \"Weekday names\";\n"
 "fieldname \"wsday\";\n"
 "lang \"en\";\n"
 "stylename \"Weekday\", \"WDay\";\n"
 "tokens {\n"
 "1, \"Yom Rishon\",   \"Rish\";\n"
 "2, \"Yom Sheni\",    \"Shen\";\n"
 "3, \"Yom Shelishi\", \"Shel\";\n"
 "4, \"Yom Revi'i\",   \"Revi\";\n"
 "5, \"Yom Hamishi\",  \"Hami\";\n"
 "6, \"Yom Shishi\",   \"Shis\";\n"
 "7, \"Yom Shabbat\",  \"Shab\";\n"
 "}\n"
 "}\n"
 "grammar \"h\" {\n"
 "optional \"wsday\";\n"
 "vocabs \"hm\", \"hw\";\n"
 "alias field {\n"
 "\"WDay\",  \"wsday\";\n"
 "\"Day\",   \"day\";\n"
 "\"Month\", \"month\";\n"
 "\"Year\",  \"year\";\n"
 "}\n"
 "alias stylename {\n"
 "\"WDay\",  \"w\";\n"
 "\"Day\",   \"dd\";\n"
 "\"Month\", \"mm\";\n"
 "\"Year\",  \"yyyy\";\n"
 "}\n"
 "alias unit {\n"
 "\"d\", \"day\";\n"
 "\"m\", \"month\";\n"
 "\"y\", \"year\";\n"
 "\"w\", \"week\";\n"
 "}\n"
 "format \"dmy\",    \"(Day) |(Month:hm) |(Year)\";\n"
 "format \"dmy.a\",  \"(Day) |(Month:hm.a) |(Year)\";\n"
 "format \"dmy.d\",  \"(Day) |(Month) |(Year)\";\n"
 "format \"wdmy\",   \"(WDay:hw) |(Day) |(Month:hm) |(Year)\";\n"
 "format \"wdmy.a\", \"(WDay:hw.a) |(Day) |(Month:hm.a) |(Year)\";\n"
 "format \"ymd\",    \"(Year)|,(Month:hm.a)|,(Day)\";\n"
 "pref \"dmy\";\n"
 "}\n"
 "scheme \"h\" {\n"
 "name \"Hebrew\";\n"
 "base hebrew;\n"
 "grammar \"h\";\n"
 "}\n",
     /* islamic.hcs */
 "vocab \"im\" {\n"
 "name \"Month names\";\n"
 "fieldname \"month\";\n"
 "lang \"en\";\n"
 "stylename \"Month\", \"Mon\";\n"
 "tokens {\n"
 "1, \"Mu\341\270\245arram\",         \"Muharram\";\n"
 "2, \"\341\271\242afar\",            \"Safar\";\n"
 "3, \"Rab\304\253\342\200\230 al-Awwal\",   \"Rabi' I\";\n"
 "4, \"Rab\304\253\342\200\230 al-\304\200khir\",   \"Rabi' II\";\n"
 "5, \"Jum\304\201d\303\241 al-\305\252l\303\241\",    \"Jumada I\";\n"
 "6, \"Jum\304\201d\303\241 al-\304\200khira\", \"Jumada II\";\n"
 "7, \"Rajab\";\n"
 "8, \"Sha\342\200\230b\304\201n\",          \"Sha'aban\";\n"
 "9, \"Rama\341\270\215\304\201n\",          \"Ramadan\";\n"
 "10, \"Shaww\304\201l\",         \"Shawwal\";\n"
 "11, \"Dh\305\253 al-Qa\342\200\230dah\",   \"Dhu al-Qi'dah\";\n"
 "12, \"Dh\305\253 al-\341\270\244ijjah\",   \"Dhu al-Hijjah\";\n"
 "}\n"
 "}\n"
 "vocab \"iw\" {\n"
 "name \"Weekday names\";\n"
 "fieldname \"wsday\";\n"
 "lang \"en\";\n"
 "stylename \"Weekday\", \"WDay\";\n"
 "tokens {\n"
 "1, \"Yawm al-A\341\270\245ad\",       \"Ahad\";\n"
 "2, \"Yawm al-Ithnayn\",    \"Ithn\";\n"
 "3, \"Yawm ath-Thul\304\201th\304\201\312\274\", \"Thul\";\n"
 "4, \"Yawm al-Arbi\342\200\230\304\201\342\200\231\",    \"Arbi\";\n"
 "5, \"Yawm al-Kham\304\253s\",     \"Kham\";\n"
 "6, \"Yawm al-Jumu\342\200\230ah\",    \"Jumu\";\n"
 "7, \"Yawm as-Sabt\",       \"Sabt\";\n"
 "}\n"
 "}\n"
 "grammar \"i\" {\n"
 "optional \"wsday\";\n"
 "vocabs \"im\", \"iw\";\n"
 "alias field {\n"
 "\"WDay\",  \"wsday\";\n"
 "\"Day\",   \"day\";\n"
 "\"Month\", \"month\";\n"
 "\"Year\",  \"year\";\n"
 "}\n"
 "alias stylename {\n"
 "\"WDay\",  \"w\";\n"
 "\"Day\",   \"dd\";\n"
 "\"Month\", \"mm\";\n"
 "\"Year\",  \"yyyy\";\n"
 "}\n"
 "alias unit {\n"
 "\"d\", \"day\";\n"
 "\"m\", \"month\";\n"
 "\"y\", \"year\";\n"
 "\"w\", \"week\";\n"
 "}\n"
 "format \"dmy\",    \"(Day) |(Month:im) |(Year)\";\n"
 "format \"dmy.a\",  \"(Day) |(Month:im.a) |(Year)\";\n"
 "format \"dmy.d\",  \"(Day) |(Month) |(Year)\";\n"
 "format \"wdmy\",   \"(WDay:iw) |(Day) |(Month:im) |(Year)\";\n"
 "format \"wdmy.a\", \"(WDay:iw.a) |(Day) |(Month:im.a) |(Year)\";\n"
 "format \"ymd\",    \"(Year)|,(Month:im.a)|,(Day)\";\n"
 "pref \"dmy\";\n"
 "}\n"
 "scheme \"i\" {\n"
 "name \"Islamic Tabular\";\n"
 "base islamic IIc;\n"
 "grammar \"i\";\n"
 "}\n"
 "scheme \"ims\" {\n"
 "name \"Islamic Microsoft Hijri\";\n"
 "base islamic IIa;\n"
 "grammar \"i\";\n"
 "}\n"
 "scheme \"if\" {\n"
 "name \"Islamic F\304\201timid\";\n"
 "base islamic IIIa;\n"
 "grammar \"i\";\n"
 "}\n",
};

size_t Cal::cal_default_scripts_size = sizeof(Cal::cal_default_scripts) / sizeof(char*);

// End of scripts/caldefscripts.f2c file
