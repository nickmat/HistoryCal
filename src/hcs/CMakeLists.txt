# The command line script program
set(HCS_SOURCES hcs.cpp)
add_executable(hcs ${HCS_SOURCES})
target_link_libraries(hcs cal)
