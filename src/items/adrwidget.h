#ifndef ADRWIDGET_H
#define ADRWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QStringList>

#include "../vcard/vcarditem.h"
#include "../ui/selectdialog.h"
#include "vcardwidget.h"

class AdrWidget : public VCardWidget
{
    Q_OBJECT
public:
    explicit AdrWidget(int row,VCardItemDefines *def,QWidget *parent = 0);
    void setVCardItem(VCardItem *tCardItem);
public slots:
    void save(void);
    void toggleMaxView();

private:
    void setupUI(void);

    QStringList optionList;
    QStringList translation;
    QString itemType;

    QHBoxLayout *hLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *hLayoutStreetPoBox;
    QHBoxLayout *hLayoutLocality;

    QLabelEdit *lEPoBox;
    QLabelEdit *lEExtAddress;
    QLabelEdit *lEStreet;
    QLabelEdit *lELocality;
    QLabelEdit *lERegion;
    QLabelEdit *lEPostalCode;
    QLabelEdit *lECountry;

    QSpacerItem *hSpacer;


};

#endif // ADRWIDGET_H
