/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcframe.h
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     HcFrame class header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th September 2013
 * Copyright:   Copyright (c) 2013..2025, Nick Matthews.
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
#include <glc/hic.h>


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
		void OnRunScript( wxCommandEvent& event ) override;
		void OnExit( wxCommandEvent& event ) override;
		void OnToggleInterm( wxCommandEvent& event ) override;
		void OnToggleCount( wxCommandEvent& event ) override;
		void OnWebsite( wxCommandEvent& event ) override;
		void OnAbout( wxCommandEvent& event ) override;

        void OnSelectInput( wxCommandEvent& event ) override;
		void OnSelectInputFormat( wxCommandEvent& event ) override;
		void OnSelectVocab( wxCommandEvent& event ) override;
		void OnSelectToken( wxCommandEvent& event ) override;
		void OnCheckTextFull( wxCommandEvent& event ) override;
		void OnInputEnter( wxCommandEvent& event ) override;
		void OnButtonConvert( wxCommandEvent& event ) override;
		void OnSelectOutput( wxCommandEvent& event ) override;
		void OnSelectOutputFormat( wxCommandEvent& event ) override;

        void UpdateSchemeLists();
        void UpdateInputFormat();
        void UpdateTextVocabs();
        void UpdateTextTokens( glich::Scheme_info* info );
        void UpdateOutputFormat();
        void CalculateOutput();

        std::string  m_from;
        std::string  m_to;
        glich::SchemeList        m_schemes;
        glich::SchemeFormatInfo  m_input_info;
        glich::SchemeFormatInfo  m_output_info;
        bool m_show_interm;
        bool m_show_count;
};

#endif // SRC_HCAL_HCFRAME_H_GUARD

