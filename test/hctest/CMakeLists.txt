# The command line script program
set( HCSTEST_SOURCES
    hcsgetfns.cpp
    hctest.cpp
    hctestmain.h
)
add_executable(hctest ${HCSTEST_SOURCES})
target_link_libraries(hctest cal)
