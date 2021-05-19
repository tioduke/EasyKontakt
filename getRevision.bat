@echo off
REM 2015-09-23
REM 2015-10-06 Schreibe version.h nur wenn sich die Version ändert
REM  Speichert den Ausgabestand des Projekts (Bazaar Revisionsnummer) in src/version.h 
REM  unter Verwendung der Datei "progversion.txt" 
REM  in der Form: static QString VERSION = "0.5.30";
REM  Der Ausgabestand wird um 1 erhöht

bzr revno > tmp.txt
set /p oldrevision= < tmp.txt
set /p progversion= < progversion.txt
set /a revision=%oldrevision%+1

set /p version= < src\version.h
rem echo "1%version%"
set neueversion=static QString VERSION = "%progversion%%revision%";
rem echo "2%neueversion%"

IF "%version%" == "%neueversion%" ( 
  rem echo "do not change version.h"
) ELSE (
   echo "update version to "%progversion%%revision%"
   @echo %neueversion%> src\version.h
)

del tmp.txt
