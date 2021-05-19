#include "help.h"
#include "ui_help.h"

Help::Help(QString /*path*/, QWidget *parent) : QWidget(parent), ui(new Ui::Help) {
    ui->setupUi(this);
    vLayout = ui->vHelpLayout;
    tBPrevious = ui->tBPrevious;
    tBNext = ui->tBNext;
    tBHome = ui->tBHome;
    tBPrevious->setEnabled(false);
    tBNext->setEnabled(false);
    ui->dummy->setVisible(false);
}

Help::~Help()
{
    delete ui;
}
void Help::loadAndShow(QString newUrl){
    load(newUrl);
    show();
}
void Help::load(QString /*newUrl*/) {
}
void Help::showUrl(QUrl url){
    showUrl(url.url());
}
void Help::showUrl(QString url){
    ui->lEUrl->setText(url);
}
void Help::enableNextButton(bool state){
    ui->tBNext->setEnabled(state);

}
void Help::enablePreviousButton(bool state){
    ui->tBPrevious->setEnabled(state);
 }


#ifdef QT_TEXTBROWSER
HelpBrowser::HelpBrowser(QString path, QWidget *parent) : Help(path, parent) {
    tBrowser = new QTextBrowser();
    vLayout->addWidget(tBrowser);
    tBrowser->setSearchPaths(QStringList() << path );
    connect(tBrowser,SIGNAL(anchorClicked(QUrl)),this,SLOT(showUrl(QUrl)));
    connect(tBrowser,SIGNAL(backwardAvailable(bool)),this,SLOT(enablePreviousButton(bool)));
    connect(tBrowser,SIGNAL(forwardAvailable(bool)),this,SLOT(enableNextButton(bool)));
    connect(tBNext,SIGNAL(clicked()),tBrowser,SLOT(forward()));
    connect(tBPrevious,SIGNAL(clicked()),tBrowser,SLOT(backward()));
    connect(tBHome,SIGNAL(clicked()),tBrowser,SLOT(home()));
    setWindowTitle("TextBrowser");
}

void HelpBrowser::load(QString newUrl) {
    tBrowser->setSource(newUrl);
}
#endif // QT_TEXTBROWSER

#ifdef QT_WEBKITWIDGETS
HelpBrowser::HelpBrowser(QString path, QWidget *parent) : Help(path, parent) {
    tBrowser = new QWebView();
    vLayout->addWidget(tBrowser);

    connect( tBHome, SIGNAL(clicked()), this,  SLOT(home()) );
    connect( tBPrevious, SIGNAL(clicked()), tBrowser,  SLOT(back()) );
    connect( tBNext, SIGNAL(clicked()), tBrowser,  SLOT(forward()) );
    connect( tBrowser, SIGNAL(loadFinished(bool)), this,  SLOT(enableHistory(bool)) );
    connect( tBrowser, SIGNAL(linkClicked(QUrl)), this,  SLOT(load(QUrl)) );

    baseUrl = path;
    helpHome = "";
    setWindowTitle("WebKitWidgets");
}

void HelpBrowser::home(void){
    load(helpHome);
}

void HelpBrowser::load(QString newUrl) {
    if ( helpHome.isEmpty()) {
        helpHome = newUrl;
    }
    QUrl url = QUrl::fromLocalFile(baseUrl+newUrl);
    load(url);
}
void HelpBrowser::load(QUrl url) {
    tBrowser->setUrl(url);
    tBrowser->show();
}

void HelpBrowser::enableHistory(bool /*state*/){
    enablePreviousButton(tBrowser->history()->canGoBack());
    enableNextButton(tBrowser->history()->canGoForward());
    showUrl(tBrowser->url());
}

#endif // QT_WEBENGINEKITWIDGETS

#ifdef QT_WEBENGINE
HelpBrowser::HelpBrowser(QString path, QWidget *parent) : Help(path, parent) {
    tBrowser = new QWebEngineView();
    vLayout->addWidget(tBrowser);

    connect( tBHome, SIGNAL(clicked()), this,  SLOT(home()) );
    connect( tBPrevious, SIGNAL(clicked()), tBrowser,  SLOT(back()) );
    connect( tBNext, SIGNAL(clicked()), tBrowser,  SLOT(forward()) );
    connect( tBrowser, SIGNAL(loadFinished(bool)), this,  SLOT(enableHistory(bool)) );
//    connect( tBrowser, SIGNAL(linkClicked(QUrl)), this,  SLOT(load(QUrl)) );

    baseUrl = path;
    helpHome = "";
    setWindowTitle("WebEngine");
}

void HelpBrowser::home(void){
    load(helpHome);
}

void HelpBrowser::load(QString newUrl) {
    if ( helpHome.isEmpty()) {
        helpHome = newUrl;
    }
    QUrl url = QUrl::fromLocalFile(baseUrl+newUrl);
    load(url);
}
void HelpBrowser::load(QUrl url) {
    tBrowser->setUrl(url);
    tBrowser->show();
}


void HelpBrowser::enableHistory(bool /*state*/){
    enablePreviousButton(tBrowser->history()->canGoBack());
    enableNextButton(tBrowser->history()->canGoForward());
    showUrl(tBrowser->url());
}

#endif // QT_WEBENGINE
