/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/soak/soakcommon.h
 * Project:     Cal Soak test: Extended Unit Tests for Cal library API.
 * Purpose:     CppUnit test program common header
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     8th October 2013
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


#ifndef SOAK_SOAKCOMMON_H_GUARD
#define SOAK_SOAKCOMMON_H_GUARD

#include <cal/calendars.h>

// Only one of these should defined at any one time.
//#define CALTEST_SHORT
//#define CALTEST_LONG
#define CALTEST_SOAK

extern std::string xymdToStr( Cal::Field extra, Cal::Field year, Cal::Field month, Cal::Field day );
extern std::string ymdToStr( Cal::Field year, Cal::Field month, Cal::Field day );
extern std::string ymToStr( Cal::Field year, Cal::Field month );
extern std::string yToStr( Cal::Field year );

#endif // SOAK_SOAKCOMMON_H_GUARD
