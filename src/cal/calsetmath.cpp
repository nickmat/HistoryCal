/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calsetmath.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Functions to carry out set operations.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th May 2014
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

#include "calsetmath.h"

#include <cassert>

using namespace std;
using namespace Cal;

namespace {

    enum SetOp {
        SO_union,      // Union                 L | R
        SO_inter,      // Intersection          L & R
        SO_rel_comp,   // Relative complement   L \ R
        SO_sim_dif     // Symmetric difference  L ^ R
    };

    RangeList op_set( SetOp op, const RangeList& left, const RangeList& right )
    {
        bool lon = false, ron = false;
        Field lpos = f_maximum, rpos = f_maximum, apos = f_minimum;
        size_t li = 0, ri = 0;
        bool aon = false, paon = false;

        if( left.size() ) {
            lpos = f_minimum;
            lon = (lpos == left[0].jdn1);
        }
        if( right.size() ) {
            rpos = f_minimum;
            ron = (rpos == right[0].jdn1);
        }
        
        RangeList answer;
        Range range;
        while( apos != f_end ) {
            apos = min( lpos, rpos ); // Next position.
            // We want to know if the left, right ranges are on or off
            // at the current answer position. If the left or right position
            // is the same as the answer postion, we can just use the on/off flag.
            // Otherwise, we want the previous setting, which is the opposite
            // of the current on/off setting.
            bool lval = (apos == lpos) ? lon : !lon;
            bool rval = (apos == rpos) ? ron : !ron;
            switch( op )
            {
            case SO_union:
                aon = (lval || rval); 
                break;
            case SO_inter:
                aon = (lval && rval); 
                break;
            case SO_rel_comp:
                aon = (lval && !rval); 
                break;
            case SO_sim_dif:
                aon = (lval != rval); 
                break;
            default:
                assert( false ); // How did we get here?
            }
            if( aon != paon ) {         // we need make an answer entry
                if( aon ) {            // start of a new range
                    range.jdn1 = apos;
                } else {               // completes a range
                    range.jdn2 = apos - 1;
                    answer.push_back( range );
                }
                paon = aon;
            }
            // move on left
            if( lpos == apos && lpos != f_invalid2 ) {
                if( li < left.size() ) {
                    if( lon ) {
                        lpos = left[li].jdn2;
                        assert( lpos != f_end );
                        lpos++;
                        li++;
                    } else { // lon == false
                        lpos = left[li].jdn1;
                    }
                } else {
                    lpos = f_invalid2;
                }
                lon = !lon;
            }
            // move on right
            if( rpos == apos && rpos != f_invalid2 ) {
                if( ri < right.size() ) {
                    if( ron ) {
                        rpos = right[ri].jdn2;
                        assert( rpos != f_end );
                        rpos++;
                        ri++;
                    } else { // ron == false
                        rpos = right[ri].jdn1;
                    }
                } else {
                    rpos = f_invalid2;
                }
                ron = !ron;
            }
        }
        return answer;
    }

}

RangeList Cal::op_set_union( const RangeList& left, const RangeList& right )
{
    return op_set( SO_union, left, right );
}

RangeList Cal::op_set_intersection( const RangeList& left, const RangeList& right )
{
    return op_set( SO_inter, left, right );
}

RangeList Cal::op_set_rel_complement( const RangeList& left, const RangeList& right )
{
    return op_set( SO_rel_comp, left, right );
}

RangeList Cal::op_set_sim_difference( const RangeList& left, const RangeList& right )
{
    return op_set( SO_sim_dif, left, right );
}

RangeList Cal::op_set_complement( const RangeList& input )
{
    RangeList answer;
    Range range;

    // Check for empty set.
    if( input.empty() ) {
        range.jdn1 = f_minimum;
        range.jdn2 = f_maximum;
        answer.push_back( range );
        return answer; // Return the full set.
    }

    int i = 0;
    if( input[0].jdn1 != f_minimum ) {
        range.jdn1 = f_minimum;
        range.jdn2 = input[0].jdn1 - 1;
        answer.push_back( range );
    }
    while( i < (int) input.size() - 1 ) {
        range.jdn1 = input[i].jdn2 + 1;
        i++;
        range.jdn2 = input[i].jdn1 - 1;
        answer.push_back( range );
    }
    if( input[i].jdn2 != f_maximum ) {
        range.jdn1 = input[i].jdn2 + 1;
        range.jdn2 = f_maximum;
        answer.push_back( range );
    }
    return answer;
}

// End of src/cal/calsetmath.cpp file
