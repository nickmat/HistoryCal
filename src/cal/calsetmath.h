/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calsetmath.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Functions to carry out set operations.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th May 2014
 * Copyright:   Copyright (c) 2014 ~ 2016, Nick Matthews.
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

#ifndef CAL_CALSETMATH_H_GUARD
#define CAL_CALSETMATH_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    RangeList op_set_union( const RangeList& left, const RangeList& right );
    RangeList op_set_intersection( const RangeList& left, const RangeList& right );
    RangeList op_set_rel_complement( const RangeList& left, const RangeList& right );
    RangeList op_set_sym_difference( const RangeList& left, const RangeList& right );
    RangeList op_set_complement( const RangeList& rlist );

    RangeList op_set_well_order( const RangeList& rlist );

}

#endif // CAL_CALSETMATH_H_GUARD