///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __HCFBFORMATDLG_H__
#define __HCFBFORMATDLG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/radiobut.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class fbNewFormatDlg
///////////////////////////////////////////////////////////////////////////////
class fbNewFormatDlg : public wxDialog 
{
	private:
	
	protected:
		wxRadioButton* m_radioBtnInSch;
		wxRadioButton* m_radioBtnOutSch;
		wxRadioButton* m_radioBtnOtherSch;
		wxComboBox* m_comboBoxSchemes;
		wxTextCtrl* m_textCtrlFCode;
		wxCheckBox* m_checkBoxIso;
		wxStaticText* m_staticText7;
		wxComboBox* m_comboBoxFormats;
		wxStaticLine* m_staticline1;
		wxButton* m_button1;
		wxButton* m_button2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSelectScheme( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		fbNewFormatDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Create New Format"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~fbNewFormatDlg();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class fbFormatDlg
///////////////////////////////////////////////////////////////////////////////
class fbFormatDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textCtrlSCode;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrlFCode;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrlControl;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrlInputStr;
		wxCheckBox* m_checkBoxUseInput;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrlOutputStr;
		wxStaticLine* m_staticline2;
		wxButton* m_button1;
		wxButton* m_button2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ) { event.Skip(); }
		
	
	public:
		
		fbFormatDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Format Control"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~fbFormatDlg();
	
};

#endif //__HCFBFORMATDLG_H__
