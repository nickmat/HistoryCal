HistoryCal version 0.0.6 alpha

The alpha releases are early versions to allow anybody interested in the project to follow its progress.

This version adds the ISO 8601 standard Week and Ordinal calendars as well as additional support for the ISO 8601 formats. Limited support for the EDTF standard has also been added.

A default format for writing dates as a sequence of 'value unit' pairs, it's been given the format code "u" which can be used with all calendar schemes. The primary purpose of this for expressing time intervals.

Changes to the date expression syntax. Allow characters used as operators to be passed as text by enclosing in double quotes.

A number of changes to the HistoryCal Script Language. The clear statement extended. The 'record' value type has been created, it replaces the mask value type. Increase precedence and change operation of the cast operators. The 'scheme', 'grammar', 'format' and 'vocab' codes do not need to be quoted unless they contain characters not allowed in names. Added the 'rules' sub-statement to the 'format' statement, use this to control the ISO:8601 formats.

A number of bug fixes, full details are available in the git repository.


A working Windows (XT, Vista, 7, 8, 10) version is available as file hcal_0-0-6_setup.exe

The source code is available in files hcal_0-0-6_src.zip (DOS/Win line endings) and  hcal_0-0-6_src.gz (Unix line endings). The source files have been successfully compiled on gnu/Linux (Mint) and on Mac OS X (Yosemite). See the included build/build.txt file for a description of the build process for Linux. The Mac OS X version is still being worked on.
