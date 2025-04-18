# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        src/hcal/CMakeLists.txt
# Project:     hcal: HistoryCal Application.
# Author:      Nick Matthews
# Website:     http://historycal.org/
# Created:     22nd January 2021
# Copyright:   Copyright (c) 2021..2025, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set(HCAL_HEADERS
  hcapp.h
  hcconvert.h
  hcfbframe.h
  hcframe.h
  hcversion.h
)
set(HCAL_SOURCES
  hcapp.cpp
  hcfbframe.cpp
  hcframe.cpp
  hcversion.cpp
)

add_executable(hcal WIN32 ${HCAL_SOURCES} ${HCAL_HEADERS} hcal.rc)

set_property(TARGET hcal PROPERTY CXX_STANDARD 17)

target_link_libraries (hcal PUBLIC hic glc wx::net wx::core wx::base)
