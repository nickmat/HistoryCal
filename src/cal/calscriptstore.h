/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscriptstore.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     ScriptStore class definition header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     17th May 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
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
        ScriptStore() : ischeme(NULL), oscheme(NULL) {}

        SValueMap table;
        Scheme*   ischeme;
        Scheme*   oscheme;
    };

}

#endif // CAL_CALSCRIPTSTORE_H_GUARD
