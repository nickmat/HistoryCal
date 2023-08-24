# utf8proc library

set(UTF8PROC_SOURCES 
    utf8proc.c
)

set( UTF8PROC_HEADERS
    utf8proc.h
)

add_library( utf8proc ${UTF8PROC_SOURCES} ${UTF8PROC_HEADERS} )
