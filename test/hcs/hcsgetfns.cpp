/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/hcs/hcsgetfns.cpp
 * Project:     HistoryCalScript: Command line HistoryCal Calculator.
 * Purpose:     Read directory file listing.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th June 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  HistoryCalScript is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  HistoryCalScript is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with HistoryCalScript.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#include "hcsmain.h"

#include <win/dirent.h>
//#include <dirent.h>

#include <iostream>

using std::vector;
using std::string;

void get_filenames( vector<string>& vec, const string& path )
{
    DIR* dir = opendir( path.c_str() );
    dirent* pdir = readdir( dir );
    while( pdir ) {
        if( pdir->d_name[0] == '.' ) { // Ignore hidden files/directories
            pdir = readdir( dir );
            continue;
        }
        string fname( pdir->d_name );
        if( pdir->d_type & DT_DIR ) {
            get_filenames( vec, path + "/" + fname );
        } else {
            size_t len = pdir->d_namlen;
            if( len > 4 && fname.substr( len - 4, 4 ) == ".hcs" ) {
                vec.push_back( path + "/" + fname );
            }
        }
        pdir = readdir( dir );
    }
    closedir( dir );
}

// End of test/hcs/hcsreaddir.cpp
