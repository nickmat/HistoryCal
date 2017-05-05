/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calfunction.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Function class to handle script functions.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th April 2017
 * Copyright:   Copyright (c) 2017, Nick Matthews.
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

#ifndef CAL_CALFUNCTION_H_GUARD
#define CAL_CALFUNCTION_H_GUARD

#include "cal/caldefs.h"
#include "calsvalue.h"

namespace Cal {

    class Function
    {
    public:
        Function( const std::string& code ) : m_code( code ) {}

        void set_script( const std::string& script ) { m_script = script; }
        void set_line( int line ) { m_line = line; }
        void set_args( const StringVec& args ) { m_args = args; }
        void set_defaults( const SValueVec& defs ) { m_defs = defs; }

        std::string get_code() const { return m_code; }
        int get_line() const { return m_line; }
        std::string get_script() const { return m_script; }
        size_t get_arg_size() const { return m_args.size(); }
        std::string get_arg_name( size_t index ) const { return m_args[index]; }
        SValue get_default_value( size_t index ) const { return m_defs[index]; }

    private:
        std::string m_code;
        int         m_line;
        std::string m_script;
        StringVec   m_args;
        SValueVec   m_defs;
    };

}

#endif // CAL_CALFUNCTION_H_GUARD