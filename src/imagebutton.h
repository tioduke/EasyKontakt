#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QTimer>
#include <QToolButton>


class ImageButton : public QLabel
{
    Q_OBJECT
public:
    explicit ImageButton(int w = 0, int h = 0,QWidget *parent = 0);
    void setImage(QImage img);
    void setPixmap(QPixmap img);

signals:
    void closeClicked(void);
    void openClicked(void);
public slots:
    void ticker(void);
private:
    QImage image;
    QByteArray data;
    QString dataFormat;
    int imageWidth;
    int imageHeight;
    QToolButton *tBClose;
    QToolButton *tBOpen;
    QTimer timer;
};

#endif // IMAGEBUTTON_H
