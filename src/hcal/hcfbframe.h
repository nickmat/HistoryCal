///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __HCFBFRAME_H__
#define __HCFBFRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class hcFbFrame
///////////////////////////////////////////////////////////////////////////////
class hcFbFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menuFile;
		wxMenu* m_menuHelp;
		wxStaticText* m_staticText1;
		wxComboBox* m_comboBoxInput;
		wxStaticText* m_staticText4;
		wxComboBox* m_comboBoxInFormat;
		wxStaticText* m_staticText6;
		wxComboBox* m_comboBoxVocab;
		wxComboBox* m_comboBoxToken;
		wxTextCtrl* m_textInput;
		wxButton* m_buttonConvert;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textIntermediate;
		wxStaticText* m_staticText2;
		wxComboBox* m_comboBoxOutput;
		wxStaticText* m_staticText41;
		wxComboBox* m_comboBoxOutFormat;
		wxTextCtrl* m_textOutput;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWebsite( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectInput( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectInputFormat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectVocab( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectToken( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInputEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonConvert( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectOutput( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectOutputFormat( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		hcFbFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("HistoryCal"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~hcFbFrame();
	
};

#endif //__HCFBFRAME_H__
