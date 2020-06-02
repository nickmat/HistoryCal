/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calfile.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     File class to handle script files.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     1st June 2020
 * Copyright:   Copyright (c) 2020, Nick Matthews.
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

#ifndef CAL_CALFILE_H_GUARD
#define CAL_CALFILE_H_GUARD

#include "cal/caldefs.h"
#include <fstream>

namespace Cal {

    class File {
    public:
        enum FileType { FT_null, FT_write, FT_append };

        File ( const std::string& code ) : m_code( code ), m_filetype( FT_write ) {}
        
        void set_filename( const std::string& filename ) { m_filename = filename; }
        void set_filetype( FileType filetype ) { m_filetype = filetype; }

        std::string get_code() const { return m_code; }

        bool open();
        std::ostream* get_out() { return &m_file; }

    private:
        std::string  m_code;
        std::string  m_filename;
        FileType     m_filetype;
        std::fstream m_file;
    };

}

#endif // CAL_CALFILE_H_GUARD
