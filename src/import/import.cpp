#include "import.h"

Import::Import(QObject *parent) :
    QObject(parent)
{
}
void Import::read(QString /*fileName*/){
}

QString Import::getGivenName(QString name){
    int pos = name.indexOf(",");
    if ( pos > -1 ) {
        return name.mid(pos+1);
    }
    pos = name.indexOf(" ");
    return name.left(pos);
}

QString Import::getFamilyName(QString name){
    int pos = name.indexOf(",");
    if ( pos > -1 ) {
        return name.left(pos);
    }
    pos = name.indexOf(" ");
    if ( pos > -1 ) {
        return name.mid(pos+1);
    }
    return "";
}

void Import::newVCard(void){
    vCardText = "";
}
QString readLine(QString text, QString suchText){
    int start = text.indexOf("\n"+suchText);
    if ( start < 0 ) return "";
    start = text.indexOf(":",start);
    int end = text.indexOf("\r\n",start);
    return text.mid(start+1,(end-start));
}

void Import::saveVCard(void){
    QString mailText = readLine("\n"+vCardText,"EMAIL");
    QString fnNameText = readLine("\n"+vCardText,"FN");
    if ( fnNameText.isEmpty() ) {
        fnNameText = mailText;
        vCardText = "FN:"+fnNameText+"\r\n"+vCardText;
    }
    QString nameText = readLine("\n"+vCardText,"N");
    if ( nameText.isEmpty() ) {
        vCardText = "N:;"+fnNameText+";;;\r\n"+vCardText;
    }
    QString text = "BEGIN:VCARD\r\n";
    text+= "VERSION:2.1\r\n";
    text += vCardText;
    text += "END:VCARD\r\n";
    emit addVCard(text,false);
}
void Import::newItem(QString key){
    keys = key;
    values = "";
}
void Import::saveItem(void){
    vCardText += keys+":"+values+"\r\n";
}
void Import::saveItem(QStringList keyList, QStringList valueList){
    QString separator = "";
    QString line = "";
    bool isEmpty = true;
    foreach(QString key,keyList) {
        line += separator+key;
        separator = ";";
    }
    line += ":";
    separator = "";
    foreach(QString value,valueList) {
        if ( value.size() ) isEmpty = false;
        line += separator+value;
        separator = ";";
    }
    if ( !isEmpty ) {
        vCardText += line+"\r\n";
    }
}

void Import::addKey(QString key){
    if ( key.isEmpty() ) {
        return;
    }
    if ( keys.size() ) {
        keys += ";";
    }
    keys += key.trimmed();
}

void Import::addValue(QString value){
    if ( value.isEmpty() ) {
        return;
    }
    if ( values.size() ) {
        values += ";";
    }
    values += value.trimmed();
}
