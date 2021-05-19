#include <QApplication>
#include <QTranslator>
#include <QSplashScreen>
#include <QPalette>
#include <QTime>
#include "mainwindow.h"
#include "func.h"
#include "aboutbox.h"
QStringList colorRoles;

QColor colorFromString(QString colorStr) {
    // expects: #RRGGBB
    int n = 2;
    int r;
    int g;
    int b;
    int pos = 1;
    bool ok;
    if ( colorStr[0] == '#') {
        n = (colorStr.size()-1) / 3;
        if ( (n * 3) + 1 != colorStr.size() ) return QColor();
        r = colorStr.mid(pos,n).toInt(&ok,16);
        pos += n;
        g = colorStr.mid(pos,n).toInt(&ok,16);
        pos += n;
        b = colorStr.mid(pos,n).toInt(&ok,16);
        return QColor(r,g,b);
    }
    return QColor();
}

QString setColorPalette(QString style) {
    int startPos = style.indexOf("\nQPalette ",0,Qt::CaseInsensitive);
    if ( startPos > -1 ) {
        int pos = style.indexOf("{",startPos)+1;
        int endPos = style.indexOf("}",pos);
        QStringList colorLines = style.mid(pos,endPos-pos).split(";");
        style = style.left(startPos)+style.mid(endPos+1);
        QPalette pal = QApplication::palette();
        foreach ( QString line, colorLines) {
            QStringList items = line.trimmed().toUpper().split(":");
            if ( items.count() == 2) {
                if ( items[1].trimmed().size() ) {
                    int colorIndex = colorRoles.indexOf(items[0]);
                    if ( colorIndex > -1 ) {
                        pal.setColor( ((QPalette::ColorRole)colorIndex)/*QPalette::Highlight*/, colorFromString(items[1].trimmed()) );
                    }
                }
            }
        }
        QApplication::setPalette(pal);
    }
    return style;
}

int main(int argc, char *argv[])
{
    colorRoles << "WINDOWTEXT" << "BUTTON" << "LIGHT" << "MIDLIGHT" << "DARK" << "MID" << "TEXT" << "BRIGHTTEXT" << "BUTTONTEXT" << "BASE" ;
    colorRoles << "WINDOW" << "SHADOW" << "HIGHLIGHT" << "HIGHLIGHTEDTEXT" << "LINK" << "LINKVISITED" << "ALTERNATEBASE" << "" << "TOOLTIPBASE" << "TOOLTIPTEXT" ;

    QApplication::setDesktopSettingsAware(false);
    QApplication app(argc, argv);

    QString path = findDocPath(app.applicationDirPath());
    QString style = getStyleSheetFile();
    style = setColorPalette(style);
    app.setStyleSheet(style);


    bool forceRun = false;
    int splashTime = 1000;

    QLocale::setDefault(QLocale(QLocale::system().name()));

    QString locale = QLocale::system().name();
    if ( ( argc > 1 ) && (!app.arguments()[1].isEmpty()) ) locale = app.arguments()[1];
    QTranslator *translator = new QTranslator;
    QTranslator *translator_qt = new QTranslator;
    // translations for EasyKontakt
    QString trFile = path+"EasyKontakt_"+locale;
    if (translator->load(trFile)) {
        app.installTranslator(translator);
    }
    // translations for standard dialogs like QFileDialog
    if (translator_qt->load(path+QDir::separator()+"qt_"+locale)) {
        app.installTranslator(translator_qt);
    }


    QTime t;
    t.start();
    showAboutBox();

//    showAboutBox();
    MainWindow w;

    for ( int i = 1; i < argc; i++) {
        QString fileName(argv[i]);
        if ( fileName.right(4).toUpper() == ".VCF") {
            if ( !w.isAlreadyOpen(fileName)) {
                w.addVCardEditor( argv[i] );
            }
        }
    }

    if (forceRun) {
        hideAboutBox();
        w.show();
        return app.exec();

    } else {
        app.processEvents();  // ohne den gehts mit Qt4 aber nicht mit Qt5
        while ( t.elapsed() < splashTime ) ;
        w.show();
        hideAboutBox();

        return app.exec();
    }
}
