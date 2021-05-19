#ifndef EXPORT_H
#define EXPORT_H

#include <QObject>
#include "../vcard/vcarditem.h"

class Export : public QObject
{
    Q_OBJECT
public:
    explicit Export(QObject *parent = 0);
    virtual QString intro(void);
    virtual QString contactIntro(void);
    virtual QString contactOutro(void);
    virtual QString outro(void);
    virtual QString itemText( VCardItem *item);
    virtual QString codecName(void) const;
signals:
    
public slots:
    
};

#endif // EXPORT_H
