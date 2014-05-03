/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calliturgical.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base for Liturgical calendars header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     14th April 2014
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

*/

#ifndef CAL_CALLITURGICAL_H_GUARD
#define CAL_CALLITURGICAL_H_GUARD

#include "calbase.h"

namespace Cal {

    extern Field liturgical_get_week( const Base* base, Field jdn );

}

#endif // CAL_CALLITURGICAL_H_GUARD
