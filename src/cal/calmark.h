/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calmark.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Mark class used by the scripts mark statement.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th October 2014
 * Copyright:   Copyright (c) 2014 - 2015, Nick Matthews.
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

#ifndef CAL_CALMARK_H_GUARD
#define CAL_CALMARK_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class Mark
    {
    public:
        Mark( const std::string& name );
        ~Mark();

        std::string get_name() const { return m_name; }

        void add_scheme( SHandle scheme );
        void add_grammar( Grammar* grammar );
        void add_vocab( Vocab* vocab );
        void add_format( Format* format );

        std::string remove_next_scheme();
        std::string remove_next_grammar();
        std::string remove_next_vocab();
        std::string remove_next_format();

    private:
        std::string            m_name;
        std::vector<SHandle>   m_schemes;
        std::vector<Grammar*>  m_grammars;
        std::vector<Vocab*>    m_vocabs;
        std::vector<Format*>   m_formats;
    };

}

#endif // CAL_CALMARK_H_GUARD