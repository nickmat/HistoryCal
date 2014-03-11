/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcframe.cpp
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     HcFrame class.
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

#include "hcframe.h"

#include "hcversion.h"

#include <utf8/utf8api.h>

using namespace std;
using namespace Cal;

#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "hcal.xpm"
#endif

/*! \brief Frame constructor.
 *
 *  Create the window Icon.
 */
HcFrame::HcFrame(
    const wxString& title, const wxPoint& pos, const wxSize& size, long style )
    : m_cal(Init_schemes_default), m_from(2), m_to(2), m_show_interm(false),
    m_show_count(false),
    hcFbFrame( (wxFrame*) NULL, wxID_ANY, title, pos, size, style )
{
    // Set frames Icon
    SetIcon( wxICON( hcal ) );

    Scheme_info info;
    wxString entry;
    for( int i = 0 ; i < m_cal.get_scheme_count() ; i++ ) {
        m_cal.get_scheme_info( &info, i );
        entry.clear();
        entry << info.name.c_str() << "  (" << info.code.c_str() << ")";
        m_comboBoxInput->Append( entry );
        m_comboBoxOutput->Append( entry );
    }
    m_comboBoxInput->SetSelection( m_from );
    UpdateInputFormat();
    UpdateTextVocabs();
    m_comboBoxOutput->SetSelection( m_to );
    UpdateOutputFormat();

    bSizerIntermeadiate->Show( false );

    m_textInput->SetFocus();
}

/*! \brief Called on an Exit Application menu option event.
 */
void HcFrame::OnExit( wxCommandEvent& event )
{
    // true is to force the frame to close
    Close( true );
}

void HcFrame::OnToggleInterm( wxCommandEvent& event )
{
    if( m_show_interm ) {
        bSizerIntermeadiate->Show( false );
        Layout();
        m_show_interm = false;
    } else {
        bSizerIntermeadiate->Show( true );
        Layout();
        m_show_interm = true;
    }
}

void HcFrame::OnToggleCount( wxCommandEvent& event )
{
    if( m_show_count ) {
        m_show_count = false;
    } else {
        m_show_count = true;
    }
    CalculateOutput();
}

/*! \brief Called on a Help, TFP Website menu option event.
 *
 */
void HcFrame::OnWebsite( wxCommandEvent& event )
{
    wxLaunchDefaultBrowser( "http://historycal.org" );
}

/*! \brief Called on a Help, About menu option event.
 *
 *  Displays a message box with the current hacl version number,
 *  wxWidgets version number, the compiler name and version and
 *  operating system description.
 *
 */
void HcFrame::OnAbout( wxCommandEvent& event )
{
    wxMessageBox(
        wxString::Format(
            _("%s"
            "Built with %s\n"
            "and calendar library %s\n"
            "with utf8proc library %s\n"
            "by %s\n"
            "running under %s."),
            hcTitle,
            wxVERSION_STRING,
            m_cal.version(),
            Utf8api::version(),
            hcGetCompilerVersion(),
            wxGetOsDescription().c_str()
        ),
        _("About HistoryCal"),
        wxOK | wxICON_INFORMATION,
        this
    );
}

void HcFrame::OnSelectInput( wxCommandEvent& event )
{
    m_from = m_comboBoxInput->GetSelection();
    UpdateInputFormat();
    UpdateTextVocabs();
    CalculateOutput();
}

void HcFrame::OnSelectInputFormat( wxCommandEvent& event )
{
    CalculateOutput();
}

void HcFrame::OnSelectVocab( wxCommandEvent& event )
{
    Scheme_info info;
    m_cal.get_scheme_info( &info, m_from );
    UpdateTextTokens( &info );
}

void HcFrame::OnSelectToken( wxCommandEvent& event )
{
    (*m_textInput) << m_comboBoxToken->GetValue();
}

void HcFrame::OnCheckTextFull( wxCommandEvent& event )
{
    Scheme_info info;
    m_cal.get_scheme_info( &info, m_from );
    UpdateTextTokens( &info );
}

void HcFrame::OnInputEnter( wxCommandEvent& event )
{
    CalculateOutput();
}

