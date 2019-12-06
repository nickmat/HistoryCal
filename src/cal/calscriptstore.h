/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscriptstore.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     ScriptStore class definition header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     17th May 2014
 * Copyright:   Copyright (c) 2014 ~ 2019, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  The Cal library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The Cal library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the Cal library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#ifndef CAL_CALSCRIPTSTORE_H_GUARD
#define CAL_CALSCRIPTSTORE_H_GUARD

#include "cal/caldefs.h"
#include "calsvalue.h"

namespace Cal {

    class Scheme;

    typedef std::map<std::string,SValue> SValueMap;

    class ScriptStore {
    public:
        ScriptStore()
            : m_ischeme( nullptr ), m_oscheme( nullptr ), m_prev( nullptr ) {}
        ScriptStore( ScriptStore* ss )
            : m_ischeme( ss->m_ischeme ), m_oscheme( ss->m_oscheme ), m_prev( ss ) {}

        void set_ischeme( Scheme* sch ) { m_ischeme = sch; }
        void set_oscheme( Scheme* sch ) { m_oscheme = sch; }

        Scheme* get_ischeme() const { return m_ischeme; }
        Scheme* get_oscheme() const { return m_oscheme; }
        ScriptStore* get_prev() const { return m_prev; }

        bool exists( const std::string& name ) const { return m_table.count( name ) == 1; }
        void set( const std::string& name, const SValue& value) { m_table[name] = value; }
        bool get( SValue* value, const std::string& name ) const {
            if ( exists( name ) ) {
                *value = m_table.at( name );
                return true;
            }
            return false;
        }
        void clear() { 
            m_table.clear();
            m_ischeme = m_oscheme = nullptr;
        }

    private:
        SValueMap m_table;
        Scheme*   m_ischeme;
        Scheme*   m_oscheme;
        ScriptStore* m_prev;
    };

}

#endif // CAL_CALSCRIPTSTORE_H_GUARD
