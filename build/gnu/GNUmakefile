# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.12 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# Compiler flags to link shared library 
LINK_DLL_FLAGS ?= -shared

# C compiler 
CC = gcc

# C++ compiler 
CXX = `$(WX_CONFIG) --cxx`

# Standard flags for CC 
CFLAGS ?= 

# Standard flags for C++ 
CXXFLAGS ?= 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS ?= 

# Standard linker flags 
LDFLAGS ?= 

# Use debug build of wxWidgets (linked with debug CRT)? [0,1]
WX_DEBUG ?= 1

# Location and arguments of wx-config script 
WX_CONFIG ?= wx-config

# Port of the wx library to build against [gtk1,gtk2,msw,x11,motif,osx_cocoa,osx_carbon,dfb]
WX_PORT ?= $(shell $(WX_CONFIG) --query-toolkit)

# Use DLL build of wx library to use? [0,1]
WX_SHARED ?= $(shell if test -z `$(WX_CONFIG) --query-linkage`; then echo 1; else echo 0; fi)

# Compile Unicode build of wxWidgets? [0,1]
WX_UNICODE ?= $(shell $(WX_CONFIG) --query-chartype | sed 's/unicode/1/;s/ansi/0/')

# Version of the wx library to build against. 
WX_VERSION ?= $(shell $(WX_CONFIG) --query-version | sed -e 's/\([0-9]*\)\.\([0-9]*\)/\1\2/')



# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

### Variables: ###

CPPDEPS = -MT$@ -MF`echo $@ | sed -e 's,\.o$$,.d,'` -MD -MP
WX_VERSION_MAJOR = $(shell echo $(WX_VERSION) | cut -c1,1)
WX_VERSION_MINOR = $(shell echo $(WX_VERSION) | cut -c2,2)
WX_CONFIG_FLAGS = $(WX_CONFIG_UNICODE_FLAG) $(WX_CONFIG_SHARED_FLAG) \
	--toolkit=$(WX_PORT) --version=$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR)
HCAL_CXXFLAGS = $(VAR) $(VAR_7) -I../../include -I../../src `$(WX_CONFIG) \
	--cxxflags $(WX_CONFIG_FLAGS)` -std=c++11 $(CPPFLAGS) $(CXXFLAGS)
HCAL_OBJECTS =  \
	hcal_hcapp.o \
	hcal_hcfbformatdlg.o \
	hcal_hcfbframe.o \
	hcal_hcformatdlg.o \
	hcal_hcframe.o \
	hcal_hcnewformatdlg.o \
	hcal_hcversion.o \
	hcal_dummy.o
HCS_CXXFLAGS = $(VAR) $(VAR_7) -I../../include -std=c++11 $(CPPFLAGS) $(CXXFLAGS)
HCS_OBJECTS =  \
	hcs_hcs.o
HCTEST_CXXFLAGS = $(VAR) $(VAR_7) -I../../include -std=c++11 $(CPPFLAGS) \
	$(CXXFLAGS)
HCTEST_OBJECTS =  \
	hctest_hctest.o \
	hctest_hcsgetfns.o
UNIT_CXXFLAGS = $(VAR) $(VAR_7) -I../../include -I$(CPPUNIT)/include -std=c++11 \
	$(CPPFLAGS) $(CXXFLAGS)
UNIT_OBJECTS =  \
	unit_def_eng.o \
	unit_def_er.o \
	unit_def_expr_str.o \
	unit_def_fr.o \
	unit_def_g.o \
	unit_def_glit.o \
	unit_def_h.o \
	unit_def_i.o \
	unit_def_isoo.o \
	unit_def_isow.o \
	unit_def_j325.o \
	unit_def_jce.o \
	unit_def_je.o \
	unit_def_jlit.o \
	unit_testcal.o \
	unit_testchinese.o \
	unit_testcommon.o \
	unit_testformat.o \
	unit_testfrenchrep.o \
	unit_testgrammar.o \
	unit_testgregorian.o \
	unit_testhebrew.o \
	unit_testhybrid.o \
	unit_testislamic.o \
	unit_testiso.o \
	unit_testj325.o \
	unit_testjulian.o \
	unit_testmain.o \
	unit_testregnal.o \
	unit_testshiftday.o \
	unit_testvocab.o
SOAK_CXXFLAGS = $(VAR) $(VAR_7) -I../../include -I$(CPPUNIT)/include -std=c++11 \
	$(CPPFLAGS) $(CXXFLAGS)
