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
#include "calparse.h"
#include "calvocab.h"

using namespace std;
using namespace Cal;

// Get a user friendly format string for user selection
string Format::get_user_format( Grammar* gmr ) const
{
    string output, fname, vocab, abbrev;
    enum State { dooutput, doprolog, dofname, dovocab, doabbrev };
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
                state = dofname;
            } else {
                output += *it;
            }
            break;
        case dofname:
            if( *it == ')' ) {
                if( gmr ) {
                    output += gmr->get_num_code_alias( fname );
                } else {
                    output += fname;
                }
                fname.clear();
                state = dooutput;
            } else if( *it == ':' ) {
                state = dovocab;
            } else {
                fname += *it;
            }
            break;
        case dovocab:
        case doabbrev:
            if( *it == '.' ) {
                state = doabbrev;
            } else if( *it == ')' ) {
                if( gmr ) {
                    Vocab* v = gmr->find_vocab( vocab );
                    if( v ) {
                        if( abbrev == "a" ) {
                            fname = v->get_style_name( Vocab::style_abbrev );
                        } else {
                            fname = v->get_style_name( Vocab::style_full );
                        }
                    }
                }
                output += fname;
                vocab.clear();
                abbrev.clear();
                fname.clear();
                state = dooutput;
            } else {
                if( state == dovocab ) {
                    vocab += *it;
                } else {
                    abbrev += *it;
                }
            }
            break;
        }
    }
    return output;
}

string Format::get_order_str() const
{
    string fname, output;
    enum State { dooutput, doprolog, dofname, dovocab };
    State state = dooutput;
    for( string::const_iterator it = m_format.begin() ; it != m_format.end() ; it++ ) {
        switch( state )
        {
        case dooutput:
            if( *it == '@' ) {
                state = doprolog;
            }
            break;
        case doprolog:
            if( *it == '(' ) {
                state = dofname;
            }
            break;
        case dofname:
            if( *it == ')' || *it == ':' || *it == '/' ) {
                if( output.size() ) {
                    output += " ";
                }
                output += fname;
                fname.clear();
                state = dooutput;
            } else {
                fname += *it;
            }
            break;
        }
    }
    return output;
}


// End of src/cal/calformat.cpp file
