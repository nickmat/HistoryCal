/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calschemes.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Schemes (scheme list) class header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
 * Copyright:   Copyright (c) 2013-2014, Nick Matthews.
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

#ifndef CAL_CALSCHEMES_H_GUARD
#define CAL_CALSCHEMES_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class Scheme;
    class Grammar;
    class Vocab;

    typedef std::map<std::string,Grammar*> GrammarMap;
    typedef std::map<std::string,Vocab*> VocabMap;

    class Schemes
    {
    public:
        Schemes() {}
        ~Schemes();

        void add_scheme( const std::string& definition );
        void add_vocab( const std::string& definition );
        void add_grammar( const std::string& definition );

        int get_scheme_count() const { return m_scheme_vec.size(); }

        Scheme* get_scheme( int scheme_id ) const;
        Scheme* get_scheme( const std::string& code ) const;
        int get_scheme_id( const std::string& code ) const;
        Grammar* get_grammar( const std::string& code ) const;
        Vocab* get_vocab( const std::string& code ) const;

    private:
        bool grammar_exist( const std::string& code ) const;
        bool vocab_exist( const std::string& code ) const;

        std::vector<Scheme*>      m_scheme_vec;
        std::map<std::string,int> m_scheme_ids;
        GrammarMap                m_grammars;
        VocabMap                  m_vocabs;
    };
}

#endif // CAL_CALSCHEMES_H_GUARD
