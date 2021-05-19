#!/bin/sh
NAME=$(basename $(pwd))
VERSION=$(echo ${NAME} | cut -d- -f2)
FILETOREMOVE="../$(echo ${NAME} | cut -d- -f1)*.deb"
PACKAGENAME="$(echo ${NAME} | cut -d- -f1)_$(echo ${NAME} | cut -d- -f2)"
BUILD="${PACKAGENAME}-$(($(bzr revno)+1))"
echo "$NAME $BUILD $PACKAGENAME $VERSION "
rm ${FILETOREMOVE}
/usr/bin/qmake -unix -o Makefile *.pro
make clean
tar -czvf "../${BUILD}.tar.gz" * -X .bzrignore
dpkg-buildpackage -rfakeroot -b
#PACKAGEBASEVERSION=$(basename "$(dir ../${PACKAGENAME}*.deb)")
#ARCHITEKTUR="$(echo ${PACKAGEBASEVERSION} | cut -d_ -f3)"
#mv "../${PACKAGEBASEVERSION}" "../${PACKAGENAME}-$(bzr revno)_${ARCHITEKTUR}"
echo "build done"
dir ../*.deb


