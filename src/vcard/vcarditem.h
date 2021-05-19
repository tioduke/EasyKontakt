#ifndef VCARDITEM_H
#define VCARDITEM_H

#include <QObject>
#include <QStringList>
//#include "../items/vcardwidget.h"

class VCardItem : public QObject
{
    Q_OBJECT
public:
    explicit VCardItem(QObject *parent = 0,QStringList *lines = NULL, int start = -1);
    int parse(QStringList *lines, int start);
    bool valid(void);
    int keyCount();
    int valueCount();

    bool isKey(QString refKey,int index = 0);
    void swapValues(int left, int right);
    QString keyProperty(QString refKey);
    QString key(int index = 0);
    QString value(int index = 0);
    QString rawValue(int index = 0);
    bool hasModified(void);
    void setData(QString keyString, QString valueString);
    QString vCardData(void);
    QString qrCodeData(void);
    QString rawData(void);
    void setWidget(QWidget *w);
    QWidget *widget(void);
signals:
    void modified(void);
    void modifyUndo(void);
public slots:
    void setModified(void);
    void undo(void);

private:
    QString textToLineLength(QString value);
    bool parseLine(QString line, int lineNo);
    QString decode(QString value);
    QString encodeUtf8(QString value);
    bool checkNonAsciiChar(void);


    QString errorText;
    QStringList keys;
    QStringList values;
    QStringList originalKeys;
    QStringList originalValues;
    bool mValid;
    bool modifiedState;
    QByteArray data;
    QWidget *vCardWidget;
};

#endif // VCARDITEM_H
