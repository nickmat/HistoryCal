/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/hcs/hcsmain.h
 * Project:     HistoryCalScript: Command line HistoryCal Calculator.
 * Purpose:     Common header for HistoryCalScript.
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

#ifndef HCS_HCSMAIN_H_GUARD
#define HCS_HCSMAIN_H_GUARD

#include <string>
#include <vector>

void get_filenames( std::vector<std::string>& vec, const std::string& path );

#endif // HCS_HCSMAIN_H_GUARD