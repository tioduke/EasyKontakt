#include "export.h"

Export::Export(QObject *parent) :
    QObject(parent)
{
}
QString Export::intro(void){
    return "";
}

QString Export::contactIntro(void){
    return "BEGIN:VCARD\r\n";
}

QString Export::contactOutro(void){
    return "END:VCARD\r\n";
}

QString Export::outro(void){
    return "";
}
QString Export::itemText( VCardItem *item){
    return item->vCardData()+"\r\n";
}
QString Export::codecName(void) const{
    return "";
}
