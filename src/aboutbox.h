#ifndef ABOUTBOX_H
#define ABOUTBOX_H
//
#include <QSplashScreen>
#include <QString>

//
class AboutBox : public QSplashScreen
{
public:
    AboutBox(const QPixmap &pixmap = QPixmap(),Qt::WindowFlags f = 0);
    void paintEvent(QPaintEvent *);
    void setTitle(QString title);
    QString title(void);
private:
    QString splashTitle;
    QPixmap logo;
};

void showAboutBox(void);
void hideAboutBox(void);
void finishAboutBox(QWidget *w);
void refreshAboutBox(void);
//static QString splashTitle = "NONE";
#endif
