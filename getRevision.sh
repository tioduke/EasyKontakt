#!/bin/sh
#  Speichert den Ausgabestand des Projekts (Bazaar Revisionsnummer) in src/version.h
#  unter Verwendung der Datei "progversion.txt"
#  in der Form: static QString VERSION = "0.5-30";
#  Der Ausgabestand wird um 1 erh�ht
if [ -f "$(which git)" ];then
	# Bazaar ist installiert, es kann losgegen
	VERSION=$(git describe --abbrev=0 --tags)
	NEW='static QString VERSION = "'$VERSION'";'
#	echo $VERSION
	OLD=$(cat src/version.h)
	if [ "$NEW" != "$OLD" ]; then
		echo "$NEW" > src/version.h
		echo "$VERSION" > progversion.txt
		cat src/version.h
	else
		echo "nNothing to be done for '$VERSION'!"
	fi
#	echo $OLD
fi
