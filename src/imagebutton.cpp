#include "imagebutton.h"
#include <QBuffer>

ImageButton::ImageButton(int w, int h,QWidget *parent) :
    QLabel(parent)
{
    imageWidth = 100;
    if ( w ) imageWidth = w;
    imageHeight = 100;
    if ( h ) imageHeight = h;
    setMinimumSize(imageWidth,imageHeight);
    setMaximumSize(imageWidth,imageHeight);
    QPixmap pic;
    pic.load(":/img/img/system-users-sw.png");
    QLabel::setPixmap(pic);
    dataFormat = "JPEG";
    setScaledContents ( true );
    tBClose = new QToolButton(this);
    tBClose->setIcon(QIcon(":/img/img/close.png"));
    tBClose->setIconSize(QSize(24,24));
    tBClose->setGeometry(QRect(imageWidth-30,0,30,30));

    tBOpen = new QToolButton(this);
    tBOpen->setIcon(QIcon(":/img/img/document-open.png"));
    tBOpen->setIconSize(QSize(24,24));
    tBOpen->setGeometry(QRect(0,0,30,30));

    connect(&timer,SIGNAL(timeout()),this,SLOT(ticker()));
    connect(tBClose,SIGNAL(clicked()),this,SIGNAL(closeClicked()));
    connect(tBOpen,SIGNAL(clicked()),this,SIGNAL(openClicked()));
}
void ImageButton::setImage(QImage img){
    setPixmap(QPixmap::fromImage(img));
}
void ImageButton::ticker(void){
    tBClose->setVisible(underMouse());
    tBOpen->setVisible(underMouse());
}

void ImageButton::setPixmap(QPixmap img){
    timer.start(200);
    tBClose->setVisible(false);
    tBOpen->setVisible(false);
    QLabel::setPixmap(img);
    //setText("text");
}