void HcFrame::OnButtonConvert( wxCommandEvent& event )
{
    CalculateOutput();
}

void HcFrame::OnSelectOutput( wxCommandEvent& event )
{
    m_to = m_comboBoxOutput->GetSelection();
    UpdateOutputFormat();
    CalculateOutput();
}

void HcFrame::OnSelectOutputFormat( wxCommandEvent& event )
{
    CalculateOutput();
}

void HcFrame::UpdateInputFormat()
{
    m_comboBoxInFormat->Clear();
    Scheme_input input;
    m_cal.get_scheme_input( &input, m_from );
    if( input.orders.size() ) {
        for( size_t i = 0 ; i < input.orders.size() ; i++ ) {
            m_comboBoxInFormat->Append( input.orders[i] );
        }
        m_comboBoxInFormat->SetSelection( input.current );
    }
}

void HcFrame::UpdateTextVocabs()
{
    m_comboBoxVocab->Clear();
    Scheme_info info;
    m_cal.get_scheme_info( &info, m_from );
    for( size_t i = 0 ; i < info.vocab_names.size() ; i++ ) {
        m_comboBoxVocab->Append( info.vocab_names[i] );
    }
    if( info.vocab_names.empty() ) {
        m_comboBoxVocab->Append( "<None>" );
    }
    m_comboBoxVocab->SetSelection( 0 );
    UpdateTextTokens( &info );
}

void HcFrame::UpdateTextTokens( Scheme_info* sinfo )
{
    m_comboBoxToken->Clear();
    Vocab_info vinfo;
    if( sinfo->vocab_codes.size() ) {
        int sel = m_comboBoxVocab->GetSelection();
        string code = sinfo->vocab_codes[sel];
        m_cal.get_vocab_info( &vinfo, code );
    }
    for( size_t i = 0 ; i < vinfo.words.size() ; i++ ) {
        if( m_checkTextFull->GetValue() ) {
            m_comboBoxToken->Append( vinfo.words[i] );
        } else {
            m_comboBoxToken->Append( vinfo.abbrevs[i] );
        }
    }
    if( vinfo.words.empty() ) {
        m_comboBoxToken->Append( "<None>" );
    }
    m_comboBoxToken->SetSelection( 0 );
}

void HcFrame::UpdateOutputFormat()
{
    m_comboBoxOutFormat->Clear();
    Scheme_output output;
    m_cal.get_scheme_output( &output, m_to );
    for( size_t i = 0 ; i < output.formats.size() ; i++ ) {
        m_comboBoxOutFormat->Append( output.formats[i] );
    }
    if( output.formats.size() ) {
        m_comboBoxOutFormat->SetSelection( output.current );
    }
}

void HcFrame::CalculateOutput()
{
    wxString inter, output;
    int order = m_comboBoxInFormat->GetSelection();
    m_cal.set_scheme_order( m_from, order );
    string input = m_textInput->GetValue().ToStdString();
    int format = m_comboBoxOutFormat->GetSelection();
    m_cal.set_scheme_format( m_to, format );
    if( input.size() ) {
        string age;
        size_t pos = input.find( " age " );
        if( pos != string::npos ) {
            age = input.substr( pos+1 );
            input = input.substr( 0, pos );
        }
        RangeList ranges = m_cal.str_to_rangelist( m_from, input );
        if( age.size() ) {
            Rel_info info;
            if( m_cal.str_to_rel_info( m_from, age, &info ) ) {
                ranges = m_cal.rel_rangelist( m_from, ranges, &info );
            }
        }

        inter << m_cal.rangelist_to_str( 0, ranges );

        int days = 0;
        for( size_t i = 0 ; i < ranges.size() ; i++ ) {
            days += ranges[i].jdn2 - ranges[i].jdn1 + 1;
        }
        output << m_cal.rangelist_to_str( m_to, ranges );
        if( m_show_count && days > 1 ) {
            output << "  [" << days << " days]";
        }
        if( output.empty() ) {
            output << "Invalid date";
        }
    }
    m_textIntermediate->SetValue( inter );
    m_textOutput->SetValue( output );
}

// End of src/hcal/hcframe.cpp file

