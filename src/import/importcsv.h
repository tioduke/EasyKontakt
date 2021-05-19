#ifndef IMPORTCSV_H
#define IMPORTCSV_H
#include "import.h"

class ImportCSV : public Import
{
    Q_OBJECT
public:
    explicit ImportCSV(QObject *parent = 0);
    void read(QString fileName);

signals:
    
public slots:

private:
    int posInList(QStringList *list,QString text);
    void createVCard(QStringList list);
};

#endif // IMPORTCSV_H
