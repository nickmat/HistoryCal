/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcformatdlg.cpp
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     hcFormatDlg class implementation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     12th August 2016
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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "hcformatdlg.h"

#include "hcconvert.h"

using namespace Cal;
using std::string;

hcFormatDlg::hcFormatDlg(
    wxWindow* parent, Calendars* cal, SHandle sch,
    const string& fcode, const string& init )
    : m_cal(cal), m_scheme(sch), m_fcode(fcode), m_based_on(init),
    fbFormatDlg( parent )
{
}

bool hcFormatDlg::TransferDataToWindow()
{
    Scheme_info sinfo;
    m_cal->get_scheme_info( &sinfo, m_scheme );
    m_gcode = sinfo.grammar_code;
    string sch_name = sinfo.code + "#  " + sinfo.name;
    m_textCtrlSCode->SetValue( Utf8ToWxStr( sch_name ) );
    m_textCtrlFCode->SetValue( Utf8ToWxStr( m_fcode ) );

    if( !m_based_on.empty() ) {
        FormatText_info finfo;
        m_cal->get_format_text_info( &finfo, m_scheme, m_based_on );
        wxASSERT( finfo.info.type == FT_text );
        m_textCtrlControl->SetValue( Utf8ToWxStr( finfo.control_str ) );
        m_textCtrlInputStr->SetValue( Utf8ToWxStr( finfo.info.input_str ) );
        m_textCtrlOutputStr->SetValue( Utf8ToWxStr( finfo.info.output_str ) );
    }
    return true;
}

bool hcFormatDlg::TransferDataFromWindow()
{
    string control = WxStrToUtf8( m_textCtrlControl->GetValue() );
    string use_as = m_checkBoxUseInput->GetValue() ? "inout" : "output";
    if( !control.empty() ) {
        m_script =
            "format \"" + m_gcode + ":" + m_fcode + "\" {\n " +
            use_as + " \"" + control + "\";\n"
            "}\n"
        ;
    }
    return true;
}

void hcFormatDlg::OnIdle( wxIdleEvent& event )
{
    string current( WxStrToUtf8( m_textCtrlControl->GetValue() ) );
    if( current != m_control ) {
        string script =
            "mark \"####\";\n"
            "format \"" + m_gcode + ":" + m_fcode + "\", \"" + current + "\";\n"
        ;
        m_cal->run_script( script );
        FormatText_info finfo;
        m_cal->get_format_text_info( &finfo, m_scheme, m_fcode );
        m_cal->run_script( "mark \"####\";" );
        wxASSERT( finfo.info.type == FT_text );

        m_textCtrlInputStr->SetValue( Utf8ToWxStr( finfo.info.input_str ) );
        m_textCtrlOutputStr->SetValue( Utf8ToWxStr( finfo.info.output_str ) );
        m_control = current;
    }
}

// End of src/hcal/hcformatdlg.cpp source file.

