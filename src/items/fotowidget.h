#ifndef FOTOWIDGET_H
#define FOTOWIDGET_H

#include "vcardwidget.h"
#include "../imagebutton.h"

class FotoWidget : public VCardWidget
{
    Q_OBJECT
public:
    explicit FotoWidget(int row,VCardItemDefines *def,QWidget *parent = 0);
    void setVCardItem(VCardItem *tCardItem);
public slots:
    void save(void);
    void saveImgToFile(QImage image);
private slots:
    void openPhoto(void);
private:
    void setupUI(void);

    QGridLayout *gridLayout;
    QHBoxLayout *hLayout;

    QToolButton *tBOpen;
    QToolButton *tBDeletePhoto;
    ImageButton *imgButton;
    QByteArray data;
    QString dataFormat;
    int imageSize;
};

#endif // FOTOWIDGET_H