SOAK_OBJECTS =  \
	soak_soak_eng.o \
	soak_soak_er.o \
	soak_soak_fr.o \
	soak_soak_g.o \
	soak_soak_h.o \
	soak_soak_i.o \
	soak_soak_j.o \
	soak_soak_j325.o \
	soak_soakcommon.o \
	soak_soakmain.o
CAL_CXXFLAGS = $(VAR) $(VAR_7) -DMAKINGLIB_CAL -I../../include -fPIC -DPIC \
	-std=c++11 $(CPPFLAGS) $(CXXFLAGS)
CAL_OBJECTS =  \
	cal_calastro.o \
	cal_calbase.o \
	cal_calcalendars.o \
	cal_calchinese.o \
	cal_caldefscripts.o \
	cal_caldlist.o \
	cal_caleaster.o \
	cal_calelement.o \
	cal_calfile.o \
	cal_calformat.o \
	cal_calformatiso.o \
	cal_calformattext.o \
	cal_calformatunit.o \
	cal_calfrench.o \
	cal_calfunction.o \
	cal_calgrammar.o \
	cal_calgregorian.o \
	cal_calhebrew.o \
	cal_calislamic.o \
	cal_calisoordinal.o \
	cal_calisoweek.o \
	cal_calhybrid.o \
	cal_caljdn.o \
	cal_caljulian.o \
	cal_calliturgical.o \
	cal_calmark.o \
	cal_calmath.o \
	cal_calparse.o \
	cal_calpattern.o \
	cal_calrecord.o \
	cal_calregnal.o \
	cal_calscheme.o \
	cal_calscript.o \
	cal_calsetmath.o \
	cal_calshift.o \
	cal_calstokenstream.o \
	cal_calsvalue.o \
	cal_caltext.o \
	cal_calversion.o \
	cal_calvocab.o
UTF8API_CFLAGS = $(VAR) $(VAR_7) -DMAKINGLIB_UTF8API -I../../include -fPIC -DPIC \
	$(CPPFLAGS) $(CFLAGS)
UTF8API_CXXFLAGS = $(VAR) $(VAR_7) -DMAKINGLIB_UTF8API -I../../include -fPIC \
	-DPIC -std=c++11 $(CPPFLAGS) $(CXXFLAGS)
UTF8API_OBJECTS =  \
	utf8api_utf8proc.o \
	utf8api_utf8api.o

### Conditionally set variables: ###

ifeq ($(WX_UNICODE),0)
WX_CONFIG_UNICODE_FLAG = --unicode=no
endif
ifeq ($(WX_UNICODE),1)
WX_CONFIG_UNICODE_FLAG = --unicode=yes
endif
ifeq ($(WX_SHARED),0)
WX_CONFIG_SHARED_FLAG = --static=yes
endif
ifeq ($(WX_SHARED),1)
WX_CONFIG_SHARED_FLAG = --static=no
endif
ifeq ($(WX_DEBUG),0)
____WX_DEBUG = -lcppunit
endif
ifeq ($(WX_DEBUG),1)
____WX_DEBUG = -lcppunitd
endif
ifeq ($(WX_DEBUG),0)
VAR = -O2
endif
ifeq ($(WX_DEBUG),1)
VAR = -O0
endif
ifeq ($(WX_DEBUG),0)
VAR_7 = 
endif
ifeq ($(WX_DEBUG),1)
VAR_7 = -g
endif


### Targets: ###

all: test_for_selected_wxbuild hcal hcs hctest unit soak libcal.so libutf8api.so

install: 

uninstall: 

