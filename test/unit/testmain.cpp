/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/test/testmain.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     CppUnit test program for the Cal library
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th October 2013
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

#include <cal/calendars.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextTestProgressListener.h>


int main( int argc, char* argv[] )
{
    std::string testPath = (argc > 1) ? std::string( argv[1] ) : "";

    // Create the event manager and test controller
    CppUnit::TestResult controller;

    // Add a listener that colllects test result
    CppUnit::TestResultCollector result;
    controller.addListener( &result );

    // Add a listener that print dots as test run.
    CppUnit::TextTestProgressListener progress;
    controller.addListener( &progress );

    // Add the top suite to the test runner
    CppUnit::TestRunner runner;
    runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );
    try {
        std::cout << "Running " <<  testPath;
        runner.run( controller, testPath );

        std::cerr << std::endl;

        // Print test in a compiler compatible format.
        CppUnit::CompilerOutputter outputter( &result, std::cerr );
        outputter.write();
    }
    catch( std::invalid_argument &e ) { // Test path not resolved
        std::cerr << std::endl << "ERROR: " << e.what() << std::endl;
        return 0;
    }

    return result.wasSuccessful() ? 0 : 1;
}

// End of src/test/testmain.h file
