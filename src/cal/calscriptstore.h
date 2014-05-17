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

#ifndef CAL_CALSCRIPTSTORE_H_GUARD
#define CAL_CALSCRIPTSTORE_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class Scheme;
    class Grammar;
    class Vocab;

    typedef std::map<std::string,Grammar*> GrammarMap;
    typedef std::map<std::string,Vocab*> VocabMap;
    typedef std::map<std::string,RangeList> RListMap;

    class ScriptStore {
    public:
        ScriptStore() : ischeme(NULL), oscheme(NULL) {}

        RListMap  rlisttable;
        Scheme*   ischeme;
        Scheme*   oscheme;
    };

}

#endif // CAL_CALSCRIPTSTORE_H_GUARD
