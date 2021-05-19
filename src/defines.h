#ifndef DEFINES_H
#define DEFINES_H

#include <QString>

enum VCardNameIndex {
    VCardNameFamily = 0,
    VCardNameGiven,
    VCardNameAdditional,
    VCardNamePrefix,
    VCardNameSuffix
};
enum VCardAddressIndex {
    VCardAdrPoBox = 0,
    VCardAdrExtAddress,
    VCardAdrStreet,
    VCardAdrLocality,
    VCardAdrRegion,
    VCardAdrPostalCode,
    VCardAdrCountry
};
enum VCardTypes {
    VCard_Hide = -1,
    VCard_Name,
    VCard_Tel,
    VCard_Adr,
    VCard_Photo,
    VCard_DateTime,
    VCard_Note,
    VCard_MultiLine,
    VCard_FallBack
};
enum ObjectTable {
    KEY_COLUMN,
    VALUE_COLUMN,
    PARAM_COLUMN,
    TYPE_COLUMN,
    EXCLUSIVE_COLUMN,
    SUBKEY_COLUMN
};


const int vCardColumn1 = 80;
const int vCardColumn2 = 150;

#endif // DEFINES_H
