/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcnewformatdlg.h
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     hcNewFormatDlg class header.
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


#include "hcfbformatdlg.h"
#include "cal/calendars.h"

#ifndef SRC_HCAL_HCNEWFORMATDLG_H_GUARD
#define SRC_HCAL_HCNEWFORMATDLG_H_GUARD

class hcNewFormatDlg : public fbNewFormatDlg
{
public:
    hcNewFormatDlg(
        wxWindow* parent, Cal::Calendars* cal,
        const std::string& in_scode, const std::string& out_scode );

    Cal::SHandle GetScheme() const { return m_scheme; }
    bool GetIsoFlag() const { return m_iso; }
    std::string GetFCode() const { return m_fcode; }
    std::string GetBasedFCode() const { return m_based_fcode; }

protected:
    virtual void OnSelectScheme( wxCommandEvent& event );

private:
    bool TransferDataToWindow();
    bool TransferDataFromWindow();

    void UpdateFormatList();

    Cal::Calendars*  m_cal;
    Cal::SchemeList  m_schemes;
    Cal::SHandle     m_in_scheme;
    Cal::SHandle     m_out_scheme;
    Cal::Scheme_info m_in_sinfo;
    Cal::Scheme_info m_out_sinfo;

    Cal::SHandle     m_scheme;
    std::string      m_fcode;
    bool             m_iso;
    std::string      m_based_fcode;
};

#endif // SRC_HCAL_HCNEWFORMATDLG_H_GUARD