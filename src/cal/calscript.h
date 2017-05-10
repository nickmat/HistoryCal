/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscript.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Run script.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th May 2014
 * Copyright:   Copyright (c) 2014 ~ 2017, Nick Matthews.
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
        bool do_mark();
        bool do_clear();
        bool do_if();
        bool do_do();
        bool do_set();
        bool do_let();
        bool do_assign( const std::string& name );
        bool do_write();
        bool do_writeln();
        bool do_scheme();
        SHandle do_create_scheme( const std::string& code );
        Base* do_base();
        Base* do_base_shift();
        Base* do_base_hybrid();
        Base* do_base_regnal();
        bool do_regnal_era( RegnalEra& era, StringVec& fieldnames );
        bool do_vocab();
        bool do_vocab_tokens( Vocab* voc );
        bool do_grammar();
        bool do_grammar_vocabs( Grammar* gmr );
        bool do_grammar_alias( Grammar* gmr );
        bool do_format( Grammar* gmr );
        FieldVec do_fixed_fields( const StringVec& fieldnames );
        bool do_function();
        bool do_call();

        SValue expr( bool get );
        SValue compare( bool get );
        SValue combine( bool get );
        SValue range( bool get );
        SValue sum( bool get );
        SValue term( bool get );
        SValue subscript( bool get );
        SValue primary( bool get );
        std::string get_name_or_primary( bool get );
        StringVec get_string_list( bool get );
        SValue get_record( bool get );

        SValue do_subscript( const SValue& left, const SValue& right );
        SValue str_cast();
        SValue date_cast();
        SValue record_cast();
        SValue error_cast();
        SValue function_call();

        SValue get_value_var( const std::string& name );

        int line() const { return m_ts.get_line(); }

        Calendars*    m_cals;
        STokenStream  m_ts;
        std::ostream* m_out;
        std::ostream* m_err;
    };

}

#endif // CAL_CALSCRIPT_H_GUARD