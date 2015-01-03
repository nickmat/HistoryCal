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

inline wxString Utf8ToWxStr( const std::string& stdstr )
{
    return wxString( stdstr.c_str(), wxConvUTF8 );
}

inline std::string WxStrToUtf8( const wxString& wxstr )
{
    return std::string( wxstr.mb_str( wxConvUTF8 ) );
}

wxString RangesToString( RangeList rlist )
{
    wxString str;
    for( size_t i = 0 ; i < rlist.size() ; i++ ) {
        if( i > 0 ) {
            str << " ";
        }
        if( rlist[i].jdn1 == f_minimum ) {
            str << "min";
        } else {
            str << rlist[i].jdn1;
        }
        if( rlist[i].jdn1 != rlist[i].jdn2 ) {
            str << " ~ ";
            if( rlist[i].jdn2 == f_maximum ) {
                str << "max";
            } else {
                str << rlist[i].jdn2;
            }
        }
    }
    return str;
}

/*! \brief Frame constructor.
 *
 *  Create the window Icon.
 */
HcFrame::HcFrame(
    const wxString& title, const wxPoint& pos, const wxSize& size, long style )
    : m_cal(Init_script_default), m_from("g"), m_to("g"),
    m_show_interm(false), m_show_count(false),
    hcFbFrame( (wxFrame*) NULL, wxID_ANY, title, pos, size, style )
{
    // Set frames Icon
    SetIcon( wxICON( hcal ) );

    UpdateSchemeLists();

    bSizerIntermeadiate->Show( false );

    m_textInput->SetFocus();
}

/*! \brief Called on a Run Script Application menu option event.
 */
void HcFrame::OnRunScript( wxCommandEvent& event )
{
    wxString caption = _("Select Script");
    wxString wildcard = _("HistoryCal Script (*.hcs)|*.hcs");
    wxString defaultDir = ".";
    wxString defaultFName = wxEmptyString;

    wxFileDialog dialog( this, caption, defaultDir, defaultFName, wildcard, wxFD_OPEN );
    if( dialog.ShowModal() == wxID_OK ) {
        std::string path = WxStrToUtf8( dialog.GetPath() );
        wxString result = Utf8ToWxStr( m_cal.run_script_file( path ) );
        if( result.size() ) {
            wxMessageBox( result );
        }
        UpdateSchemeLists();
        CalculateOutput();
    }
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
    int ip = m_comboBoxInput->GetSelection();
    m_from = m_schemes[ip].code;
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
    m_cal.get_scheme_info( &info, m_cal.get_scheme( m_from ) );
    UpdateTextTokens( &info );
}

void HcFrame::OnSelectToken( wxCommandEvent& event )
{
    // Split the text in the textInput control into 2.
    // 1) Before insersion point or text selection.
    // 2) After insersion point or text selection.
    // Reconstruct it, with the token selected from comboBoxToken
    // between 1 and 2. Put it back into textInput.
    long sel1, sel2;
    m_textInput->GetSelection( &sel1, &sel2 );
    long end = m_textInput->GetLastPosition();
    wxString bef, aft;
    if( sel1 ) {
        bef = m_textInput->GetRange( 0, sel1 );
    }
    aft = m_textInput->GetRange( sel2, end );

    m_textInput->SetValue( bef + m_comboBoxToken->GetValue() + aft );
}

void HcFrame::OnCheckTextFull( wxCommandEvent& event )
{
    Scheme_info info;
    m_cal.get_scheme_info( &info, m_cal.get_scheme( m_from ) );
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
    int op = m_comboBoxOutput->GetSelection();
    m_to = m_schemes[op].code;
    UpdateOutputFormat();
    CalculateOutput();
}

void HcFrame::OnSelectOutputFormat( wxCommandEvent& event )
{
    CalculateOutput();
}

