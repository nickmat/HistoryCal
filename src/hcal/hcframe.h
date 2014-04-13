/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcframe.h
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     HcFrame class header.
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

#ifndef SRC_HCAL_HCFRAME_H_GUARD
#define SRC_HCAL_HCFRAME_H_GUARD

#include "hcfbframe.h"

#include <cal/calendars.h>

/*! \brief The main HistoryCal application frame.
 *
 */
class HcFrame : public hcFbFrame
{
public:
    HcFrame( 
        const wxString& title, 
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE );

private:
		virtual void OnExit( wxCommandEvent& event );
		virtual void OnToggleInterm( wxCommandEvent& event );
		virtual void OnToggleCount( wxCommandEvent& event );
		virtual void OnWebsite( wxCommandEvent& event );
		virtual void OnAbout( wxCommandEvent& event );

        virtual void OnSelectInput( wxCommandEvent& event );
		virtual void OnSelectInputFormat( wxCommandEvent& event );
		virtual void OnSelectVocab( wxCommandEvent& event );
		virtual void OnSelectToken( wxCommandEvent& event );
		virtual void OnCheckTextFull( wxCommandEvent& event );
		virtual void OnInputEnter( wxCommandEvent& event );
		virtual void OnButtonConvert( wxCommandEvent& event );
		virtual void OnSelectOutput( wxCommandEvent& event );
		virtual void OnSelectOutputFormat( wxCommandEvent& event );

        void UpdateInputFormat();
        void UpdateTextVocabs();
        void UpdateTextTokens( Cal::Scheme_info* info );
        void UpdateOutputFormat();
        void CalculateOutput();

        Cal::Calendars   m_cal;
        std::vector<int> m_schemes;
        int   m_from;
        int   m_to;
        bool  m_show_interm;
        bool  m_show_count;
};

#endif // SRC_HCAL_HCFRAME_H_GUARD

