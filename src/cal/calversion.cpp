/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calversion.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Library version number.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     19th September 2013
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

#include "calversion.h"

#define VERSION_NUM      "0.2.1"

#if defined(NDEBUG) || !defined(_DEBUG)
#define VERSION_CONFIG   ""
#else
#define VERSION_CONFIG   " debug"
#endif

using namespace Cal;

/*! A string containing the current full version number.
 */
const char* Cal::cal_version = VERSION_NUM VERSION_CONFIG;

/*****************************************************************************

 Version history.
 19sep13  v0.1.x  Library created and developed with The Family Pack project.

 11jan14  v0.2.0  Library modified for Programmable Calendric Calculator.

 25nov14  v0.2.1  Library renamed Cal libary. Home project renamed HistoryCal. 

*****************************************************************************/

// End of src/cal/calversion.cpp file