void HcFrame::UpdateSchemeLists()
{
    m_schemes = m_cal.get_scheme_list();
    m_comboBoxInput->Clear();
    m_comboBoxOutput->Clear();
    bool from_found = false, to_found = false;
    for( size_t i = 0 ; i < m_schemes.size() ; i++ ) {
        wxString entry = Utf8ToWxStr( m_schemes[i].name ) 
            + "  (" + Utf8ToWxStr( m_schemes[i].code ) + ")";
        m_comboBoxInput->Append( entry );
        if( m_schemes[i].code == m_from ) {
            m_comboBoxInput->SetSelection( i );
            from_found = true;
        }
        m_comboBoxOutput->Append( entry );
        if( m_schemes[i].code == m_to ) {
            m_comboBoxOutput->SetSelection( i );
            to_found = true;
        }
    }
    // Ensure m_from and m_to have valid scheme codes.
    if( m_schemes.size() ) {
        if( ! from_found ) {
            m_from = m_schemes[0].code;
            m_comboBoxInput->SetSelection( 0 );
        }
        if( ! to_found ) {
            m_to = m_schemes[0].code;
            m_comboBoxOutput->SetSelection( 0 );
        }
    }
    UpdateInputFormat();
    UpdateTextVocabs();
    UpdateOutputFormat();
}

void HcFrame::UpdateInputFormat()
{
    m_comboBoxInFormat->Clear();
    m_cal.get_scheme_input( &m_input_info, m_cal.get_scheme( m_from ) );
    for( size_t i = 0 ; i < m_input_info.code.size() ; i++ ) {
        string fmt = Utf8ToWxStr( m_input_info.descrip[i] )
            + "  (" + Utf8ToWxStr( m_input_info.code[i] ) + ")";
        m_comboBoxInFormat->Append( fmt );
    }
    if( m_input_info.code.size() ) {
        m_comboBoxInFormat->SetSelection( m_input_info.current );
    }
}

void HcFrame::UpdateTextVocabs()
{
    m_comboBoxVocab->Clear();
    Scheme_info info;
    m_cal.get_scheme_info( &info, m_cal.get_scheme( m_from ) );
    for( size_t i = 0 ; i < info.vocab_names.size() ; i++ ) {
        m_comboBoxVocab->Append( Utf8ToWxStr( info.vocab_names[i] ) );
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
            m_comboBoxToken->Append( Utf8ToWxStr( vinfo.words[i] ) );
        } else {
            m_comboBoxToken->Append( Utf8ToWxStr( vinfo.abbrevs[i] ) );
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
    m_cal.get_scheme_output( &m_output_info, m_cal.get_scheme( m_to ) );
    for( size_t i = 0 ; i < m_output_info.code.size() ; i++ ) {
        string fmt = Utf8ToWxStr( m_output_info.descrip[i] )
            + "  (" + Utf8ToWxStr( m_output_info.code[i] ) + ")";
        m_comboBoxOutFormat->Append( fmt );
    }
    if( m_output_info.code.size() ) {
        m_comboBoxOutFormat->SetSelection( m_output_info.current );
    }
}

void HcFrame::CalculateOutput()
{
    wxString inter, output;
    SHandle sch_from = m_cal.get_scheme( m_from );
    SHandle sch_to = m_cal.get_scheme( m_to );

    int order = m_comboBoxInFormat->GetSelection();
    m_cal.set_input_format( sch_from, m_input_info.code[order] );
    int format = m_comboBoxOutFormat->GetSelection();
    m_cal.set_output_format( sch_to, m_output_info.code[format] );
    string input = WxStrToUtf8( m_textInput->GetValue() );
    if( input.size() ) {
        string age;
        size_t pos = input.find( " age " );
        if( pos != string::npos ) {
            age = input.substr( pos+1 );
            input = input.substr( 0, pos );
        }
        RangeList ranges = m_cal.expr_str_to_rangelist( sch_from, input );
        if( age.size() ) {
            Rel_info info;
            if( m_cal.str_to_rel_info( sch_from, age, &info ) ) {
                ranges = m_cal.rel_rangelist( sch_from, ranges, &info );
            }
        }

        inter << RangesToString( ranges );

        output << Utf8ToWxStr( m_cal.rangelist_to_str( sch_to, ranges ) );
        size_t rsize = ranges.size();
        if( m_show_count && rsize ) {
            if( ranges[0].jdn1 == f_minimum || ranges[rsize-1].jdn2 == f_maximum ) {
                output << "  [Infinite days]";
            } else {
                int days = 0;
                for( size_t i = 0 ; i < rsize ; i++ ) {
                    days += ranges[i].jdn2 - ranges[i].jdn1 + 1;
                }
                if( days > 1 ) {
                    output << "  [" << days << " days]";
                }
            }
        }
        if( output.empty() ) {
            output << "Invalid date";
        }
    }
    m_textIntermediate->SetValue( inter );
    m_textOutput->SetValue( output );
}

// End of src/hcal/hcframe.cpp file

