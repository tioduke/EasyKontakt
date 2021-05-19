#include "vcard.h"

VCard::VCard(QObject *parent, QStringList *lines, int start) :
    QObject(parent)
{
    mValid = false;
    parse(lines,start);
}
int VCard::parse(QStringList *lines, int start){
    if ( lines ) {
        if ( start > -1) {
            for ( int lineNo = start; lineNo < lines->count(); lineNo++ ) {
                int pos;
                QString lineAsUpper = lines->at(lineNo).toUpper();
                if ( (pos = lineAsUpper.indexOf("BEGIN")) > -1) {
                    if ( (lineAsUpper.indexOf("VCARD")) > pos) {
                        return lineNo-1;
                    }
                }
                if ( (pos = lineAsUpper.indexOf("END")) > -1) {
                    if ( (lineAsUpper.indexOf("VCARD")) > pos) {
                        mValid = true;
                        originalText = text();
                        originalItems = vCardItems.count();
                        return lineNo;
                    }
                }
                VCardItem *vCardItem = new VCardItem(this);
                lineNo = vCardItem->parse(lines,lineNo);
                if ( vCardItem->valid() ) {
                    vCardItems << vCardItem;
                } else {
                    delete vCardItem;
                }
            }
        }
    }
    return -1; // Error
}
VCard::~VCard(){
    while ( vCardItems.count() ) {
        delete vCardItems.takeLast();
    }
}
void VCard::undo(void){
    setText(originalText);
}
bool VCard::hasModified(void) {
    foreach(VCardItem *vCardItem,vCardItems) {
        if (vCardItem->hasModified()) {
            return true;
        }
    }
    return (originalItems != vCardItems.count());
}

bool VCard::valid(void) {
    return mValid;
}
QString VCard::value(QString key, int keyIndex,int valueIndex){
    foreach( VCardItem *item, vCardItems) {
        if ( item->isKey(key,keyIndex)) {
            return item->value(valueIndex).trimmed();
        }
    }
    return "";
}
VCardItem *VCard::vCardItem(int index){
    if (( index > -1)& (index < vCardItems.count() )) {
        return vCardItems[index];
    }
    return NULL;
}

VCardItem *VCard::vCardItem(QString key){
    int index = 0;
    return vCardItem(key, &index);
}

VCardItem *VCard::vCardItem(QString key, int *index){
    if ( *index > -1) {
        for ( (*index)++ ; *index < vCardItems.count(); (*index)++) {
            VCardItem *item = vCardItems[*index];
            if (item->isKey(key) ) {
                return item;
            }
        }
    }
    return NULL;
}
void VCard::deleteVCardItem(VCardItem *item){
    int index = vCardItems.indexOf(item);
    if ( index > -1) {
        delete vCardItems.takeAt(index);
    }
}
VCardItem *VCard::addVCardItem(QString keyString,QString valueString){
    VCardItem *vCardItem = new VCardItem(this);
    vCardItem->setData(keyString,valueString);
    if ( vCardItem->valid() ) {
        vCardItems << vCardItem;
        return vCardItem;
    }
    return NULL;
}
void VCard::setText(QString newText){
    mValid = false;
    while ( vCardItems.count()) {
        delete vCardItems.takeLast();
    }
    QStringList textList = newText.split("\n");
    int start = ( textList[0].indexOf("VCARD") > -1 ) ? 1 : 0;
    parse(&textList,start);
}

QString VCard::text(void){
    QString text;
    text += "BEGIN:VCARD\r\n";
    int itemIndex = 0;
    VCardItem *item;
    while ( (item = vCardItem(itemIndex++)) ) {
        text += item->vCardData()+"\r\n";
    }
    text += "END:VCARD\r\n";
    return text;
}
QString VCard::qrCodeText(void){
    QString text;
    text += "BEGIN:VCARD\r\n";
    int itemIndex = 0;
    VCardItem *item;
    while ( (item = vCardItem(itemIndex++)) ) {
        QString line = item->qrCodeData();
        if ( !line.isEmpty() ) {
            text += line+"\r\n";
        }
    }
    text += "END:VCARD\r\n";
    return text;
}
void VCard::setChecked(bool state){
    checkState = state;
}

bool VCard::isChecked(void){
    return checkState;
}
