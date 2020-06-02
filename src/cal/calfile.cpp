/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calfile.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     File class to handle script files.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     1st June 2020
 * Copyright:   Copyright (c) 2020, Nick Matthews.
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

#include "calfile.h"

using namespace Cal;
using std::string;

bool File::open()
{
    if ( m_file.is_open() ) {
        return false;
    }
    std::fstream::openmode mode = std::fstream::out;
    switch ( m_filetype )
    {
    case FT_write:
        mode |= std::fstream::trunc;
        break;
    case FT_append:
        mode |= std::fstream::app;
        break;
    default:
        return false;
    }
    m_file.open( m_filename, mode );
    if ( !m_file.is_open() ) {
        return false;
    }
    return true;
}

// End of src/cal/calfile.cpp file
