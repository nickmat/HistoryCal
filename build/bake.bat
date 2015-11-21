bakefile -f gnu -I %WXWIN%\build\bakefiles\wxpresets -D BUILD_DIR=gnu_eu -D EXCLUDE_UTIL=1 -o gnu_eu/GNUmakefile hcal.bkl

bakefile -f gnu -I %WXWIN%\build\bakefiles\wxpresets -D BUILD_DIR=gnu -D EXCLUDE_UTIL=0 -o gnu/GNUmakefile hcal.bkl

bakefile -f msvs2008prj -I %WXWIN%\build\bakefiles\wxpresets -D BUILD_DIR=vc9 -D EXCLUDE_UTIL=0 -o vc9/hcal hcal.bkl

pause