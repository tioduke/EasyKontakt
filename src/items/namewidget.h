#ifndef NAMEWIDGET_H
#define NAMEWIDGET_H

#include "../defines.h"

#include "vcardwidget.h"
#include <QString>
#include <QStringList>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>


class NameWidget : public VCardWidget
{
    Q_OBJECT
public:
    explicit NameWidget(int row,VCardItemDefines *def,QWidget *parent = 0);
    void setVCardItem(VCardItem *tCardItem);
public slots:
    void save(void);
    void swapName(void);


private:
    void setupUI(void);

    QHBoxLayout *hLayout;

    QLabelEdit *lENameGiven;
    QLabelEdit *lENameAdditional;
    QLabelEdit *lENameFamily;
    QLabelEdit *lENamePrefix;
    QLabelEdit *lENameSuffix;
    QToolButton *tBSwap;

};

#endif // NAMEWIDGET_H
