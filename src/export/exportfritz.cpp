#include <QDateTime>
#include "exportfritz.h"
#include "../defines.h"

ExportFritz::ExportFritz(QObject *parent) :
    Export(parent)
{
    mSortFamilyName = false;
}
void ExportFritz::setExchangeMode(bool sortFamilyName){
    mSortFamilyName = sortFamilyName;
}
QString ExportFritz::intro(void){
    contactCount = 0;
    QString text = "";
    text += "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n";
    text += "<phonebooks>\n";
    text += "  <phonebook>\n";
    return text;
}

QString ExportFritz::contactIntro(void){
    contactCount++;
    telText.clear();
    emailText = "";
    return "    <contact><category>0</category>";
    // category 1 für Wichtige Person -> anruf auch bei Klingelsperre
}

QString ExportFritz::contactOutro(void){
    QString text = "";
    if ( telText.count() ) {
        text += "<telephony nid=\"";
        text += QString::number(telText.count());
        text += "\">";
        foreach (QString tel, telText) {
            text += tel;
        }
        text += "</telephony>";
        if ( emailText.isEmpty() ) {
            text += "<services/>";  // hier EMail
        } else {
            text += "<services nid=\"1\">";
            text += emailText;
            text += "</services>";
        }
        text += "<setup/>";
        text += "<modtime>"+QString::number(QDateTime::currentDateTime().toTime_t())+"</modtime>";
        text += "<uniqueid>"+QString::number(contactCount)+"</uniqueid>";
    }
    text += "</contact>\n";
    return text;
}

QString ExportFritz::outro(void){
    QString text = "";
    text += "  </phonebook>\n";
    text += "</phonebooks>\n";
    return text;
}
QString ExportFritz::itemText( VCardItem *item){
    QString text = "";
    QString key = item->key();
    if ( key == "FN") {
    } else if ( key == "VERSION") {
    } else if ( key == "N") {
        text += "<person><realName>";
        if ( mSortFamilyName ) {
            text += xmlEncoding(item->value(VCardNameFamily)+" "+item->value(VCardNameGiven)+" "+item->value(VCardNameAdditional));
        } else {
            text += xmlEncoding(item->value(VCardNameGiven)+" "+item->value(VCardNameAdditional)+" "+item->value(VCardNameFamily));
        }
        text += "</realName></person>";
    } else if ( key == "TEL") {
        telText << createTel(item);
    } else if ( key == "EMAIL") {
        emailText = createEmail(item,emailText);
    } else {
        //text += "\n<undefined>"+item->rawData()+"</undefined>\n";
    }
    return text;
}
QString ExportFritz::createTel(VCardItem *item){
    QString preferred = "0";
    QString typeStr = "";
    //OptionList=USER DEFINED;VOICE;CELL;FAX;VIDEO;PAGER;TEXT;TEXTPHONE

    QStringList vcardRefStr({"USER DEFINED","VOICE","CELL","FAX","VIDEO","PAGER","TEXT","TEXTPHONE"});
    QStringList fritzRefStr({"sonstige","home","mobile","fax","sonstige","sonstige","sonstige","sonstige"});
    QString tel = "<number";
    bool isWorkNumber = false;
    int index = 1;
    QString key = item->key(index++).toUpper();
    while ( key.size() ) {
        key = key.toUpper();
        if ( key == "WORK") {
            isWorkNumber = true;
        }
        int keyIndex = vcardRefStr.indexOf(key);
        if ( keyIndex > -1) {
            typeStr = " type=\""+fritzRefStr[keyIndex]+"\"";
        } else if ( key == "pref") {
            preferred = "1";
        }
//        if ( key != "home") {
//           key += "xxx";
//        }
        key = item->key(index++).toLower();
    }
    // in VCARD kann eine Telefonnummer zwei Eigenschaften z.B. MOBILE und WORK haben
    // Fritzbox kennt nur eine Eigenschaft, so dass hier die Eigenschaft "work" nur dann verwendet wird,
    // wenn dafür keine andere Eigenschaft wie z.B. "mobile" oder "fax" verwendet wird
    if ( isWorkNumber) {
        if ( typeStr.indexOf("home")> -1 ) {
            typeStr = " type=\"work\"";
        }
    }
    // 01.04.2019
    if ( typeStr.isEmpty() ) {
       typeStr = " type=\"home\"";
    }

    tel += typeStr;
    tel += " prio=\""+preferred+"\"";
    index = 1;
    QString value = item->value(index++);
    while ( key.size() ) {
        if ( value.indexOf("quickdial") > -1) {
            tel += " "+value;
        } else if ( value.indexOf("vanity") > -1) {
            tel += " "+value;
        }
        value = item->value(index++);
    }
    tel += " id=\""+QString::number(telText.count())+"\">";
    tel += item->value();
    tel += "</number>";
    return tel;
}
QString ExportFritz::createEmail(VCardItem *item,QString oldText){
    // Fritz.Box kennt nur eine EMail Adresse -> wir nehmen deshalb die bevorzugte EMail
    QString preferred = "0";

    int index = 1;
    QString key = item->key(index++).toLower();
    while ( key.size() ) {
        if ( key == "pref") {
            preferred = "1";
        }
        key = item->key(index++).toLower();
    }
    if ( ( preferred == "1") | oldText.isEmpty() ) {
        QString email = "<email classifier=\"private\" id=\"0\">";
        email += item->value(0);
        email += "</email>";
        return email;
    }
    return oldText;
}
QString ExportFritz::codecName(void) const{
    return "ISO 8859-1";
}
QString ExportFritz::xmlEncoding(QString text){
    text = text.replace("<","&lt;");
    text = text.replace(">","&gt;");
    text = text.replace("&","&amp;");
    text = text.replace("\"","&quot;");
    text = text.replace("'","&apos;");
    return text;
}
