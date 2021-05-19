#ifndef IMPORTFRITZ_H
#define IMPORTFRITZ_H
#include <QString>
#include <QXmlStreamReader>
#include <QTextCodec>
#include "import.h"

class ImportFritz : public Import
{
    Q_OBJECT
public:
    explicit ImportFritz(QObject *parent = 0);
    void read(QString fileName);

signals:
    
public slots:

    
};

#endif // IMPORTFRITZ_H
