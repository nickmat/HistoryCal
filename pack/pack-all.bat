set hcalver=0-0-5
cd win
call createpack.bat
cd ..
copy win\hcal-setup.exe hcal_%hcalver%_setup.exe
del win\hcal-setup.exe

cd src
call createsource.bat
cd ..
copy src\hcal.gz hcal_%hcalver%_src.gz
del src\hcal.gz
copy src\hcal.zip hcal_%hcalver%_src.zip
del src\hcal.zip

pause