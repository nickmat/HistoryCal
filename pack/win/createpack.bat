rem Create the windows installer package.

copy ..\..\build\vc9\Release\hcal.exe
copy ..\..\build\vc9\Release\cal.dll
copy ..\..\build\vc9\Release\utf8api.dll
copy ..\..\docs\cal-lib-changes.txt
copy ..\..\docs\hcal-changes.txt
copy ..\..\example\*.hcs
"%ProgramFiles(x86)%\NSIS\makensis.exe" hcal.nsi

del hcal.exe
del cal.dll
del utf8api.dll
del *.hcs
del cal-lib-changes.txt
del hcal-changes.txt
