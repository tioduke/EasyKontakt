@echo off
REM 2015-09-23
REM 2015-10-06 Schreibe version.h nur wenn sich die Version �ndert
REM  Speichert den Ausgabestand des Projekts (Bazaar Revisionsnummer) in src/version.h
REM  unter Verwendung der Datei "progversion.txt"
REM  in der Form: static QString VERSION = "0.5.30";
REM  Der Ausgabestand wird um 1 erh�ht

git describe --abbrev=0 --tags > progversion.txt
set /p progversion= < progversion.txt

set /p version= < src\version.h
rem echo "1%version%"
set neueversion=static QString VERSION = "%progversion%";
rem echo "2%neueversion%"

IF "%version%" == "%neueversion%" (
  rem echo "do not change version.h"
) ELSE (
   echo "update version to "%progversion%"
   @echo %neueversion%> src\version.h
)

del tmp.txt
