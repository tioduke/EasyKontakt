@echo off
REM 2015-09-23
REM
REM Compile and make selfextracting installfile for easy....
REM

REM
REM set path !!! Please change this !!!
REM

REM set PROGRAMFILES=C:\Program Files
set path=%path%;C:\Qt\Tools\mingw730_32\bin;C:\Qt\5.14.1\mingw73_32\bin
REM set path=%path%;%PROGRAMFILES%\mingw-w64\i686-5.2.0-posix-dwarf-rt_v4-rev0\mingw32\bin

REM
REM find sourcecode directory and application name
REM

for /f "delims=" %%a in ("%~dp0.") do set "SOURCEDIR=%%~nxa"
for /f "delims=-" %%a in ("%SOURCEDIR%") do set "APPNAME=%%a"

REM
REM build application
REM

qmake -config release
mingw32-make

REM
REM copy needed files to ../APPNAME
REM

md ..\%APPNAME%
del /S /Q ..\%APPNAME%\*
copy ..\dll\*.* ..\%APPNAME%
copy ..\dll\platforms\*.* ..\%APPNAME%\platforms
copy bin\%APPNAME%.exe ..\%APPNAME%
copy bin\%APPNAME%.ico ..\%APPNAME%
xcopy bin\help\*  ..\%APPNAME%\help /i /e /d /y
copy bin\help\%APPNAME%Win.css  ..\%APPNAME%\help\%APPNAME%.css

REM 
REM copy additional files
REM

call %APPNAME%.bat %APPNAME% %SOURCEDIR%

REM
REM build package
REM

bzr revno > tmp.txt
set /p oldrevision= < tmp.txt
set /a revision=%oldrevision%+1
"%PROGRAMFILES%\7-Zip\7zG.exe" a -t7z -sfx ..\%SOURCEDIR:-=_%-%revision%.exe ..\%APPNAME%
del tmp.txt
