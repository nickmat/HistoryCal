cmake_minimum_required(VERSION 2.8)
project(HistoryCal)
set(CMAKE_BUILD_TYPE Release)

# Project header files path
include_directories(../include)

# The UTF8 library used:
set(UTF8_SOURCES 
  ../include/utf8/utf8api.h
  ../src/utf8/utf8api.cpp
  ../src/utf8/utf8proc.c
  ../src/utf8/utf8proc.h
)
add_library(utf8api SHARED ${UTF8_SOURCES})

#The main cal library:
file(GLOB CAL_SOURCES "../src/cal/*.cpp")
add_library(cal SHARED ${CAL_SOURCES})
target_link_libraries(cal utf8api)

# The command line script program
set(HCS_SOURCES ../src/hcs/hcs.cpp)
add_executable(hcs ${HCS_SOURCES})
target_link_libraries(hcs cal)

# The wxWidgets GUI program.
file(GLOB HCAL_SOURCES "../src/hcal/*.cpp")
add_executable(hcal WIN32 ${HCAL_SOURCES} ../src/hcal/hcal.rc)
# Note, order of wxWidgets libs may be important!
find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(hcal cal ${wxWidgets_LIBRARIES})

# The script test program.
set(HCTEST_SOURCES
  ../test/hctest/hcsgetfns.cpp
  ../test/hctest/hctest.cpp
  ../test/hctest/hctestmain.h
)
add_executable(hctest ${HCTEST_SOURCES})
target_link_libraries(hctest cal)

find_package(CppUnit REQUIRED)
file(GLOB UNIT_SOURCES "../test/unit/*.cpp")
add_executable(unit ${UNIT_SOURCES})
target_include_directories(unit ${CPPUNIT_INCLUDE_DIR})
target_link_libraries( unit ${CPPUNIT_LIBRARIES})

