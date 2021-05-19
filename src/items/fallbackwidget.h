#ifndef FALLBACKWIDGET_H
#define FALLBACKWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QStringList>

#include "../vcard/vcarditem.h"
#include "../ui/selectdialog.h"
#include "vcardwidget.h"

class FallBackWidget : public VCardWidget
{
public:
    FallBackWidget(int row,VCardItemDefines *def,QWidget *parent = 0);
    ~FallBackWidget();
    void setVCardItem(VCardItem *tCardItem);
    void setDefaultValues(void);
public slots:
    void save(void);
    void toggleMaxView();

private:
    void setupUI(void);

    QStringList optionList;
    QStringList translation;
    QString itemType;

    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    QGridLayout *gridLayout;

//    QGridLayout *gridLayout;

    QList<QLabelEdit *> lEList;

//    QLabelEdit *lEPoBox;

//    QSpacerItem *hSpacer;

};

#endif // FALLBACKWIDGET_H
