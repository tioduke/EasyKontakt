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
#ifndef WHELPINFO_H
#define WHELPINFO_H
//
#include "ui_helpinfo.h"
#include <QDialog>
#include <QTextEdit>
#include <QWebView>
#include <QWebPage>
#include <QWebHistory>
#include <QPushButton>
#include <QGroupBox>

QString findHelpFile(QString path);
QString getHelpFile(void);


//
class WHelpInfo : public QWidget, public Ui::dlgHelpInfo
{
Q_OBJECT
public:
	WHelpInfo( QWidget * parent = 0 );
	void helpShow();
	void load(QString url);
    void loadAndShow(QString url);
    void setBaseUrl(QString url);
    void init(QString homeDocument = "");
private slots:
	void enableHistory(bool doIt);
	void helpPrint(void);
	void helpShowHome(void);
    void linkIsHovered(QString link, QString title, QString textContent);
    void downloadRequested ( const  QNetworkRequest &networkRequest );
    void loadPage(void);
    void changeUrl(QUrl url);
public slots:
	void linkTo(QUrl link);
private:
    QString getHelpFile(void);
    QString baseUrl;
	QPushButton *pBSave;
	void setup(void);
	QString helpFile;
	QStringList helpPaths;
    bool isReloaded;
};
#endif
