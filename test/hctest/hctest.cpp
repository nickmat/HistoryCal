/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/hctest/hctest.cpp
 * Project:     HistoryCalTest: Command line HistoryCal Test Script Program.
 * Purpose:     Command line test program for Cal Calendar library.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     18th May 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  HistoryCalTest is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  HistoryCalTest is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with HistoryCalTest.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#include "hctestmain.h"

#include <cal/calendars.h>
#include <utf8/utf8api.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>

#define VERSION   "0.8.0"
#define PROGNAME  "HistoryCalTest"
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
    std::ifstream in( name.c_str() );
    return std::string( std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() );
}

string run_test( Calendars* cal, const string& filename )
{
    string script = read_file( filename );
    string output = cal->run_script( script );
    string expected;
    size_t pos1 = script.find( "/*[OUTPUT]\n" );
    if( pos1 != string::npos ) {
        pos1 += 11;
        size_t pos2 = script.find( "[OUTPUT]*/", pos1 );
        if( pos2 != string::npos ) {
            expected = script.substr( pos1, pos2 - pos1 );
        }
    }
    string error;
    if( output.empty() ) {
        error = "No output";
    } else if( output != expected ) {
        error = "  Expected: " + expected + "\n"
            + "    Output: " + output;
    }
    string result;
    if( !error.empty() ) {
        result = filename + "\n" + error;
        cout << "F";
    } else {
        cout << ".";
    }
    return result;
}

void run_full_test( Calendars* cal, const string& path )
{
    clock_t t = clock();
    vector<string> filenames;
    vector<string> errors;
    get_filenames( filenames, path );
    for( size_t i = 0 ; i < filenames.size() ; i++ ) {
        string error = run_test( cal, filenames[i] );
        if( !error.empty() ) {
            errors.push_back( error );
        }
    }
    cout << "\n\nRun (" << filenames.size() << ") ";
    if( !errors.empty() ) {
        cout << " fail (" << errors.size() << ")";
        for( size_t i = 0 ; i < errors.size() ; i++ ) {
            cout << "\n\n" << errors[i];
        }
    }
    cout << "\n\n";
    int s = (clock() - t) / CLOCKS_PER_SEC;
    int m = (int) s / 60;
    cout << "Timed: " << m << "m " << s - (m*60) << "s\n";
}

int main( int argc, char* argv[] )
{
    Calendars cal( Init_script_default );
    cout << g_title << "\n";

    string argv1;
    switch( argc )
    {
    case 1:
        break;
    case 2:
        argv1 = string(argv[1]);
        run_test( &cal, argv1 );
        return 0;
    case 3:
        argv1 = string(argv[1]);
        run_full_test( &cal, string(argv[2]) );
        return 0;
    default:
        cout << "Too many arguments.\n";
        return 1;
    }

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
        } else if( word == "test" ) {
            run_test( &cal, tail );
            continue;
        }
        string output = cal.run_script( cmnd );
        if( output.size() ) {
            cout << output << "\n";
        }
    }
    return 0;
}

// End of test/hctest/hctest.cpp
