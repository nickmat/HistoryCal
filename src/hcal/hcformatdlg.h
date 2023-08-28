/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hcal/hcformatdlg.h
 * Project:     HistoryCal: Programmable Historical Calendar Calculator.
 * Purpose:     hcFormatDlg class header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     12th August 2016
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
#include <glc/glc.h>

#ifndef SRC_HCAL_HCFORMATDLG_H_GUARD
#define SRC_HCAL_HCFORMATDLG_H_GUARD

class hcFormatDlg : public fbFormatDlg
{
public:
    hcFormatDlg( wxWindow* parent, glich::Glich* glc,
       const std::string& scode, const std::string& fcode, const std::string& init);

    std::string get_script() const { return m_script; }

private:
    bool TransferDataToWindow();
    bool TransferDataFromWindow();

    virtual void OnIdle( wxIdleEvent& event );

    glich::Glich* m_glc;
    std::string   m_scode;
        
    std::string   m_fcode;
    std::string   m_gcode;
    std::string   m_based_on;
    std::string   m_control;
    std::string   m_script;
};

#endif // SRC_HCAL_HCFORMATDLG_H_GUARD