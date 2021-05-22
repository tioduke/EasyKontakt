#-------------------------------------------------
#
# Project created by QtCreator 2013-08-20T17:39:29
#
#-------------------------------------------------
#
# Last Update: 2020-02-01
#
#-------------------------------------------------

#packagesExist(webkit) {
#    QT += webkit
#    DEFINES += QT_WEBKIT
#    DEFINES -= QT_TEXTBROWSER
#}

#packagesExist(webkitwidgets) {
#    QT += webkitwidgets
#    DEFINES += QT_WEBKITWIDGETS
#    DEFINES -= QT_TEXTBROWSER
#}

#packagesExist(webenginewidgets) {
#    QT += webenginewidgets
#    DEFINES += QT_WEBENGINE_XX
#    DEFINES -= QT_TEXTBROWSER
#}

win32 {
    CONFIG += WIN_OS
}

win64 {
    CONFIG += WIN_OS
}




QT      += core gui network

isEqual(QT_MAJOR_VERSION, 4) {
    DEFINES += QT4V
    OBJECTS_DIR = buildQt4
    MOC_DIR = buildQt4
    UI_DIR = buildQt4
    defines += QT_TEXTBROWSER
}
isEqual(QT_MAJOR_VERSION, 5) {
    DEFINES += QT5V
    QT      += widgets printsupport
    OBJECTS_DIR = buildQt5
    MOC_DIR = buildQt5
    UI_DIR = buildQt5
    lessThan(QT_MINOR_VERSION,6) {
        DEFINES += QT_WEBKITWIDGETS
        QT += webkitwidgets
    } else {
        WIN_OS {
            DEFINES += QT_TEXTBROWSER
        } else {
            DEFINES += QT_WEBENGINE
            QT += webenginewidgets
        }
    }
}

RESOURCES += \
    src/ui/images.qrc \
    src/help/help.qrc

unix {
        system(./getRevision.sh)
}
WIN_OS {
        system(getRevision.bat)
        RC_FILE = easykontakt_resource.rc
}


TARGET = EasyKontakt
TEMPLATE = app

DESTDIR = bin

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/aboutbox.cpp \
    src/func.cpp \
    src/ui/selectdialog.cpp \
    src/vcard/vcardlist.cpp \
    src/vcard/vcarditem.cpp \
    src/vcard/vcard.cpp \
    src/vcard/vcardeditor.cpp \
    src/items/telwidget.cpp \
    src/items/vcardwidget.cpp \
    src/items/namewidget.cpp \
    src/items/fotowidget.cpp \
    src/items/imagewidget.cpp \
    src/items/adrwidget.cpp \
    src/items/qlabeledit.cpp \
    src/import/import.cpp \
    src/import/importfritz.cpp \
    src/import/importcsv.cpp \
    src/ui/checklist.cpp \
    src/stringgridmodel.cpp \
    src/items/datewidget.cpp \
    src/items/notewidget.cpp \
    src/export/export.cpp \
    src/export/exportfritz.cpp \
    src/imagebutton.cpp \
    src/items/fallbackwidget.cpp \
    src/help/help.cpp

HEADERS  += src/mainwindow.h \
    src/aboutbox.h \
    src/defines.h \
    src/func.h \
    src/version.h \
    src/ui/selectdialog.h \
    src/vcard/vcardlist.h \
    src/vcard/vcarditem.h \
    src/vcard/vcard.h \
    src/vcard/vcardeditor.h \
    src/items/vcardwidget.h \
    src/items/telwidget.h \
    src/items/namewidget.h \
    src/items/fotowidget.h \
    src/items/imagewidget.h \
    src/items/adrwidget.h \
    src/items/qlabeledit.h \
    src/import/import.h \
    src/import/importfritz.h \
    src/import/importcsv.h \
    src/ui/checklist.h \
    src/stringgridmodel.h \
    src/items/datewidget.h \
    src/items/notewidget.h \
    src/export/export.h \
    src/export/exportfritz.h \
    src/imagebutton.h \
    src/items/fallbackwidget.h \
    src/help/help.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/selectdialog.ui \
    src/ui/helpinfo.ui \
    src/ui/vcardeditor.ui \
    src/ui/checklist.ui \
    src/help/help.ui

OTHER_FILES += \
    other/help/EasyKontakt.css \
    other/help/thunderbird.def \
    other/help/VCardItems.conf \
    other/VCardItems.conf \
    other/EasyKontakt.css


# compile the translation files

TRANSLATIONS = \
    i18n/EasyKontakt_de_DE.ts \
    i18n/EasyKontakt_en_US.ts

TRANSLATION_TARGET_DIR = bin
TRANSLATIONS_OUT = \
    $$TRANSLATION_TARGET_DIR/EasyKontakt_de_DE.qm \
    $$TRANSLATION_TARGET_DIR/EasyKontakt_en_US.qm

isEmpty(QMAKE_LRELEASE) {
    win32:LANGREL = $$[QT_INSTALL_BINS]\lrelease.exe
    else:LANGREL = $$[QT_INSTALL_BINS]/lrelease
}
langrel.input = TRANSLATIONS
langrel.output = $$TRANSLATION_TARGET_DIR/${QMAKE_FILE_BASE}.qm
langrel.commands = \
    $$LANGREL -compress -nounfinished -removeidentical ${QMAKE_FILE_IN} \
          -qm $$TRANSLATION_TARGET_DIR/${QMAKE_FILE_BASE}.qm
langrel.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += langrel
PRE_TARGETDEPS += $$TRANSLATIONS_OUT

translations.files = $$TRANSLATIONS_OUT
translations.CONFIG = no_check_exist


unix:!macx {
 conffiles.path += /usr/share/EasyKontakt/
 helpfiles.path += /usr/share/EasyKontakt/
 logofiles.path += /usr/share/EasyKontakt/logos/
 desktop.path +=  /usr/share/applications
 icon.path +=  /usr/share/pixmaps
 target.path +=  /usr/bin/
 translations.path += /usr/share/EasyKontakt/
 helpfiles.files +=  other/help/*
 conffiles.files +=  other/VCardItems.conf
 helpfiles.files +=  other/example.vcf
 logofiles.files += other/logos/*
 desktop.files +=  other/EasyKontakt.desktop
 icon.files +=  other/*.png
 INSTALLS +=  target  conffiles helpfiles  desktop  icon logofiles translations
}

DISTFILES +=
