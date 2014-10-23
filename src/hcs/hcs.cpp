/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcs/hcs.cpp
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

#include <iostream>
#include <fstream>
#include <sstream>

#define VERSION   "0.8.0"
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

void do_usage()
{
    cout << g_title << g_copyright << "\n";
    cout << 
        "Usage:\n"
        "  hcs [options]\n"
        "\n"
        "Options:\n"
        "  -h    Show this help message and exit.\n"
        "  -n    Do not run the default script on start up.\n"
        "  -e    Exit the program without running the command line.\n"
        "  name  Run the file 'name' as a script.\n"
        "        Multiple files are run in the order that they appear.\n" 
        "\n"
        "Commandl line:\n"
        "  help           Show this help message.\n"
        "  run filename   Run the script in filename.\n"
        //"  info           Get info on available calendars.\n"
        "  bye            Exit the program.\n"
        "Anything else is passed directly to the script interpreter.\n"
        "\n"
    ;
}

int main( int argc, char* argv[] )
{
    vector<string> filenames;
    bool run_default = true;
    bool do_cmd_line = true;

    for( int i = 1 ; i < argc ; i++ ) {
        if( argv[i][0] == '-' ) {
            switch( argv[i][1] )
            {
            case 'h': // Help
                do_usage();
                return 0;
            case 'n': // No default script.
                run_default = false;
                break;
            case 'e': // Exit without running command line.
                do_cmd_line = false;
                break;
            default:
                cout << "Command line flag not recognised.\n";
            }
        } else {
            filenames.push_back( string( argv[i] ) );
        }
    }

    cout << g_title << g_copyright << "\n";

    Calendars* cal;
    if( run_default ) {
        cal = new Calendars( Init_script_default );
    } else {
        cal = new Calendars;
    }

    // Run script files if given.
    for( size_t i = 0 ; i < filenames.size() ; i++ ) {
        string script = read_file( filenames[i] );
        string response = cal->run_script( script );
        if( response.size() ) {
            cout << response << "\n";
        }
    }

    if( do_cmd_line ) {
        for(;;) {
            cout << "hcs: ";
            string cmnd;
            getline( cin, cmnd );
            string word, tail;
            word = get_first_word( cmnd, &tail, ' ' );

            if( word == "bye" ) {
                break;
            } else if( word == "info" ) {
                // TODO: Add info command.
                continue;
            } else if( word == "help" ) {
                do_usage();
                continue;
            } else if( word == "run" ) {
                cmnd = read_file( tail );
            }
            string output = cal->run_script( cmnd );
            if( output.size() ) {
                cout << output << "\n";
            }
        }
    }
    delete cal;
    return 0;
}

// End of src/hcs/hcs.cpp
