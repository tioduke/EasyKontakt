@echo off

REM 2015-09-23
REM
REM build actions for this application
REM

set APPNAME=%1
set SOURCEDIR=%2

copy bin\example.vcf ..\%APPNAME%
copy bin\VcardItems.conf ..\%APPNAME%
copy bin\EasyKontakt.conf ..\%APPNAME%
