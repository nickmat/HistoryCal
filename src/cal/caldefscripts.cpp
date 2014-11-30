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
 "}\n"
 "grammar \"d\" {\n"
 "alias field {\n"
 "\"Day\", \"day\";\n"
 "}\n"
 "alias stylename {\n"
 "\"Day\", \"ddddd\";\n"
 "}\n"
 "format \"d\", \"@(Day)\";\n"
 "pref \"d\";\n"
 "}\n",
     /* jdn.hcs */
 "scheme \"jdn\" {\n"
 "name \"Julian Day Number\";\n"
 "base jdn;\n"
 "grammar \"d\";\n"
 "}\n",
     /* julian.hcs */
 "vocab \"m\" {\n"
 "name \"Month names\";\n"
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
 "format \"dmy\",   \"@(Day) @(Month:m.a) @(Year)\";\n"
 "format \"wdmy\",  \"@(WDay:w.a) @(Day) @(Month:m.a) @(Year)\";\n"
 "format \"dmy+\",  \"@(Day) @(Month:m) @(Year)\";\n"
 "format \"wdmy+\", \"@(WDay:w) @(Day) @(Month:m) @(Year)\";\n"
 "format \"mdy\",   \"@(Month:m.a) @(Day), @(Year)\";\n"
 "format \"ymd\",   \"@(Year)@:(Month:m.a)@:(Day)\";\n"
 "pref \"dmy\";\n"
 "}\n"
 "scheme \"j\" {\n"
 "name \"Julian\";\n"
 "base julian;\n"
 "grammar \"j\";\n"
 "}\n"
 "scheme \"g\" {\n"
 "name \"Gregorian\";\n"
 "base gregorian;\n"
 "grammar \"j\";\n"
 "}\n"
 "grammar \"j.sh\" {\n"
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
 "format \"dmy\",   \"@(Day) @(Month:m.a) @(Year)\";\n"
 "format \"dmyh\",  \"@(Day) @(Month:m.a) @(Year/Hist-Year)\";\n"
 "format \"wdmyh\", \"@(WDay:w.a) @(Day) @(Month:m.a) @(Year/Hist-Year)\";\n"
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
 "}\n",
     /* julian_hybrid.hcs */
 "vocab \"on\" {\n"
 "name \"Scheme names\";\n"
 "lang \"en\";\n"
 "stylename \"Scheme\", \"Sch\";\n"
 "tokens {\n"
 "0, \"Old Style\", \"os\";\n"
 "1, \"New Style\", \"ns\";\n"
 "}\n"
 "}\n"
 "vocab \"jajg\" {\n"
 "name \"Scheme names\";\n"
 "lang \"en\";\n"
 "stylename \"Scheme\", \"Sch\";\n"
 "tokens {\n"
 "0, \"Julian Ann\", \"ja\";\n"
 "1, \"Julian\",     \"j\";\n"
 "2, \"Gregorian\",  \"g\";\n"
 "}\n"
 "}\n"
 "grammar \"hy\" {\n"
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
 "format \"dmyh\",  \"@(Day) @(Month:m.a) @(Year/Hist-Year)\";\n"
 "format \"dmy\",   \"@(Day) @(Month:m.a) @(Year)\";\n"
 "format \"dmys\",  \"@(Day) @(Month:m.a) @(Year) @(Scheme:jajg.a)\";\n"
 "format \"wdmyh\", \"@(WDay:w.a) @(Day) @(Month:m.a) @(Year/Hist-Year)\";\n"
 "format \"wdmyhs\",\"@(WDay:w.a) @(Day) @(Month:m.a) @(Year/Hist-Year) @(Scheme:jajg)\";\n"
 "pref \"dmyh\";\n"
 "}\n"
 "scheme \"eng\" {\n"
 "name \"English Hybrid\";\n"
 "hybrid {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "extended \"wday\", \"unshift\";\n"
 "scheme \"ja\";\n"
 "change 2360976;\n"
 "scheme \"j\";\n"
 "change 2361222;\n"
 "scheme \"g\";\n"
 "}\n"
 "grammar \"hy\";\n"
 "}\n",
     /* liturgical.hcs */
 "vocab \"lit\" {\n"
 "name \"Liturgy Weeks\";\n"
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
 "format \"wdmyl\",  \"@(WDay:w.a) @(Day) @(Month:m.a) @(Year) @(LitWeek:lit.a)\";\n"
 "format \"wdmyl+\", \"@(WDay:w) @(Day) @(Month:m) @(Year) @(LitWeek:lit)\";\n"
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
     /* english_reg.hcs */
 "vocab \"er\" {\n"
 "name \"English Monarchs\";\n"
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
 "vocab \"part\" {\n"
 "name \"Part of Reign Year\";\n"
 "lang \"en\";\n"
 "stylename \"Part\", \"Part\";\n"
 "tokens {\n"
 "0, \"First\";\n"
 "1, \"Second\";\n"
 "}\n"
 "}\n"
 "grammar \"er\" {\n"
 "alias field {\n"
 "\"WDay\",       \"wday\";\n"
 "\"Reign\",      \"era\";\n"
 "\"Day\",        \"day\";\n"
 "\"Month\",      \"month\";\n"
 "\"Year\",       \"year\";\n"
 "\"Hist-Year\",  \"er-hyear\";\n"
 "\"Part\",       \"er-part\";\n"
 "\"Scheme\",     \"er-scheme\";\n"
 "}\n"
 "alias stylename {\n"
 "\"Day\",       \"dd\";\n"
 "\"Month\",     \"mm\";\n"
 "\"Year\",      \"yyyy\";\n"
 "\"Hist-Year\", \"y\";\n"
 "\"Scheme\",    \"s\";\n"
 "}\n"
 "vocabs \"w\", \"er\", \"m\", \"on\", \"part\";\n"
 "format \"dmyr\",  \"@(Day) @(Month:m.a)@, (Part:part)@ (Year/Hist-Year)@ (Reign:er)@, (Scheme:on.a)\";\n"
 "format \"wdmyr\", \"@(WDay:w.a) @(Day) @(Month:m.a)@ (Year/Hist-Year)@ (Reign:er)@, (Scheme:on.a)\";\n"
 "pref \"dmyr\";\n"
 "}\n"
 "scheme \"er\" {\n"
 "name \"English Regnal\";\n"
 "regnal {\n"
 "fields \"year\", \"month\", \"day\";\n"
 "extended \"er-hyear\", \"wday\", \"er-part\", \"er-scheme\";\n"
 "era {\n"
 "scheme \"eng\";\n"
 "match \"unshift\", \"er-hyear\", \"scheme\", \"er-scheme\";\n"
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
 "match \"era\", \"year\",  \"john-part\", \"er-part\";\n"
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
 "scheme \"eng\";\n"
 "match \"unshift\", \"er-hyear\", \"scheme\", \"er-scheme\";\n"
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
 "match \"unshift\", \"er-hyear\", \"scheme\", \"er-scheme\";\n"
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
 "match \"scheme\", \"er-scheme\";\n"
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
 "range 2434049~2457023;\n"
 "scheme 46 { shift \"g\", 2434049; }\n"
 "}\n"
 "}\n"
 "grammar \"er\";\n"
 "}\n"
};

size_t Cal::cal_default_scripts_size = sizeof(Cal::cal_default_scripts) / sizeof(char*);

// End of scripts/caldefscripts.f2c file
