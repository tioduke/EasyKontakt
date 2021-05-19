#include <QTextCodec>
#include "vcarditem.h"

VCardItem::VCardItem(QObject *parent,QStringList *lines, int start) :
    QObject(parent)
{
    mValid = false;
    vCardWidget = NULL;
    errorText = "";
    parse(lines,start);
}
bool VCardItem::hasModified(void){
    return modifiedState;
}
void VCardItem::setModified(void) {
    modifiedState = true;
    emit modified();
}
void VCardItem::setWidget(QWidget *w){
    vCardWidget = w;
}

QWidget *VCardItem::widget(void){
    return vCardWidget;
}

int VCardItem::parse(QStringList *lines, int start) {
    QString whiteSpace = " \t";
    if ( lines ) {
        if ( start > -1) {
            QString line = lines->at(start).trimmed();
            for ( int lineNo = start+1; lineNo < lines->count(); lineNo++ ) {
                //if ( line.indexOf("ENCODING=QUOTED-PRINTABLE") > -1) {
                if (whiteSpace.indexOf(lines->at(lineNo).left(1)) < 0 ) {
                    if ( (line.right(1) == "=") && (line.indexOf("ENCODING=QUOTED-PRINTABLE") > -1) ) {
                       // Zeile endet mit "=" es geht also in der nächsten Zeile weiter
                       line = line.left(line.count()-1)+lines->at(lineNo).trimmed();
                    } else {
                        parseLine(line,lineNo);
                        return lineNo-1;
                    }
                } else {
                    // Zeile beginnt mit Leerzeichen(oder Tab) es geht also in der nächsten Zeile weiter
                    line += lines->at(lineNo).trimmed();
                }
            }
            parseLine(line,start);
            return lines->count();
        }
    }
    return start+1;
}
bool VCardItem::parseLine(QString line, int lineNo){
    if (line.isEmpty() ) {
        return false;
    }
    int pos = line.indexOf(':');
    if ( pos < 0 ) {
        errorText = "Error in line "+QString::number(lineNo)+"->"+line+"<";
        return false;
    }
    keys = line.left(pos).toUpper().trimmed().split(';');
    values = line.mid(pos+1).trimmed().split(';');
    originalKeys = line.left(pos).toUpper().trimmed().split(';');
    originalValues = line.mid(pos+1).trimmed().split(';');
    mValid = true;
    modifiedState = false;
    return true;
}
void VCardItem::undo(void){
    modifiedState = false;
    keys.clear();
    values.clear();
    keys = originalKeys;
    values = originalValues;
    emit modifyUndo();
}

bool VCardItem::valid(void) {
    return mValid;
}
int VCardItem::keyCount(){
    return keys.count();
}

int VCardItem::valueCount(){
    return values.count();
}

bool VCardItem::isKey(QString refKey, int index){
    if ( (index > -1) & (index < keys.count() ) ) {
        return keys[index] == refKey.toUpper();
    }
    return false;
}
QString VCardItem::key(int index){
    if ( (index > -1) & (index < keys.count() ) ) {
        return keys[index];
    }
    return "";
}
void VCardItem::swapValues(int left, int right){
    if ( left < valueCount() ) {
        if ( right < valueCount() ) {
            QString text = values[left];
            values[left] = values[right];
            values[right] = text;
            setModified();
        }
    }
}

QString VCardItem::value(int index){
    return decode(rawValue(index));
}
QString VCardItem::rawValue(int index){
    if ( (index > -1) & (index < values.count() ) ) {
        return values[index];
    }
    return "";
}
QString VCardItem::keyProperty(QString refKey){
    foreach(QString key,keys) {
        if (key.indexOf(refKey) == 0 ) {
            int pos = key.indexOf('=');
            if ( pos > -1 ) {
                return key.mid(pos+1);
            }
            return refKey;
        }
    }
    return "";
}
bool VCardItem::checkNonAsciiChar(void){
    foreach(QString value,values) {
        for ( int i = 0; i < value.size(); i++) {
            if ( value[i] > '~') {
                return true;
            }
        }
    }
    return false;
}
QString VCardItem::textToLineLength(QString value){
    return value;



    QString text = "";
    QString newLine = "";
    int index = 74;
    while ( index < value.size() ) {
        text += value.left(index)+"\r\n ";
        value = value.mid(index);
        index = 73;
        newLine = "\r\n";
    }
    text += value;
    return text+newLine;
}
QString VCardItem::qrCodeData(void){
    QString text = vCardData();
    if ( text.indexOf("BASE64") > -1 ) {
        return ""; //text.replace("\r\n ","");
    }
    return text;
}

QString VCardItem::vCardData(void){
    QString text = "";
    bool hasNonAsciiChar = checkNonAsciiChar();
    if (hasNonAsciiChar ) {
        keys << "CHARSET=UTF-8";
        keys << "ENCODING=QUOTED-PRINTABLE";
        for ( int i = 0; i < values.count(); i++) {
            values[i] = encodeUtf8(values[i]);
        }
    }
    text += keys.join(";");
    text += ':';
    text += values.join(";");
    return textToLineLength(text);
}
QString VCardItem::rawData(void){
    QString text = "";
    text += keys.join(";");
    text += ':';
    text += values.join(";");
    return text;
}

QString VCardItem::decode(QString value){
    if ( value.isEmpty() ) return "";
    QString encoding = keyProperty("ENCODING");
    if ( encoding.isEmpty() ) {
        return value;
    }

//    if ( encoding == "3DQUOTED-PRINTABLE") {
//        encoding = "QUOTED-PRINTABLE";
//        value = value.replace("=3D","=");
//    }
    if ( encoding == "QUOTED-PRINTABLE") {
        // Example: aus "=42=C3=BC=72=6F" wird "Büro"
        bool ok;
        QTextCodec *codec = QTextCodec::codecForName(keyProperty("CHARSET").toLatin1());
        if ( !codec ) {
            codec = QTextCodec::codecForName("UTF-8");
        }
        QByteArray byteList;
        for ( int i = 0; i < value.size(); i++) {
            if ( value[i] != '=') {
                char charValue = value.mid(i++,2).toInt(&ok,16);
                if ( ok ) byteList.append( charValue );
            }
        }
        return codec->toUnicode(byteList);
    } else if ( encoding == "BASE64") {
        data = QByteArray::fromBase64(value.toLatin1());
        if ( keyProperty("JPEG") == "JPEG" ) return "JPEG";
        return "UNKNOWN Base64 Data";
    }
    return "UNKNOWN ENCODING";
}

QString VCardItem::encodeUtf8(QString value){
    // Example: aus "Büro" wird "=42=C3=BC=72=6F"
    QString text = "";
    for ( int i = 0; i < value.size(); i++) {
        QChar ch = value.at(i);
        ushort charValue = ch.unicode();
        if (charValue > 127) {
            ushort utfValue = charValue >> 6;
            utfValue += 0xC0;
            charValue &= 0xBF;
            text += "="+QString::number(utfValue,16);
        }
        text += "="+QString::number(charValue,16);
    }
    return text.toUpper();
}
void VCardItem::setData(QString keyString, QString valueString){
    keys = keyString.toUpper().trimmed().split(';');
    values = valueString.trimmed().split(';');
    // TODO valueString könnte umlaute enthalten
    setModified();
    mValid = true;
}