clean: 
	rm -f ./*.o
	rm -f ./*.d
	rm -f hcal
	rm -f hcs
	rm -f hctest
	rm -f unit
	rm -f soak
	rm -f libcal.so
	rm -f libcal.so
	rm -f libutf8api.so
	rm -f libutf8api.so

test_for_selected_wxbuild: 
	@$(WX_CONFIG) $(WX_CONFIG_FLAGS)

hcal: $(HCAL_OBJECTS) libcal.so libutf8api.so
	$(CXX) -o $@ $(HCAL_OBJECTS)  $(VAR_7)  $(LDFLAGS)  libcal.so libutf8api.so `$(WX_CONFIG) $(WX_CONFIG_FLAGS) --libs core,base`

hcs: $(HCS_OBJECTS) libcal.so libutf8api.so
	$(CXX) -o $@ $(HCS_OBJECTS)  $(VAR_7)  $(LDFLAGS)  libcal.so libutf8api.so

hctest: $(HCTEST_OBJECTS) libcal.so libutf8api.so
	$(CXX) -o $@ $(HCTEST_OBJECTS)  $(VAR_7)  $(LDFLAGS)  libcal.so libutf8api.so

unit: $(UNIT_OBJECTS) libcal.so libutf8api.so
	$(CXX) -o $@ $(UNIT_OBJECTS)  $(VAR_7)  -L$(CPPUNIT)/lib $(LDFLAGS)  libcal.so libutf8api.so $(____WX_DEBUG)

soak: $(SOAK_OBJECTS) libcal.so libutf8api.so
	$(CXX) -o $@ $(SOAK_OBJECTS)  $(VAR_7)  -L$(CPPUNIT)/lib $(LDFLAGS)  libcal.so libutf8api.so $(____WX_DEBUG)

libcal.so: $(CAL_OBJECTS) libutf8api.so
	$(CXX) $(LINK_DLL_FLAGS) -fPIC -o $@ $(CAL_OBJECTS)  $(VAR_7) $(LDFLAGS)  libutf8api.so

libutf8api.so: $(UTF8API_OBJECTS)
	$(CXX) $(LINK_DLL_FLAGS) -fPIC -o $@ $(UTF8API_OBJECTS)  $(VAR_7) $(LDFLAGS)

hcal_hcapp.o: ./../../src/hcal/hcapp.cpp
	$(CXX) -c -o $@ $(HCAL_CXXFLAGS) $(CPPDEPS) $<

hcal_hcfbformatdlg.o: ./../../src/hcal/hcfbformatdlg.cpp
	$(CXX) -c -o $@ $(HCAL_CXXFLAGS) $(CPPDEPS) $<

hcal_hcfbframe.o: ./../../src/hcal/hcfbframe.cpp
	$(CXX) -c -o $@ $(HCAL_CXXFLAGS) $(CPPDEPS) $<

hcal_hcformatdlg.o: ./../../src/hcal/hcformatdlg.cpp
	$(CXX) -c -o $@ $(HCAL_CXXFLAGS) $(CPPDEPS) $<

hcal_hcframe.o: ./../../src/hcal/hcframe.cpp
	$(CXX) -c -o $@ $(HCAL_CXXFLAGS) $(CPPDEPS) $<

hcal_hcnewformatdlg.o: ./../../src/hcal/hcnewformatdlg.cpp
	$(CXX) -c -o $@ $(HCAL_CXXFLAGS) $(CPPDEPS) $<

hcal_hcversion.o: ./../../src/hcal/hcversion.cpp
	$(CXX) -c -o $@ $(HCAL_CXXFLAGS) $(CPPDEPS) $<

hcal_dummy.o: ./../../src/dummy.cpp
	$(CXX) -c -o $@ $(HCAL_CXXFLAGS) $(CPPDEPS) $<

hcs_hcs.o: ./../../src/hcs/hcs.cpp
	$(CXX) -c -o $@ $(HCS_CXXFLAGS) $(CPPDEPS) $<

hctest_hctest.o: ./../../test/hctest/hctest.cpp
	$(CXX) -c -o $@ $(HCTEST_CXXFLAGS) $(CPPDEPS) $<

hctest_hcsgetfns.o: ./../../test/hctest/hcsgetfns.cpp
	$(CXX) -c -o $@ $(HCTEST_CXXFLAGS) $(CPPDEPS) $<

unit_def_eng.o: ./../../test/unit/def_eng.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_er.o: ./../../test/unit/def_er.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_expr_str.o: ./../../test/unit/def_expr_str.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_fr.o: ./../../test/unit/def_fr.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_g.o: ./../../test/unit/def_g.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_glit.o: ./../../test/unit/def_glit.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_h.o: ./../../test/unit/def_h.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_i.o: ./../../test/unit/def_i.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_isoo.o: ./../../test/unit/def_isoo.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_isow.o: ./../../test/unit/def_isow.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_j325.o: ./../../test/unit/def_j325.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_jce.o: ./../../test/unit/def_jce.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_je.o: ./../../test/unit/def_je.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_def_jlit.o: ./../../test/unit/def_jlit.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testcal.o: ./../../test/unit/testcal.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testchinese.o: ./../../test/unit/testchinese.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testcommon.o: ./../../test/unit/testcommon.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testformat.o: ./../../test/unit/testformat.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testfrenchrep.o: ./../../test/unit/testfrenchrep.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testgrammar.o: ./../../test/unit/testgrammar.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testgregorian.o: ./../../test/unit/testgregorian.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testhebrew.o: ./../../test/unit/testhebrew.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testhybrid.o: ./../../test/unit/testhybrid.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testislamic.o: ./../../test/unit/testislamic.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testiso.o: ./../../test/unit/testiso.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testj325.o: ./../../test/unit/testj325.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testjulian.o: ./../../test/unit/testjulian.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testmain.o: ./../../test/unit/testmain.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testregnal.o: ./../../test/unit/testregnal.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testshiftday.o: ./../../test/unit/testshiftday.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

unit_testvocab.o: ./../../test/unit/testvocab.cpp
	$(CXX) -c -o $@ $(UNIT_CXXFLAGS) $(CPPDEPS) $<

soak_soak_eng.o: ./../../test/soak/soak_eng.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soak_er.o: ./../../test/soak/soak_er.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soak_fr.o: ./../../test/soak/soak_fr.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soak_g.o: ./../../test/soak/soak_g.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soak_h.o: ./../../test/soak/soak_h.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soak_i.o: ./../../test/soak/soak_i.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soak_j.o: ./../../test/soak/soak_j.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soak_j325.o: ./../../test/soak/soak_j325.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soakcommon.o: ./../../test/soak/soakcommon.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

soak_soakmain.o: ./../../test/soak/soakmain.cpp
	$(CXX) -c -o $@ $(SOAK_CXXFLAGS) $(CPPDEPS) $<

cal_calastro.o: ./../../src/cal/calastro.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calbase.o: ./../../src/cal/calbase.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calcalendars.o: ./../../src/cal/calcalendars.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calchinese.o: ./../../src/cal/calchinese.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_caldefscripts.o: ./../../src/cal/caldefscripts.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_caldlist.o: ./../../src/cal/caldlist.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_caleaster.o: ./../../src/cal/caleaster.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calelement.o: ./../../src/cal/calelement.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calfile.o: ./../../src/cal/calfile.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calformat.o: ./../../src/cal/calformat.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calformatiso.o: ./../../src/cal/calformatiso.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calformattext.o: ./../../src/cal/calformattext.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calformatunit.o: ./../../src/cal/calformatunit.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calfrench.o: ./../../src/cal/calfrench.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calfunction.o: ./../../src/cal/calfunction.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calgrammar.o: ./../../src/cal/calgrammar.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calgregorian.o: ./../../src/cal/calgregorian.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calhebrew.o: ./../../src/cal/calhebrew.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calislamic.o: ./../../src/cal/calislamic.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calisoordinal.o: ./../../src/cal/calisoordinal.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calisoweek.o: ./../../src/cal/calisoweek.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calhybrid.o: ./../../src/cal/calhybrid.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_caljdn.o: ./../../src/cal/caljdn.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_caljulian.o: ./../../src/cal/caljulian.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calliturgical.o: ./../../src/cal/calliturgical.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calmark.o: ./../../src/cal/calmark.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calmath.o: ./../../src/cal/calmath.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calparse.o: ./../../src/cal/calparse.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calpattern.o: ./../../src/cal/calpattern.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calrecord.o: ./../../src/cal/calrecord.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calregnal.o: ./../../src/cal/calregnal.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calscheme.o: ./../../src/cal/calscheme.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calscript.o: ./../../src/cal/calscript.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calsetmath.o: ./../../src/cal/calsetmath.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calshift.o: ./../../src/cal/calshift.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calstokenstream.o: ./../../src/cal/calstokenstream.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calsvalue.o: ./../../src/cal/calsvalue.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_caltext.o: ./../../src/cal/caltext.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calversion.o: ./../../src/cal/calversion.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

cal_calvocab.o: ./../../src/cal/calvocab.cpp
	$(CXX) -c -o $@ $(CAL_CXXFLAGS) $(CPPDEPS) $<

utf8api_utf8proc.o: ./../../src/utf8/utf8proc.c
	$(CC) -c -o $@ $(UTF8API_CFLAGS) $(CPPDEPS) $<

utf8api_utf8api.o: ./../../src/utf8/utf8api.cpp
	$(CXX) -c -o $@ $(UTF8API_CXXFLAGS) $(CPPDEPS) $<

.PHONY: all install uninstall clean


# Dependencies tracking:
-include ./*.d
