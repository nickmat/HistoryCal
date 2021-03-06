/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/hctest/hctest.cpp
 * Project:     HistoryCalTest: Command line HistoryCal Test Script Program.
 * Purpose:     Command line test program for Cal Calendar library.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     18th May 2014
 * Copyright:   Copyright (c) 2014 .. 2021, Nick Matthews.
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

#define VERSION   "0.10.2"
#define PROGNAME  "HistoryCalTest"
#define COPYRIGHT  "2014..2021 Nick Matthews"

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

struct TestResults {
    TestResults() 
        : tests( 0 ), fails( 0 ), skips( 0 ), show_skips( false ) {}
    int tests;
    int fails;
    int skips;
    bool show_skips;
};

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

string run_test( TestResults* totals, Calendars* cal, const string& filename )
{
    string script = read_file( filename );
    size_t pos1 = script.find( "\n/*[SKIP]*/" );
    if ( pos1 != string::npos ) {
        totals->skips++;
        if ( totals->show_skips ) {
            cout << "S";
            return filename + " skipped.";
        }
        return "";
    }
    cal->run_script( "mark __:test:__; clear;" ); // All test should start clean;
    string output = cal->run_script( script );
    totals->tests++;
    string expected;
    pos1 = script.find( "/*[OUTPUT]\n" );
    if( pos1 != string::npos ) {
        pos1 += 11;
        size_t pos2 = script.find( "\n[OUTPUT]*/", pos1 );
        if( pos2 != string::npos ) {
            expected = script.substr( pos1, pos2 - pos1 );
        }
    }
    string error;
    if( output.empty() ) {
        error = "No output";
    } else if( output != expected ) {
        error = "  Expected: " + expected + "\n"
            + "  Output: " + output;
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

string run_full_test( TestResults* totals, Calendars* cal, const string& path )
{
    vector<string> filenames;
    string result;
    get_filenames( filenames, path );
    for( size_t i = 0 ; i < filenames.size() ; i++ ) {
        string error = run_test( totals, cal, filenames[i] );
        if( !error.empty() ) {
            totals->fails++;
            result += "\n\n" + error;
        }
    }
    return result;
}

string run_test_script( TestResults* totals, Calendars* cal, const string& filename )
{
    string script = read_file( filename );
    string output = cal->run_script( script );
    string expected;
    size_t pos1 = script.find( "/*[OUTPUT]\n" );
    if( pos1 != string::npos ) {
        pos1 += 11;
        size_t pos2 = script.find( "\n[OUTPUT]*/", pos1 );
        if( pos2 != string::npos ) {
            expected = script.substr( pos1, pos2 - pos1 );
        }
    }
    string result = filename + "  ";
    if ( output.empty() ) {
        result += "No output\n";
        totals->fails++;
    } else if ( output == expected ) {
        result += "Pass: " + output + "\n\n";
    } else {
        result += "\n Expected: " + expected + "\n"
            + "   Output: " + output + "\n\n";
        totals->fails++;
    }
    totals->tests++;
    return result;
}

int main( int argc, char* argv[] )
{
    cout << g_title << "\n";

    clock_t t = clock();
    Calendars* cal_stdlib = nullptr;
    Calendars* cal_none = nullptr;
    Calendars* cal = nullptr;
    string result;
    TestResults totals;
    for ( int i = 1; i < argc; i++ ) {
        string arg = argv[i];
        if ( arg == "-t" ) {
            continue; // We used to require this, now ignore it.
        }
        if ( arg == "-l" ) { // Lowercase L
            if ( !cal_stdlib ) {
                cal_stdlib = new Calendars( Init_script_default );
            }
            cal = cal_stdlib;
            continue;
        }
        if ( arg == "-n" ) {
            if ( !cal_none ) {
                cal_none = new Calendars( Init_script_none );
            }
            cal = cal_none;
            continue;
        }
        if ( arg == "-s" ) {
            totals.show_skips = true;
            continue;
        }
        CheckFile cf = check_file( arg );
        if ( !cal && cf != CheckFile::none ) {
            cal = cal_stdlib = new Calendars( Init_script_default );
        }
        if ( cf == CheckFile::file ) {
            result += run_test_script( &totals, cal, arg );
            continue;
        } else if ( cf == CheckFile::dir ) {
            result += run_full_test( &totals, cal, arg );
            continue;
        }
        result += "Unknown command line switch " + arg + "\n\n";
    }
    cout << result;

    double dt = ((double) clock() - t) / CLOCKS_PER_SEC;

    cout << "\n\nRun (" + to_string( totals.tests ) + ") "
        "  fail (" + to_string( totals.fails ) + ")"
        "  skip (" + to_string( totals.skips ) + ")"
        "  Timed: " + to_string( dt ) + "s\n\n";

    return 0;
}

// End of test/hctest/hctest.cpp
