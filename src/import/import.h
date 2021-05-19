#ifndef IMPORT_H
#define IMPORT_H

#include <QObject>
#include "../func.h"
#include "../vcard/vcard.h"

class Import : public QObject
{
    Q_OBJECT
public:
    explicit Import(QObject *parent = 0);
    virtual void read(QString fileName);
    //virtual void read(QString text);
    
signals:
    void addVCard(QString text,bool goToNewLine);
//    void addVCard(VCard *vCard);
//    void addItem(QString key,QString value);
    
public slots:

protected:
    void newVCard(void);
    void saveVCard(void);
    void newItem(QString key);
    void saveItem(void);
    void saveItem(QStringList keyList, QStringList valueList);
    void addKey(QString key);
    void addValue(QString value);
    QString getGivenName(QString name);
    QString getFamilyName(QString name);

    QString vCardText;
    VCard *vCard;
    QString keys;
    QString values;
};

#endif // IMPORT_H
