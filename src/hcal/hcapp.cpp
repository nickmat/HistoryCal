/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcapp.cpp
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     Program App class.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th September 2013
 * Copyright:   Copyright (c) 2013-2023, Nick Matthews.
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

#include "hcconvert.h"
#include "hcframe.h"

#include <glc/glc.h>


IMPLEMENT_APP(HcApp)

class HcInOut : public glich::InOut
{
public:
    std::string get_input( const std::string& prompt ) override;
};

std::string HcInOut::get_input( const std::string& prompt )
{
    wxTextEntryDialog dialog(
        nullptr, Utf8ToWxStr( prompt ),
        _( "Glich Script Input" ), "", wxOK | wxCANCEL
    );
    if( dialog.ShowModal() == wxID_OK ) {
        return WxStrToUtf8( dialog.GetValue() );
    }
    return std::string();
}


bool HcApp::OnInit()
{
    glich::init_glc( glich::InitLibrary::Hics, new HcInOut );

    long style = wxMINIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU |
        wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxTAB_TRAVERSAL;
    auto frame = new HcFrame( "HistoryCal", wxDefaultPosition, wxSize( -1, -1 ), style );
    frame->Show();
    return true;
}

int HcApp::OnExit()
{
    glich::exit_glc();
    return 0;
}


// End of src/hcal/hcapp.cpp file
