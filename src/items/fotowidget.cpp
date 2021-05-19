#include "fotowidget.h"
#include "../func.h"
#include "../defines.h"
#include <QFileDialog>
#include <QBuffer>

FotoWidget::FotoWidget(int row,VCardItemDefines *def,QWidget *parent) :
    VCardWidget(row,def, parent)
{
    QString sizeString = def->item[PARAM_COLUMN];
    bool ok;
    imageSize = sizeString.toInt(&ok);
    if ( !ok ) imageSize = 100;
    setMinimumSize(QSize(imageSize,imageSize));
    setMaximumSize(QSize(imageSize,imageSize));
    setupUI();
    connect(imgButton,SIGNAL(openClicked()),this,SLOT(openPhoto()));
    connect(imgButton,SIGNAL(closeClicked()),this,SLOT(deleteMeLater()));

}

void FotoWidget::setupUI(void){
    setupUIBefore();

    cBWork->setVisible(false);
    cBHome->setVisible(false);
    cBPreferred->setVisible(false);
    lEType->setVisible(false);

    hLayout = new QHBoxLayout(this) ;

    imgButton = new ImageButton(imageSize,imageSize);
    hLayout->addWidget(imgButton);

    tBDelete->setVisible(false);

    //setupUIAfter();
    layout()->setContentsMargins(0, 0, 0, 0);
}
void FotoWidget::save(void){
    if ( modifiedState ) {
        QString base64(data.toBase64());
        saveToFile(QDir::homePath()+"/base64.txt",base64);
        vCardItem->setData("PHOTO;ENCODING=BASE64;"+dataFormat,base64);
    }
    setModified(false);
}

void FotoWidget::setVCardItem(VCardItem *tCardItem){
    VCardWidget::setVCardItem(tCardItem);
    QStringList formatList;
    QString format = "";
    formatList << "JPEG" << "JPG" << "PNG";
    for (int index = 1; index < vCardItem->keyCount(); index++ ) {
        QString key = vCardItem->key(index);
        if (formatList.indexOf(key) > -1) {
            format = key;
        }
    }

    QString encoding = vCardItem->keyProperty("ENCODING");

    if ( encoding.isEmpty() ) {
        encoding = vCardItem->keyProperty("BASE64");
    }
    if (encoding == "BASE64") {
        data = QByteArray::fromBase64(vCardItem->rawValue().toLatin1());
        if (( format == "JPEG") | (format == "JPG") | (format == "PNG") ) {
            QPixmap pixmap;
            pixmap.loadFromData(data);
            imgButton->setPixmap(pixmap);
        }
    }
    setModified(false);
}
void FotoWidget::openPhoto(void){
    QString fileName = QFileDialog::getOpenFileName(this,
               tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if ( !fileName.isEmpty() ) {
        dataFormat = "JPEG";
        QImage image;
        image.load(fileName);
        imgButton->setImage(image);

        QImage imgScaled( image.scaled(QSize(imageSize,imageSize),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        imgScaled.save(&buffer, dataFormat.toLatin1(),imageSize);
        buffer.close();

        setModified();
    }
}
void FotoWidget::saveImgToFile(QImage image){
    QImage img50(image.scaled(QSize(50,50),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    QImage img100(image.scaled(QSize(100,100),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    QByteArray imgData50;
    QBuffer buffer50(&imgData50);
    buffer50.open(QIODevice::WriteOnly);
    img50.save(&buffer50, "JPEG");
    buffer50.close();
    QString img50base64(imgData50.toBase64());

    QByteArray imgData100;
    QBuffer buffer100(&imgData100);
    buffer100.open(QIODevice::WriteOnly);
    img100.save(&buffer100, "JPEG");
    buffer100.close();
    QString img100base64(imgData100.toBase64());

    saveToFile(QDir::homePath()+"/img50base64.txt",img50base64);
    saveToFile(QDir::homePath()+"/img100base64.txt",img100base64);
    img50.save(QDir::homePath()+"/img50.jpg","JPEG");
    img100.save(QDir::homePath()+"/img100.jpg","JPEG");
}

//void FotoWidget::openPhoto(void){
//    QString fileName = QFileDialog::getOpenFileName(this,
//               tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
//    if ( !fileName.isEmpty() ) {
//        dataFormat = "JPEG";
//        QPixmap pixmap;
//        pixmap.load(fileName);
//        pixmap = pixmap.scaled(QSize(s50,s50),Qt::KeepAspectRatio,Qt::SmoothTransformation);
//        QBuffer buffer(&data);
//        buffer.open(QIODevice::WriteOnly);
//        pixmap.save(&buffer, dataFormat.toLatin1(),50);
//        buffer.close();
//        tBOpen->setIcon(QIcon(pixmap));
//        setModified();
//    }
//}
