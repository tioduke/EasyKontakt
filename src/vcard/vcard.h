#ifndef VCARD_H
#define VCARD_H

#include <QObject>
#include <QStringList>

#include "vcarditem.h"

class VCard : public QObject
{
    Q_OBJECT
public:
    explicit VCard(QObject *parent = 0, QStringList *lines = NULL, int start = -1);
    ~VCard();
    void setChecked(bool state);
    bool isChecked(void);
    int parse(QStringList *lines, int start);
    bool valid(void);
    QString value(QString key, int keyIndex = 0,int valueIndex = 0);
    VCardItem *vCardItem(QString key, int *startIndex);
    VCardItem *vCardItem(QString key);
    VCardItem *vCardItem(int index);
    void deleteVCardItem(VCardItem *item);
    VCardItem *addVCardItem(QString keyString,QString valueString = "");
    QString text(void);
    void setText(QString newText);
    QString qrCodeText(void);
    bool hasModified(void);
    void undo(void);
signals:
    
public slots:

private:
    bool mValid;
    QList<VCardItem *> vCardItems;
    bool checkState;
    QString originalText;
    int originalItems;
};

#endif // VCARD_H
