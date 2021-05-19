#include <QDate>
#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include "aboutbox.h"
#include "version.h"
#include "func.h"

void drawRotatedText(QPainter *painter, float degrees, int x, int y, const QString &text) {
    painter->save();
    painter->translate(x,y);
    painter->rotate(degrees);
    painter->setPen(QColor(255,255,255));
    painter->setFont(QFont(painter->font().family(),22/*,QFont::Bold*/));
    painter->drawText(QRect(0,0,240,33),Qt::AlignHCenter|Qt::AlignVCenter,text);
    painter->restore();
}

AboutBox::AboutBox(const QPixmap &pixmap,Qt::WindowFlags f ):QSplashScreen(pixmap,f) {
    splashTitle = "unknown title";
    QString iconName = ":/img/img/"+appName().toLower()+".png";
    logo.load(iconName);
    qApp->setWindowIcon(QIcon(iconName));
}

void AboutBox::paintEvent(QPaintEvent *){
    // Splash screen size 258 x 408
    QPainter p(this);
    int rand = -6;
    int lh = logo.height()+rand;
    p.drawPixmap(rand,height()-lh,logo);
    drawRotatedText(&p,-90,364,249,splashTitle);
}
void AboutBox::setTitle(QString title){
    splashTitle = title;
}
QString AboutBox::title(void){
    return splashTitle;
}


AboutBox *aboutBox;
QPushButton *button;
QString Message = "";

void showAboutBox(void){
    QString title = appName();
    if ( ! aboutBox ) {
        QPixmap pixmap(":/img/img/splash_screen2.png");
        aboutBox = new AboutBox(pixmap, Qt::WindowStaysOnTopHint);
        button = new QPushButton("X",aboutBox);
        button->setObjectName("noResizeCloseButton");
        button->setGeometry(385, 0, 22, 22);
    } else {
        button->setVisible(true);
        aboutBox->connect( button, SIGNAL(clicked()), aboutBox,  SLOT(close()) );
    }
    if ( !title.isEmpty() ) {
        aboutBox->setTitle(title);
    }

    QStringList monthNames;
    monthNames << ""<< "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
    QString dateStr = __DATE__;
    QString month = QString::number(monthNames.indexOf(dateStr.left(3)));
    QString year = dateStr.right(4);
    QString day = dateStr.mid(4,2).trimmed();
    QDate date(QDate::fromString(month+" "+day+" "+year,"M d yyyy"));
    Message = "";
    Message += "<div  style=\"color: #000000;";
//    Message += "background-color: #aaaaaa;";
//    Message += "top: 0px;";
//    Message += "bottom: 0px;";
    Message += "\">";
    Message += "(c) 2012-2013 Werner Sigrist<br>";
    Message += "GNU General Public License v3<br>";
    Message += "www.sigvdr.de<br>";
    Message += "sig@sigvdr.de";

    Message += "<h2 style=\"color: #ffffff;\">"+aboutBox->title()+" "+VERSION+"</h2>";

    Message += "Build with<br>";

#ifdef __GNUC__
    Message += "GCC ";
    Message += __VERSION__;
    Message += " - ";
#endif
    Message += "QT ";
    Message += QT_VERSION_STR;
    Message += "<br>";
    Message += __TIME__;
    Message += "<br>";
    Message += date.toString(Qt::SystemLocaleLongDate)+"<br>";
    Message += "</div>";

//    Message = "";
//    Message += "<div  style=\"color: #000000;\"><br>(c) 2012-2013 Werner Sigrist<br>";
//    Message += "GNU General Public License v3<br>";
//    Message += "www.sigvdr.de<br>";
//    Message += "sig@sigvdr.de</div>";

//    Message += "<h2 style=\"color: #ffffff;\">"+aboutBox->title()+" "+VERSION+"</h2>";

//    Message += "<div style=\"color: #000000;\">";
//    Message += "<br><br>Build with<br>";

//#ifdef __GNUC__
//    Message += "GCC ";
//    Message += __VERSION__;
//    Message += " - ";
//#endif
//    Message += "QT ";
//    Message += QT_VERSION_STR;
//    Message += "<br>";
//    Message += __TIME__;
//    Message += "<br>";
//    Message += date.toString(Qt::SystemLocaleLongDate)+"<br>";
//    Message += "</div>";
    aboutBox->show();
    aboutBox->showMessage(Message,Qt::AlignCenter);
}
void refreshAboutBox(void){
    aboutBox->showMessage(Message,Qt::AlignCenter);
}

void hideAboutBox(void){
    aboutBox->close();
    //	delete splash;
}
void finishAboutBox(QWidget *w){
    aboutBox->finish(w);
}
//
