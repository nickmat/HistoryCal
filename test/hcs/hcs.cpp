/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/hcs/hcs.cpp
 * Project:     HistoryCalScript: Command line HistoryCal Calculator.
 * Purpose:     Command line test program for Cal Calendar library.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     18th May 2014
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

#include <cal/calendars.h>
#include <utf8/utf8api.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#define VERSION   "0.7.0"
#define PROGNAME  "HistoryCalScript"
#define COPYRIGHT  "2014 Nick Matthews"

const char* g_version = VERSION;
const char* g_progName = PROGNAME;
const char* g_copyright = "Copyright (c) " COPYRIGHT "\n";

#if defined(NDEBUG) || !defined(_DEBUG)
const char* g_title = PROGNAME " - Version " VERSION "\n";
#else
const char* g_title = PROGNAME " - Version " VERSION " Debug\n";
#endif

using namespace Cal;
using namespace std;

string left_trim( const string& str )
{
    size_t pos = 0;
    while( str.find( ' ', pos ) == pos ) {
        pos++;
    }
    if( pos != string::npos ) {
        return str.substr( pos );
    }
    return "";
}

string get_first_word( const string& str, string* tail, char sep )
{
    size_t pos = str.find( sep );
    string result = str.substr( 0, pos );
    if( tail ) {
        *tail = ( pos == string::npos ) ? "" : left_trim( str.substr( pos + 1 ) );
    }
    return result;
}

string read_file( const string& name )
{
    ifstream in( name.c_str() );
    stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

int main()
{
    Calendars cal( Init_script_default );

    cout << g_title << "\n";

    for(;;) {
        cout << "hcc: ";
        string cmnd;
        getline( cin, cmnd );
        string word, tail;
        word = get_first_word( cmnd, &tail, ' ' );

        if( word == "exit" || word == "bye" || word == "quit" ) {
            break;
        } else if( word == "info" ) {
            continue;
        } else if( word == "run" ) {
            cmnd = read_file( tail );
        }
        string output = cal.run_script( cmnd );
        if( output.size() ) {
            cout << output << "\n";
        }
    }
    return 0;
}

// End of test/hcs/hcs.cpp
