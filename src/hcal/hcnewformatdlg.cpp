/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcnewformatdlg.cpp
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     hcNewFormatDlg class implementation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     6th August 2016
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

#include "hcnewformatdlg.h"

#include "hcconvert.h"

using namespace Cal;
using std::string;

hcNewFormatDlg::hcNewFormatDlg(
    wxWindow* parent, Calendars* cal,
    const string& in_scode, const string& out_scode )
    : m_cal(cal), m_scheme(NULL), m_iso(false), fbNewFormatDlg( parent )
{
    m_in_scheme = cal->get_scheme( in_scode );
    m_out_scheme = cal->get_scheme( out_scode );

    cal->get_scheme_info( &m_in_sinfo, m_in_scheme );
    cal->get_scheme_info( &m_out_sinfo, m_out_scheme );
}

void hcNewFormatDlg::OnSelectScheme( wxCommandEvent& event )
{
    UpdateFormatList();
}

bool hcNewFormatDlg::TransferDataToWindow()
{
    m_radioBtnInSch->SetLabel( Utf8ToWxStr(m_in_sinfo.code+"#  "+m_in_sinfo.name) );
    m_radioBtnOutSch->SetLabel( Utf8ToWxStr(m_out_sinfo.code+"#  "+m_out_sinfo.name) );
    m_radioBtnOutSch->SetValue( true );
    m_schemes = m_cal->get_scheme_list();
    for( size_t i = 0 ; i < m_schemes.size() ; i++ ) {
        wxString entry = Utf8ToWxStr(m_schemes[i].code+"#  "+m_schemes[i].name);
        m_comboBoxSchemes->Append( entry );
        if( m_schemes[i].code == "g" ) {
            m_comboBoxSchemes->SetSelection( i );
        }
    }
    m_checkBoxIso->SetValue( m_iso );
    UpdateFormatList();
    return true;
}

bool hcNewFormatDlg::TransferDataFromWindow()
{
    if( m_radioBtnInSch->GetValue() ) {
        m_scheme = m_in_scheme;
    } else if( m_radioBtnOutSch->GetValue() ) {
        m_scheme = m_out_scheme;
    } else {
        int i = m_comboBoxSchemes->GetSelection();
        if( i >= 0 ) {
            m_scheme = m_schemes[i].handle;
        }
    }
    m_fcode = m_textCtrlFCode->GetValue();
    m_iso = m_checkBoxIso->GetValue();

    int sel = m_comboBoxFormats->GetSelection();
    if( sel > 0 ) {
        Scheme_info sinfo;
        m_cal->get_scheme_info( &sinfo, m_scheme );
        SchemeFormatInfo finfo;
        m_cal->get_output_info( &finfo, m_scheme );
        m_based_fcode = Utf8ToWxStr( finfo.descs[sel-1].codes[0].code );
    }
    return true;
}

void hcNewFormatDlg::UpdateFormatList()
{
    SHandle scheme = NULL;
    if( m_radioBtnOutSch->GetValue() ) {
        scheme = m_in_scheme;
    } else if( m_radioBtnInSch->GetValue() ) {
        scheme = m_out_scheme;
    } else {
        int i = m_comboBoxSchemes->GetSelection();
        if( i >= 0 ) {
            scheme = m_schemes[i].handle;
        }
    }

    Scheme_info sinfo;
    m_cal->get_scheme_info( &sinfo, scheme );
    SchemeFormatInfo finfo;
    m_cal->get_output_info( &finfo, scheme );
    m_comboBoxFormats->Clear();
    m_comboBoxFormats->Append( "< NONE >" );
    for( size_t i = 0 ; i < finfo.descs.size() ; i++ ) {
        wxString fmt = Utf8ToWxStr( 
            sinfo.code  + ":" + finfo.descs[i].codes[0].code + "#  " +
            finfo.descs[i].desc
        );
        m_comboBoxFormats->Append( fmt );
    }
    m_comboBoxFormats->SetSelection( 0 );
}

// End of src/hcal/hcnewformatdlg.cpp source file.

