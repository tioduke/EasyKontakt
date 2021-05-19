#ifndef TELWIDGET_H
#define TELWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QStringList>

#include "../vcard/vcarditem.h"
#include "../ui/selectdialog.h"
#include "vcardwidget.h"


class TelWidget : public VCardWidget
{
    Q_OBJECT
    
public:
    explicit TelWidget(int row,VCardItemDefines *def,QWidget *parent = 0);
    void setVCardItem(VCardItem *tCardItem);
public slots:
    void save(void);
    void toggleMaxView();

private:
    void createParameter(int count);
    void setupUI(void);
    QStringList optionList;
    QStringList translation;
    QString itemType;

    QHBoxLayout *hLayout;
    QHBoxLayout *addLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *hLayoutType;

    QLabelEdit *lEProperty;

    QList<QLineEdit *> editLines;
};

#endif // TELWIDGET_H
