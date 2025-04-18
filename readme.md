# HistoryCal v0.1.0

![Glich logo](./images/logo2/logo2.png)

Welcome to the latest release of HistoryCal, an ambitious multi-platform
historical date and calendar calculator.

For more details and links to a compiled version for Windows (10 and 11) and the source code, please visit the website at http://historycal.org

To build the the program on Windows with Visual Studio 2022,
the following should work:-
```
git clone --recurse-submodules https://github.com/nickmat/HistoryCal.git
cd HistoryCal\build
.\cmake-vc-working.bat
vc-working\hcal.sln
```
If all goes well, this will open Visual Studio 2022 (or later)
with all the source files, including third-party ones,
available to build the HistoryCal program under the project name "hcal".
