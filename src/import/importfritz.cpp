#include "importfritz.h"


//<contact>
//    <category>0</category>
//    <person><realName>Niepel</realName></person>
//    <telephony nid="1">
//        <number type="mobile" quickdial="9" vanity="" prio="1" id="0">017669966</number>
//    </telephony>
//    <services/>
//    <setup/>
//    <mod_time>1329992692</mod_time>
//    <uniqueid>24</uniqueid>
//</contact>
//<contact>
//     <category>1</category>                                                                                   //////// Fritzbox Anzeige: Wichtig
//     <person><realName>Sigrist, Werner</realName></person>
//     <telephony nid="2">
//         <number type="home" prio="1" id="0" quickdial="1" vanity="889911">0733553355</number>
//         <number type="mobile" prio="0" id="1">015788558855</number>
//     </telephony>
//     <services nid="1">
//         <email classifier="private" id="0">sig@sig.de</email>
//     </services>
//     <setup/>
//     <mod_time>1378711978</mod_time>
//     <uniqueid>19</uniqueid>
//</contact>
ImportFritz::ImportFritz(QObject *parent) :
    Import(parent)
{
}
void ImportFritz::read(QString fileName){
    QString text = loadFromFile(fileName,"ISO 8859-1");
    QXmlStreamReader reader(text.toLatin1());
    while (!reader.atEnd()) {
        reader.readNext();
        QString key = reader.name().toString();
        //QString value = reader.text().toString();

        if (reader.isStartElement()) {
            if (key == "contact") {
                newVCard();

            //} else if ( key == "person") {
            } else if ( key == "realName") {
                newItem("FN");
                QString value = reader.readElementText();
                QString givenName = getGivenName(value);
                QString familyName = getFamilyName(value);
                addValue(givenName+" "+familyName);
                saveItem();
                newItem("N");
                addValue(familyName+";"+givenName+";;;");
                saveItem();

//            } else if ( key == "telephony") {
//                newItem("TEL");
            } else if ( key == "number") {
                newItem("TEL");
                QString quickdial = reader.attributes().value("quickdial").toString();
                QString vanity = reader.attributes().value("vanity").toString();
                addKey(reader.attributes().value("type").toString().toUpper());
                if (reader.attributes().value("prio").toString() == "1") {
                    addKey("PREF");
                }
                QString value = reader.readElementText();
                addValue(value);
                addValue("quickdial="+quickdial+",vanity="+vanity);
                saveItem();
            } else if ( key == "email") {
                newItem("EMAIL");
//                if (reader.attributes().value("classifier").toString() == "private") {
//                // Fritzbox 7270 kennt keine andere Eigenschaft als classifier=private Wozu dann speichern?
//                    addKey("HOME");
//                }
                QString value = reader.readElementText();
                addValue(value);
                saveItem();
            }
        } else if ( reader.isEndElement() ) {
            if (key == "contact") {
                saveVCard();
//            } else if ( itemNames.indexOf(key) > -1 ) {
//                saveItem();
            }
        }

    }

}
