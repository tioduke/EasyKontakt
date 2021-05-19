#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QToolButton>
namespace Ui {
class Help;
}

class Help : public QWidget
{
    Q_OBJECT

public:
    explicit Help(QString path = "", QWidget *parent = 0);
//    virtual void init(QString path);
    void loadAndShow(QString newUrl);
    ~Help();
public slots:
    void enableNextButton(bool state);
    void enablePreviousButton(bool state);
    void showUrl(QUrl url);
    void showUrl(QString url);
private slots:
    virtual void load(QString newUrl);
protected:
    Ui::Help *ui;
    QVBoxLayout *vLayout;
    QToolButton *tBNext;
    QToolButton *tBPrevious;
    QToolButton *tBHome;
};

#ifdef QT_TEXTBROWSER
#include <QTextBrowser>
class HelpBrowser : public Help
{
    Q_OBJECT
public:
    HelpBrowser(QString path = "", QWidget *parent = 0);
private slots:
    void load(QString newUrl);
private:
    QTextBrowser *tBrowser;
};
#endif // QT_TEXTBROWSER

#ifdef QT_WEBKITWIDGETS
#include <QWebView>
#include <QWebHistory>

class HelpBrowser : public Help
{
    Q_OBJECT
public:
    HelpBrowser(QString path = "", QWidget *parent = 0);
private slots:
    void load(QString newUrl);
    void load(QUrl url);
    void enableHistory(bool state = false);
    void home(void);
private:
    QWebView *tBrowser;
    QString baseUrl;
    QString helpHome;
};
#endif // QT_WEBKITWIDGETS

#ifdef QT_WEBENGINE
#include <QWebEngineView>
#include <QWebEngineHistory>

class HelpBrowser : public Help
{
    Q_OBJECT
public:
    HelpBrowser(QString path = "", QWidget *parent = 0);
private slots:
    void load(QString newUrl);
    void load(QUrl url);
    void enableHistory(bool state = false);
    void home(void);
private:
    QWebEngineView *tBrowser;
    QString baseUrl;
    QString helpHome;
};
#endif // QT_WEBENGINE

#endif // HELP_H
