///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


#include "hcfbformatdlg.h"

///////////////////////////////////////////////////////////////////////////

fbNewFormatDlg::fbNewFormatDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Scheme:") ), wxVERTICAL );
	
	m_radioBtnInSch = new wxRadioButton( sbSizer1->GetStaticBox(), wxID_ANY, _("Input"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_radioBtnInSch, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_radioBtnOutSch = new wxRadioButton( sbSizer1->GetStaticBox(), wxID_ANY, _("Output"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_radioBtnOutSch, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_radioBtnOtherSch = new wxRadioButton( sbSizer1->GetStaticBox(), wxID_ANY, _("Other:"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_radioBtnOtherSch, 0, wxALL, 5 );
	
	m_comboBoxSchemes = new wxComboBox( sbSizer1->GetStaticBox(), wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY ); 
	bSizer9->Add( m_comboBoxSchemes, 1, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	sbSizer1->Add( bSizer9, 0, wxEXPAND, 5 );
	
	
	bSizer7->Add( sbSizer1, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText6;
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("Code:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer10->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_textCtrlFCode = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer10->Add( m_textCtrlFCode, 1, wxTOP|wxRIGHT, 5 );
	
	m_checkBoxIso = new wxCheckBox( this, wxID_ANY, _("ISO:8601"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_checkBoxIso, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	
	bSizer7->Add( bSizer10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("Based on:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer101->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_comboBoxFormats = new wxComboBox( this, wxID_ANY, _("Combo!"), wxDefaultPosition, wxSize( 250,-1 ), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY ); 
	bSizer101->Add( m_comboBoxFormats, 1, wxTOP|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer7->Add( bSizer101, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer7->Add( m_staticline1, 0, wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button1 = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button2, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	bSizer7->Add( bSizer11, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	bSizer7->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_radioBtnInSch->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( fbNewFormatDlg::OnSelectScheme ), NULL, this );
	m_radioBtnOutSch->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( fbNewFormatDlg::OnSelectScheme ), NULL, this );
	m_radioBtnOtherSch->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( fbNewFormatDlg::OnSelectScheme ), NULL, this );
	m_comboBoxSchemes->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( fbNewFormatDlg::OnSelectScheme ), NULL, this );
}

fbNewFormatDlg::~fbNewFormatDlg()
{
	// Disconnect Events
	m_radioBtnInSch->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( fbNewFormatDlg::OnSelectScheme ), NULL, this );
	m_radioBtnOutSch->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( fbNewFormatDlg::OnSelectScheme ), NULL, this );
	m_radioBtnOtherSch->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( fbNewFormatDlg::OnSelectScheme ), NULL, this );
	m_comboBoxSchemes->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( fbNewFormatDlg::OnSelectScheme ), NULL, this );
	
}

fbFormatDlg::fbFormatDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("Scheme:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer6->Add( m_staticText4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_textCtrlSCode = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer6->Add( m_textCtrlSCode, 2, wxTOP|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, _("Format:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer6->Add( m_staticText5, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	m_textCtrlFCode = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer6->Add( m_textCtrlFCode, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	
	bSizer1->Add( bSizer6, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Control:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_textCtrlControl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_textCtrlControl, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Input:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_textCtrlInputStr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer3->Add( m_textCtrlInputStr, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	m_checkBoxUseInput = new wxCheckBox( this, wxID_ANY, _("Use"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_checkBoxUseInput, 0, wxALIGN_CENTER_VERTICAL|wxTOP, 5 );
	
	
	bSizer1->Add( bSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Output:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer5->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlOutputStr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer5->Add( m_textCtrlOutputStr, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	bSizer1->Add( bSizer5, 0, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline2, 0, wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button1 = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button2, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	bSizer1->Add( bSizer11, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( fbFormatDlg::OnIdle ) );
}

fbFormatDlg::~fbFormatDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( fbFormatDlg::OnIdle ) );
	
}
