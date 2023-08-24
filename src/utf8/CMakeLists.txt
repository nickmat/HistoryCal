# utf8proc library

set(UTF8_SOURCES 
    utf8api.cpp
)

set( UTF8_HEADERS
    ../../include/utf8/utf8api.h
)

add_library( utf8api ${UTF8_SOURCES} ${UTF8_HEADERS} )

target_include_directories( utf8api PRIVATE ../../3rdparty )

target_link_libraries( utf8api PRIVATE utf8proc )
