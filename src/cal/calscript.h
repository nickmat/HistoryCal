/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscript.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Run script.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th May 2014
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

#ifndef CAL_CALSCRIPT_H_GUARD
#define CAL_CALSCRIPT_H_GUARD

#include "cal/caldefs.h"
#include "calstokenstream.h"

namespace Cal {

    class Calendars;
    class Base;
    class Vocab;
    struct RegnalEra;

    class Script
    {
    public:
        Script( Calendars* cals, std::istream& in, std::ostream& out );

        bool run();

    private:
        bool error( const std::string& mess ) { return m_ts.error( mess ); }
        ScriptStore* store() const;
        bool statement();
        bool do_set();
        bool do_rlist();
        bool do_let();
        bool do_write();
        bool do_writeln();
        bool do_mark();
        bool do_scheme();
        SHandle do_create_scheme( const std::string& code );
        Base* do_base();
        Base* do_base_shift();
        Base* do_base_hybrid();
        Base* do_base_regnal();
        bool do_regnal_era( RegnalEra& era, const StringVec& fieldnames );
        bool do_vocab();
        bool do_vocab_tokens( Vocab* voc );
        bool do_grammar();
        bool do_grammar_vocabs( Grammar* gmr );
        bool do_grammar_format( Grammar* gmr );
        bool do_grammar_alias( Grammar* gmr );
        StringVec do_string_list();
        FieldVec do_fixed_fields( const StringVec& fieldnames );

        SValue expr( bool get );
        SValue compare( bool get );
        SValue sum( bool get );
        SValue term( bool get );
        SValue combine( bool get );
        SValue range( bool get );
        SValue primary( bool get );
        SValue fields_expr( bool get );

        SValue get_value_var( const std::string& name );
        std::string get_name_or_string( const SToken& token ) const;

        Calendars*    m_cals;
        STokenStream  m_ts;
        std::ostream* m_out;
        std::ostream* m_err;
    };

}

#endif // CAL_CALSCRIPT_H_GUARD