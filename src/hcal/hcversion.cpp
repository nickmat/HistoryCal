/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcversion.cpp
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     Version constants and functions.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th September 2013
 * Copyright:   Copyright (c) 2013..2025, Nick Matthews.
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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "hcversion.h"

#define VERSION_STATUS   " alpha"

#if defined(NDEBUG) || !defined(_DEBUG)
#define VERSION_CONFIG   ""
#else
#define VERSION_CONFIG   " debug"
#endif

#define RELEASE_NUM   " R1"
#define VERSION_NUM   "V0.1.0" RELEASE_NUM


#define VERSION   VERSION_NUM VERSION_STATUS VERSION_CONFIG " " __DATE__

/*! A string containing the current full version number.
 */
const char* hcVersion = VERSION;

/*! A string containing a long hand version name and copyright message.
 */
const char* hcTitle = "HistoryCal - " VERSION "\n"
                      "Copyright (c) 2013..2025  Nick Matthews\n\n";

/*************************************************************************//**

 \file

 Source code revision history

 19sep13  v0.0.0   Work started on pCalCalc and Cal library, adapted from 
                   The Family Pack

 25nov14  v0.0.1   Create initial alpha version. Renamed HistoryCal.
                   Initial alpha release.

  6dec14  v0.0.2   Uses Cal library version 0.2.3. Correct typos.

 16mar15  v0.0.3   Correct handling of utf8 strings.
                   Improve inserting text from the text vocabulary selector.
                   Use Cal library version 0.2.4.

 24may15  v0.0.4   Use Cal library version 0.2.5.

 18nov15  v0.0.5   Use Cal library version 0.2.6.

 26jun16  v0.0.6   Use Cal library version 0.2.7. See docs/hcal-changes.txt

cancelled v0.0.7   Use Cal library version 0.2.7.
                   Add GUI for formating.

  active  v0.1.0 R1  Replace Cal library with Glich library.

 See also website http://historycal.org

***************************************************************************/

/*! Returns a string representing the compiler name and version.
 *  Works for Visual C++ and GCC so far.
 */
wxString hcGetCompilerVersion()
{
#if defined(__VISUALC__)
    return wxString::Format( _("Visual C++ version %d"), __VISUALC__ );
#elif defined(__GNUC__)
    return wxString::Format( 
        _("GCC compiler version %d.%d.%d"),
        __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__
    );
#else
    return _("Unknown compiler");
#endif
}

// End of src/hcal/hcversion.cpp file
