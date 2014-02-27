/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calformat.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Format class to control the formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th November 2013
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

#include "calformat.h"

#include "calgrammar.h"
#include "calvocab.h"

using namespace std;
using namespace Cal;

// Get a user friendly format string for user selection
string Format::get_user_format( Grammar* gmr ) const
{
    string output, fname, vocab;
    enum State { dooutput, doprolog, dopriority, dofname, dovocab };
    State state = dooutput;
    for( string::const_iterator it = m_format.begin() ; it != m_format.end() ; it++ ) {
        switch( state )
        {
        case dooutput:
            if( *it == '@' ) {
                state = doprolog;
            } else {
                output += *it;
            }
            break;
        case doprolog:
            if( *it == '(' ) {
                state = dopriority;
            } else {
                output += *it;
            }
            break;
        case dopriority:
            if( *it == ':' ) {
                state = dofname;
            }
            break;
        case dofname:
            if( *it == ')' ) {
                output += fname;
                fname.clear();
                state = dooutput;
            } else if( *it == ':' ) {
                state = dovocab;
            } else {
                fname += *it;
            }
            break;
        case dovocab:
            if( *it == ')' ) {
                if( gmr ) {
                    Vocab* v = gmr->find_vocab( vocab );
                    if( v ) {
                        fname = v->get_field();
                    }
                }
                output += fname;
                vocab.clear();
                fname.clear();
                state = dooutput;
            } else {
                vocab += *it;
            }
            break;
        }
    }
    return output;
}


// End of src/cal/calformat.cpp file
