@echo off
echo Package source files

if exist hcal rmdir /S /Q hcal
if exist hcal.zip del hcal.zip
if exist hcal.gz del hcal.gz

mkdir hcal
mkdir hcal\build
mkdir hcal\build\gnu
mkdir hcal\build\vc9
mkdir hcal\docs
mkdir hcal\example
mkdir hcal\include
mkdir hcal\include\cal
mkdir hcal\include\utf8
mkdir hcal\include\win
mkdir hcal\scripts
mkdir hcal\src
mkdir hcal\src\cal
mkdir hcal\src\hcal
mkdir hcal\src\hcs
mkdir hcal\src\utf8
mkdir hcal\test
mkdir hcal\test\hctest
mkdir hcal\test\scripts
mkdir hcal\test\scripts\test
mkdir hcal\test\soak
mkdir hcal\test\unit
mkdir hcal\util
mkdir hcal\util\file2cpp

copy ..\..\readme.txt hcal\readme.txt > nul
copy ..\..\license.txt hcal\license.txt > nul
copy ..\..\.gitignore hcal\.gitignore > nul
copy ..\..\build\build.txt hcal\build > nul
copy ..\..\build\gnu\*.* hcal\build\gnu > nul
copy ..\..\build\vc9\*.sln hcal\build\vc9 > nul
copy ..\..\build\vc9\*.vcproj hcal\build\vc9 > nul

rem ==================[ Docs ]======================
copy ..\..\docs\*.* hcal\docs > nul

rem ================[ Examples ]====================
copy ..\..\example\*.* hcal\example > nul

rem =================[ Source ]=====================
rem copy ..\..\include\*.* hcal\include > nul
copy ..\..\include\cal\*.* hcal\include\cal > nul
copy ..\..\include\utf8\*.* hcal\include\utf8 > nul
copy ..\..\include\win\*.* hcal\include\win > nul
copy ..\..\src\dummy.cpp hcal\src > nul
copy ..\..\src\cal\*.* hcal\src\cal > nul
copy ..\..\src\hcal\*.* hcal\src\hcal > nul
copy ..\..\src\hcs\*.* hcal\src\hcs > nul
copy ..\..\src\utf8\*.* hcal\src\utf8 > nul
copy ..\..\test\hctest\*.* hcal\test\hctest > nul
copy ..\..\test\scripts\*.* hcal\test\scripts > nul
copy ..\..\test\scripts\test\*.* hcal\test\scripts > nul
copy ..\..\test\soak\*.* hcal\test\soak > nul
copy ..\..\test\unit\*.* hcal\test\unit > nul
copy ..\..\util\file2cpp\*.* hcal\util\file2cpp > nul

rem =================[ Scripts ]=====================
copy ..\..\scripts\*.* hcal\scripts > nul

7za a -r hcal.zip hcal > ziplist.txt
dtou -v -I hcal -E .;c;cpp;css;f2c;fbp;h;hcs;htm;txt;xpm > convert-rpt.txt
7za a -r hcal.tar hcal > nul
7za a -r hcal.gz hcal.tar > nul

del hcal.tar
rmdir /S /Q hcal


