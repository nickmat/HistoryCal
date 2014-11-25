cd win
call createpack.bat
cd ..
copy win\hcal-setup.exe hcal_0-0-1_setup.exe
del win\hcal-setup.exe

cd src
call createsource.bat
cd ..
copy src\hcal.gz hcal_0-0-1_src.gz
del src\hcal.gz
copy src\hcal.zip hcal_0-0-1_src.zip
del src\hcal.zip

pause