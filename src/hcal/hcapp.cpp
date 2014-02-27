/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcapp.cpp
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     Program App class.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th September 2013
 * Copyright:   Copyright (c) 2013-2014, Nick Matthews.
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

#include "hcapp.h"

#include "hcframe.h"


IMPLEMENT_APP(HcApp)

bool HcApp::OnInit()
{
    long style = wxMINIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU |
        wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxTAB_TRAVERSAL;
    frame = new HcFrame( "HistoryCal", wxDefaultPosition, wxSize( -1, -1 ), style );
    frame->Show();
    return true;
}

// End of src/hcal/hcapp.cpp file
