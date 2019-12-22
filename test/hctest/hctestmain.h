/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/hctest/hctestmain.h
 * Project:     HistoryCalTest: Command line HistoryCal Test Script Program.
 * Purpose:     Common header for HistoryCalTest.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th June 2014
 * Copyright:   Copyright (c) 2014 ~ 2019, Nick Matthews.
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

#ifndef HCS_HCTESTMAIN_H_GUARD
#define HCS_HCTESTMAIN_H_GUARD

#include <string>
#include <vector>


enum CheckFile { CF_none, CF_file, CF_dir };
CheckFile check_file( const std::string& name );

void get_filenames( std::vector<std::string>& vec, const std::string& path );

#endif // HCS_HCTESTMAIN_H_GUARD