#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QImage>

class ImageWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ImageWidget(QString fileName,QString header,QString text,QWidget *parent = 0);
    ~ImageWidget();
signals:
    
public slots:

private:
    QVBoxLayout *vLayout;
    QLabel *lImage;
    QLabel *lHeader;
    QLabel *lText;
    QToolButton *tBClose;

};

#endif // IMAGEWIDGET_H
