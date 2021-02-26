# utf8proc library

set(UTF8_SOURCES 
    ../../include/utf8/utf8api.h
    utf8api.cpp
    utf8proc.c
    utf8proc.h
)

add_library(utf8api ${UTF8_SOURCES})

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (utf8api PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)
