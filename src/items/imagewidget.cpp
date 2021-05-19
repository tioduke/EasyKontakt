#include "imagewidget.h"

ImageWidget::ImageWidget(QString fileName,QString header,QString text = "", QWidget *parent) :
    QDialog(parent)
{
    vLayout = new QVBoxLayout(this);
    if ( !header.isEmpty() ) {
        lHeader = new QLabel(this);
        lHeader->setText(header);
        vLayout->addWidget(lHeader);
    }
    lImage = new QLabel(this);
    vLayout->addWidget(lImage);

    if ( !text.isEmpty() ) {
        lText = new QLabel(this);
        lText->setText(text);
        vLayout->addWidget(lText);
    }

    tBClose = new QToolButton(this);
    tBClose->setText(tr("close"));
    vLayout->addWidget(tBClose);

    lImage->setPixmap(QPixmap(fileName));
    connect(tBClose,SIGNAL(clicked()),this,SLOT(close()));
    setMinimumWidth(200);
}
ImageWidget::~ImageWidget(){
    delete tBClose;
    delete lImage;
    delete vLayout;
}
