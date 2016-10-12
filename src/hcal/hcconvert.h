/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcconvert.h
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     Conversion between wxWidgets and C++ std library.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     12th October 2016
 * Copyright:   Copyright (c) 2016, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  HistoryCal is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  HistoryCal is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with HistoryCal.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#ifndef SRC_HCAL_HCCONVERSION_H_GUARD
#define SRC_HCAL_HCCONVERSION_H_GUARD

inline wxString Utf8ToWxStr( const std::string& stdstr )
{
    return wxString( stdstr.c_str(), wxConvUTF8 );
}

inline std::string WxStrToUtf8( const wxString& wxstr )
{
    return std::string( wxstr.mb_str( wxConvUTF8 ) );
}

#endif // SRC_HCAL_HCCONVERSION_H_GUARD
