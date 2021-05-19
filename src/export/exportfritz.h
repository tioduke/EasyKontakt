#ifndef EXPORTFRITZBOX_H
#define EXPORTFRITZBOX_H

#include "export.h"

class ExportFritz : public Export
{
    Q_OBJECT
public:
    explicit ExportFritz(QObject *parent = 0);
    virtual QString intro(void);
    virtual QString contactIntro(void);
    virtual QString contactOutro(void);
    virtual QString outro(void);
    QString itemText( VCardItem *item);
    virtual QString codecName(void) const;
    void setExchangeMode(bool sortFamilyName);
signals:
    
public slots:
private:
    QString xmlEncoding(QString text);
    QString createTel(VCardItem *item);
    QString createEmail(VCardItem *item,QString oldText);
    QStringList telText;
    QString emailText;
    int contactCount;
    bool mSortFamilyName;
};

#endif // EXPORTFRITZBOX_H
