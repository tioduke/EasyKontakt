#!/bin/sh
#  Speichert den Ausgabestand des Projekts (Bazaar Revisionsnummer) in src/version.h 
#  unter Verwendung der Datei "progversion.txt" 
#  in der Form: static QString VERSION = "0.5-30";
#  Der Ausgabestand wird um 1 erhöht
if [ -f "$(which bzr)" ];then
	# Bazaar ist installiert, es kann losgegen
	STRING=$(cat progversion.txt)
	VERSION=$(($(bzr revno)+1))
	NEW='static QString VERSION = "'$STRING$VERSION'";'
#	echo $VERSION
	OLD=$(cat src/version.h)
	if [ "$NEW" != "$OLD" ]; then
		echo "$NEW" > src/version.h
		if [ -f "$(which dpkg-buildpackage)" ];then
			#echo "Debianpaket bauen"
			#   Für das Debianpaket wird die Versionsnummer auch in debian/changelog gespeichert
			DEB_NAME=$(basename $(pwd) | cut -d- -f1)
			BIN_NAME=$(grep TARGET *.pro | cut -d= -f2 | tr -d ' ')
			echo "$DEB_NAME ($STRING$VERSION) unstable; urgency=low\n" > debian/changelog
			echo "  * Changelog " >> debian/changelog
			echo "  see www.sigvdr.de/$BIN_NAME\n" >> debian/changelog
			#bzr log  --log-format=gnu-changelog  >> debian/changelog
			#bzr log | grep -e revno -e "  "  >> debian/changelog
			echo " -- Sig <sig@sigvdr.de>  $(date -R)\n" >> debian/changelog
			#echo " -- sig <sig@sigvdr.de>  Thu, 02 Aug 2012 18:19:28 +0200" >> debian/changelog
		fi
		cat src/version.h
	else
		echo "nNothing to be done for 'VERSION'!"
	fi
#	echo $OLD
fi
