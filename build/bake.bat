bakefile -f gnu -I %WXWIN%\build\bakefiles\wxpresets -D BUILD_DIR=gnu -o gnu/GNUmakefile hcal.bkl

bakefile -f msvs2008prj -I %WXWIN%\build\bakefiles\wxpresets -D BUILD_DIR=vc9 -o vc9/hcal hcal.bkl

pause