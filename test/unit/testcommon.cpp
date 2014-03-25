/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/test/testcommon.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     CppUnit test program common source
 * Author:      Nick Matthews
 * Website:     http://pcalcalc.org
 * Created:     7th October 2013
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

#include "testcommon.h"

#include <sstream>


using namespace std;
using namespace Cal;

Field testJdnValues[MaxSample] = {
    1507232,
    1660038,
    1746894,
    1770642,
    1892732,
    1931580,
    1974852,
    2091165,
    2121510,
    2155780,
    2174030,
    2191585,
    2195262,
    2229275,
    2245581,
    2266101,
    2288543,
    2290902,
    2323141,
    2334849,
    2348021,
    2366979,
    2385649,
    2392826,
    2416224,
    2425849,
    2430267,
    2430834,
    2431005,
    2448699,
    2450139,
    2465738,
    2486077
};

string xymdToStr( Field extra, Field year, Field month, Field day )
{
    stringstream tst; 
    tst << extra << " " << year << " " << month << " " << day;
    return tst.str();
}

string ymdToStr( Field year, Field month, Field day )
{
    stringstream tst; 
    tst << year << " " << month << " " << day;
    return tst.str();
}

string ymToStr( Field year, Field month )
{
    stringstream tst; 
    tst << year << " " << month;
    return tst.str();
}

string yToStr( Field year )
{
    stringstream tst; 
    tst << year;
    return tst.str();
}

const char* invalid = "";

// End of src/test/testcommon.h file
