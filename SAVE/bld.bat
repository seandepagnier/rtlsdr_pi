REM FE2  Testplugin

REM  For Opencpn 5.6.2 and wxWidgets-3.1.2
REM cd build
REM cmake -T v141_xp -G -DOCPN_TARGET=MSVC "Visual Studio 15 2017"
REM       -DCMAKE_BUILD_TYPE=RelWithDebInfo  .. 
REM cmake --build . --target tarball --config RelWithDebInfo >output.txt
REM bash ./cloudsmith-upload.sh

REM  For Opencpn 5.6.2 and wxWidgets-3.1.2
REM cd build
REM cmake -T v141_xp -DOCPN_TARGET=MSVC ..
REM cmake --build . --target package --config release >output.txt
REM bash ./cloudsmith-upload.sh

REM  For Opencpn 5.7.1 and wxWidgets-3.2.1
REM .\buildwin\win_deps.bat
REM cd build
REM cmake -T v143 -A Win32 -DOCPN_TARGET=MSVC "Visual Studio 17 2022" ^
REM      -DCMAKE_BUILD_TYPE=RelWithDebInfo  ..
REM cmake --build . --target tarball --config RelWithDebInfo >output.txt

REM  Setup for wxWidgets 3.2.2
REM  wxWidgets_INCLUDE_DIRS   %WXWIN%\include
REM  wxWidgets_LIB_DIR   %WXWIN%\lib\vc_dll
REM  wxWidgets_LIBRARIES    %WXWIN%\lib\vc_dll
REM  wxWidgets_ROOT_DIR   %WXWIN%
REM  WXWIN   C:\Users\fcgle\source\repos2\OpenCPN\cache\buildwxWidgets\

REM configdev58.bat
REM Can use  "call configdev58.bat"
REM set "wxDIR=C:\Users\fcgle\source\repos2\OpenCPN\buildwin\..\cache\buildwxWidgets" 
REM set "wxWidgets_ROOT_DIR=C:\Users\fcgle\source\repos2\OpenCPN\buildwin\..\cache\buildwxWidgets" 
REM set "wxWidgets_LIB_DIR=C:\Users\fcgle\source\repos2\OpenCPN\buildwin\..\cache\buildwxWidgets\lib\vc_dll" 

REM  Copied built wxWidgets-3.2.2 to C:\Users\fcgle\source\wxWidgets-3.2.2
set "wxDIR=%WXWIN%
set "wxWidgets_ROOT_DIR=%WXWIN% 
set "wxWidgets_LIB_DIR=%WXWIN%\lib\vc14x_dll" 
set "WXWIN=C:\Users\fcgle\source\wxWidgets-3.2.2"

REM  For Opencpn 5.8 and wxWidgets-3.2.2


cd build
cmake -T v143 -A Win32 -DOCPN_TARGET=MSVC ..
cmake --build . --target package --config relwithdebinfo >output.txt
bash ./cloudsmith-upload.sh
