///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


#include "hcfbframe.h"

///////////////////////////////////////////////////////////////////////////

hcFbFrame::hcFbFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxMenuBar* m_menubar1;
	m_menubar1 = new wxMenuBar( 0 );
	wxMenu* m_menuFile;
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuRun;
	m_menuRun = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Run &Script ...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuRun );

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuExit;
	m_menuExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuExit );

	m_menubar1->Append( m_menuFile, _("&File") );

	wxMenu* m_menuView;
	m_menuView = new wxMenu();
	m_menuViewInterm = new wxMenuItem( m_menuView, wxID_ANY, wxString( _("Show &Intermediate") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuViewInterm );

	m_menuViewCount = new wxMenuItem( m_menuView, wxID_ANY, wxString( _("Add Day &Count") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuViewCount );

	m_menubar1->Append( m_menuView, _("&View") );

	wxMenu* m_menuHelp;
	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuHelpWebsite;
	m_menuHelpWebsite = new wxMenuItem( m_menuHelp, wxID_ANY, wxString( _("&Website") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpWebsite );

	wxMenuItem* m_menuHelpAbout;
	m_menuHelpAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( _("&About...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpAbout );

	m_menubar1->Append( m_menuHelp, _("&Help") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Input:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer5->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_comboBoxInput = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	bSizer5->Add( m_comboBoxInput, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );


	bSizer1->Add( bSizer5, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText4;
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("Format:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer8->Add( m_staticText4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_comboBoxInFormat = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	bSizer8->Add( m_comboBoxInFormat, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );


	bSizer1->Add( bSizer8, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText6;
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer10->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_comboBoxVocab = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	bSizer10->Add( m_comboBoxVocab, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );

	m_comboBoxToken = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	bSizer10->Add( m_comboBoxToken, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );

	m_checkTextFull = new wxCheckBox( this, wxID_ANY, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkTextFull->SetValue(true);
	bSizer10->Add( m_checkTextFull, 0, wxALIGN_CENTER_VERTICAL|wxTOP, 5 );


	bSizer1->Add( bSizer10, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_textInput = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer2->Add( m_textInput, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );


	bSizer3->Add( 0, 0, 1, 0, 5 );

	m_buttonConvert = new wxButton( this, wxID_ANY, _("Convert"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_buttonConvert, 0, wxALL, 5 );


	bSizer3->Add( 0, 0, 1, 0, 5 );


	bSizer1->Add( bSizer3, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizerIntermeadiate = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText3;
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Intermediate:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizerIntermeadiate->Add( m_staticText3, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textIntermediate = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_WORDWRAP );
	bSizerIntermeadiate->Add( m_textIntermediate, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );


	bSizer1->Add( bSizerIntermeadiate, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Output:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer6->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_comboBoxOutput = new wxComboBox( this, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	bSizer6->Add( m_comboBoxOutput, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );


	bSizer1->Add( bSizer6, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText* m_staticText41;
	m_staticText41 = new wxStaticText( this, wxID_ANY, _("Format:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	bSizer81->Add( m_staticText41, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

	m_comboBoxOutFormat = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	bSizer81->Add( m_comboBoxOutFormat, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );


	bSizer1->Add( bSizer81, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_textOutput = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textOutput->SetMinSize( wxSize( 400,80 ) );

	bSizer4->Add( m_textOutput, 1, wxALL|wxEXPAND, 5 );


	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( hcFbFrame::OnRunScript ), this, m_menuRun->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( hcFbFrame::OnExit ), this, m_menuExit->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( hcFbFrame::OnToggleInterm ), this, m_menuViewInterm->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( hcFbFrame::OnToggleCount ), this, m_menuViewCount->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( hcFbFrame::OnWebsite ), this, m_menuHelpWebsite->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( hcFbFrame::OnAbout ), this, m_menuHelpAbout->GetId());
	m_comboBoxInput->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectInput ), NULL, this );
	m_comboBoxInFormat->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectInputFormat ), NULL, this );
	m_comboBoxVocab->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectVocab ), NULL, this );
	m_comboBoxToken->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectToken ), NULL, this );
	m_checkTextFull->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( hcFbFrame::OnCheckTextFull ), NULL, this );
	m_textInput->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( hcFbFrame::OnInputEnter ), NULL, this );
	m_buttonConvert->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( hcFbFrame::OnButtonConvert ), NULL, this );
	m_comboBoxOutput->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectOutput ), NULL, this );
	m_comboBoxOutFormat->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectOutputFormat ), NULL, this );
}

hcFbFrame::~hcFbFrame()
{
	// Disconnect Events
	m_comboBoxInput->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectInput ), NULL, this );
	m_comboBoxInFormat->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectInputFormat ), NULL, this );
	m_comboBoxVocab->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectVocab ), NULL, this );
	m_comboBoxToken->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectToken ), NULL, this );
	m_checkTextFull->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( hcFbFrame::OnCheckTextFull ), NULL, this );
	m_textInput->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( hcFbFrame::OnInputEnter ), NULL, this );
	m_buttonConvert->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( hcFbFrame::OnButtonConvert ), NULL, this );
	m_comboBoxOutput->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectOutput ), NULL, this );
	m_comboBoxOutFormat->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( hcFbFrame::OnSelectOutputFormat ), NULL, this );

}
