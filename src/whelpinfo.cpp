/* 	This file is part of EasyWebitor,
    an open-source cross-platform web pages editor tool

        Copyright (c) :[2008-2013] Werner Sigrist
        Contact e-mail: Werner Sigrist <sig@sigvdr.de>
        Program URL   : http://www.sigvdr.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "whelpinfo.h"
#include "func.h"
#include <QFile>
#include <QTextStream>
#include <QLocale>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDesktopServices>
#include <QFileDialog>
#include <QPrinter>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QWebFrame>

QString helpFile = "";

QString findHelpFile(QString path){
	QString locale = QLocale::system().name();
	QStringList args = qApp->arguments();
	if ( args.count() > 1 ) {
		locale = args[1];
	}
	QString fileName = "EasyWebitor_";
	helpFile = fileName+locale+".html";
    if ( ! QFile::exists(path+"/"/*QDir::separator()*/+helpFile) ) {
		locale = locale.left(locale.indexOf("_"));
		helpFile = fileName+locale+".html";
        if ( ! QFile::exists(path+"/"/*QDir::separator()*/+helpFile) ) {
			helpFile = fileName+".html";
            if ( ! QFile::exists(path+"/"/*QDir::separator()*/+helpFile) ) {
				helpFile = "";
			}
		}
	}
	return helpFile;
}

QString WHelpInfo::getHelpFile(void){
	if ( helpFile.isEmpty() ) {
		findHelpFile(getDocPath());
	}
	return helpFile;
}


WHelpInfo::WHelpInfo( QWidget * parent) 
	: QWidget(parent)
{
	setupUi(this);
    setVisible(false);
    setup();
    baseUrl = "/";
//    init();
}
//
void WHelpInfo::setup(void){
	connect( pBHome, SIGNAL(clicked()), this,  SLOT(helpShowHome()) );
	connect( pBRefresh, SIGNAL(clicked()), tBrowser,  SLOT(reload()) );
	connect( pBBack, SIGNAL(clicked()), tBrowser,  SLOT(back()) );
	connect( pBNext, SIGNAL(clicked()), tBrowser,  SLOT(forward()) );
    connect( pBPrint, SIGNAL(clicked()), this,  SLOT(loadPage()) );

    connect( lEUrl, SIGNAL(returnPressed()), this,  SLOT(loadPage()) );

    connect( tBrowser, SIGNAL(loadFinished(bool)), this,  SLOT(enableHistory(bool)) );
    connect( tBrowser, SIGNAL(linkClicked(QUrl)), this,  SLOT(changeUrl(QUrl)) );
    connect( tBrowser->page(), SIGNAL(linkHovered(QString,QString, QString)),this,  SLOT(linkIsHovered(QString,QString,QString)) );


    connect( tBrowser->page(), SIGNAL(downloadRequested (const QNetworkRequest &)), this, SLOT(downloadRequested (const QNetworkRequest & )));
    tBrowser->page()->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);
    //tBrowser->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}
void WHelpInfo::linkIsHovered(QString link, QString /*title*/, QString /*textContent*/){
    lUrl->setText("Url: "+link);
}
void WHelpInfo::downloadRequested (const  QNetworkRequest &networkRequest ){
    lUrl->setText(networkRequest.url().toString());
}

void WHelpInfo::enableHistory(bool /*doIt*/){
	pBBack->setEnabled(tBrowser->history()->canGoBack());
	pBNext->setEnabled(tBrowser->history()->canGoForward());
	lEUrl->setText(tBrowser->url().toString());
    if ( !isReloaded ) {
        // Änderungen an den CSS Dateien werden ohne reload() nicht aktualisiert
        // Fehler im QWebView?? (4.8)
        // also: setUrl(neue Seite) // warten bis geladen // noch mal laden (reload())
        isReloaded = true;
        tBrowser->reload();
    }
    //tBrowser->page()->setContentEditable(true);
}

void WHelpInfo::init(QString homeDocument){
    baseUrl = getDocPath();

    helpPaths << baseUrl+"/";
    helpFile = "index.html";
    if ( homeDocument.size() )
        helpFile = homeDocument;
    load(baseUrl+"/"+helpFile);


//    helpShow();
}
void WHelpInfo::loadPage(void){
    load(lEUrl->text());
}
void WHelpInfo::changeUrl(QUrl url){
    isReloaded = false;
    tBrowser->setUrl(url);
    tBrowser->show();
}

void WHelpInfo::load(QString url){
    isReloaded = false;
    if ( url.left(4) == "http" ) {
        changeUrl(QUrl(url));
    } else if ( url.left(3) == "www" ) {
        changeUrl(QUrl("http://"+url));
    } else {
#ifdef QT5V
        changeUrl(QUrl("file://"+baseUrl+"/"+url)); // geändert wg. Qt5.1
#else
        changeUrl(QUrl(baseUrl+"/"+url)); // alt Qt4
#endif
    }
}
void WHelpInfo::loadAndShow(QString url){
    load(url);
    show();
}
void WHelpInfo::setBaseUrl(QString url){
    baseUrl = url;
}


void WHelpInfo::helpShowHome(){
    tBrowser->load(QUrl(baseUrl+"/"+helpFile));
}

void WHelpInfo::linkTo(QUrl link){
	QString linkStr = link.toString();  
	if (linkStr.left(7) == "http://") {
		QDesktopServices::openUrl(link);
		return;
	}
	int pos = linkStr.indexOf("internal:");
	if ( pos > -1 ) {
//		tBrowser->scrollToAnchor(linkStr.mid(pos+9));
		return;
	}
	pos = linkStr.indexOf("#");
	if ( pos > -1 ) {
//		tBrowser->scrollToAnchor(linkStr.mid(pos+1));
		return;
	}
//	tBrowser->scrollToAnchor(linkStr);	
}

void WHelpInfo::helpPrint(void){
	QPrinter printer;
	printer.setOutputFileName(QDir::homePath()+"/test.pdf");
	printer.setOrientation(QPrinter::Landscape);
	tBrowser->print(&printer);
}
