#include "importcsv.h"
#include <QStringList>

ImportCSV::ImportCSV(QObject *parent) :
    Import(parent)
{
}
int ImportCSV::posInList(QStringList *list,QString text){
    for ( int index = 0; index < list->count(); index++) {
        if ( list->at(index).indexOf(text) > -1 ) {
            return index;
        }
    }
    return -1;
}
void ImportCSV::createVCard(QStringList list){
    bool ok;
    newVCard();
    QStringList values;
    QStringList keys;
    for ( int index = 0; index < list.count(); index++) {
        bool saveIt = false;
        QStringList listItems = list[index].split('=');
        if (( listItems.count() > 2) & ( listItems[0].size() > 2) & ( listItems[0].left(2) != "//")) {
            keys = listItems[1].split(':');
            if (( keys[0] == "N" ) | ( keys[0] == "ADR" )) {
                int n = keys[1].toInt(&ok);
                if ( ok ) {
                    while ( values.count() < (n+1) ) values << "";
                    values[n] = listItems[2];
                    keys.takeAt(1);
                }
                if ( keys[keys.count()-1] == "END") {
                    keys.takeLast();
                    saveIt = true;
                }
            } else {
                values << listItems[2];
                saveIt = true;
            }
        }
        if ( saveIt ) {
            saveItem(keys,values);
            keys.clear();
            values.clear();
        }
    }
    saveVCard();
}

void ImportCSV::read(QString fileName){
    QString def = loadFromFile(getDocPath()+"/thunderbird.def");
    if ( def.isEmpty()) {
        return;
    }
    QString text = loadFromFile(fileName/*,"ISO 8859-1"*/);
    if ( text.isEmpty() ) {
        return;
    }
    QStringList lines = text.split("\n");
    QChar separator = ' ';
    int n = lines[0].count(' ');
    int nn = lines[0].count(',');
    if (nn > n ) {
        separator = ',';
        n = nn;
    }
    nn = lines[0].count(';');
    if (nn > n ) {
        separator = ';';
        n = nn;
    }
    nn = lines[0].count('\t');
    if (nn > n ) {
        separator = '\t';
        n = nn;
    }
    QStringList headerList = lines[0].split(separator);
    for ( int line = 1; line < lines.count(); line++) {
        // a defList item is: Vorname=N:GIVENNAME see thunderbird.def
        QStringList defList = def.split("\n");
        QStringList dataList =  lines[line].split(separator);
        if ( dataList.count() > 1 ) {
            while (dataList.count() > headerList.count() ) headerList << "";
            for ( int index = 0; index < dataList.count(); index++) {
                QString value = dataList[index];
                QString key = headerList[index];
                int pos = posInList(&defList,key);
                if ( pos > -1){
                    // a defList item is now: Vorname=N:GIVENNAME=Tom
                    defList[pos] += "="+value;
                }
            }
            createVCard(defList);
        }
    }
}
