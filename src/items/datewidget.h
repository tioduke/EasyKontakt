#ifndef DATEWIDGET_H
#define DATEWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QDateEdit>
#include <QLabel>
#include <QStringList>
#include <QCalendarWidget>

#include "../vcard/vcarditem.h"
#include "../ui/selectdialog.h"
#include "vcardwidget.h"

class DateWidget : public VCardWidget
{
    Q_OBJECT
public:
    explicit DateWidget(int row,VCardItemDefines *def,QWidget *parent = 0);
    void setVCardItem(VCardItem *tCardItem);
public slots:
    void save(void);
    void toggleMaxView();
private slots:
    void updateEventEnd(QDateTime dateTime);
private:
    void setupUI(void);
    QStringList optionList;
    QStringList translation;
    QString itemType;

    QHBoxLayout *hLayout;
    //QHBoxLayout *addLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *hLayoutType;

    QDateTimeEdit *currentDateTimeEdit;
    QLabelEdit *lEEvent;
    QLabelEdit *lEEventEnd;

    QList<QLineEdit *> editLines;
};

#endif // DATEWIDGET_H
