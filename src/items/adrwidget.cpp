#include "adrwidget.h"
#include "../defines.h"
AdrWidget::AdrWidget(int row,VCardItemDefines *def,QWidget *parent) :
    VCardWidget(row,def,parent)
{
    setupUI();
}

void AdrWidget::toggleMaxView(){
    VCardWidget::toggleMaxView();
    lERegion->setVisible(showMax);
    lECountry->setVisible(showMax);
    lEPoBox->setVisible(showMax);
}
void AdrWidget::setupUI(void){
    setupUIBefore();

    hLayout = new QHBoxLayout(this);

    gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(0);
    hLayoutStreetPoBox = new QHBoxLayout();
    hLayoutStreetPoBox->setSpacing(0);
    hLayoutLocality = new QHBoxLayout();
    hLayoutLocality->setSpacing(0);


    lEPoBox = createLabelEdit(this,tr("Post office box"),vCardColumn2);
    lEExtAddress = createLabelEdit(this,tr("Extended address"),vCardColumn2);
    lEStreet = createLabelEdit(this,tr("Street address"));
    lELocality = createLabelEdit(this,tr("Locality"));
    lERegion = createLabelEdit(this,tr("Region"),vCardColumn2);
    lEPostalCode = createLabelEdit(this,tr("Postal code"),vCardColumn1);
    lECountry = createLabelEdit(this,tr("Country"));

    hSpacer = new QSpacerItem(40,5,QSizePolicy::Expanding,QSizePolicy::Minimum);

    tBAddItems->setVisible(true);
    toggleMaxView();

    hLayoutStreetPoBox->addWidget(lEStreet);
    hLayoutStreetPoBox->addWidget(lEExtAddress);

    hLayoutLocality->addWidget(lEPostalCode);
    hLayoutLocality->addWidget(lELocality);

    gridLayout->addWidget(tBAddItems,1,0,Qt::AlignBottom);

    gridLayout->addLayout(hLayoutStreetPoBox,0,1);
    gridLayout->addLayout(hLayoutLocality,1,1,Qt::AlignBottom);
    gridLayout->addWidget(lECountry,2,1);

    gridLayout->addWidget(lEType,0,2);
    gridLayout->addWidget(lEPoBox,1,2,Qt::AlignBottom);
    gridLayout->addWidget(lERegion,2,2);

    hLayout->addLayout(gridLayout);

    vLayoutCheckboxes->addStretch();
    hLayout->addWidget(wCheckBoxes);

//    hLayout->addLayout(vLayoutCheckboxes);
    tBDelete->setToolTip(tr("Delete address"));
    hLayout->addWidget(tBUndo);
    hLayout->addWidget(tBDelete);

    setupUIAfter();
}

//post office box   |   (Postfach)
//extended address  |   (Adresszusatz)     (e.g., apartment or suite number)
//street address    |   (Straße+Hausnummer)
//locality          |   (Ort)              (e.g., city);
//region            |   (Region)           (e.g., state or province);
//postal code       |   (Postleitzahl)
//country name      |   (Land)             (full name in the language specified in

void AdrWidget::save(void){
    if ( modifiedState ) {
        QString keyString = itemType;
        if (cBWork->isChecked()) {
            keyString += ";WORK";
        }
        if (cBHome->isChecked()) {
            keyString += ";HOME";
        }
        QString type = lEType->text().trimmed();
        if ( !type.isEmpty()) {
            QStringList typeList = type.split(',');
            foreach(type,typeList) {
                int index = translation.indexOf(type);
                if ( index > 0) {
                    keyString += ";"+optionList[index];
                } else {
                    keyString += ";X-"+type;
                }
            }
        }
        if (cBPreferred->isChecked()) {
            keyString += ";PREF";
        }
        QStringList value;
        value << "" << "" << "" << "" << "" << "" << "";
        value[VCardAdrPoBox] = lEPoBox->text();
        value[VCardAdrExtAddress] = lEExtAddress->text();
        value[VCardAdrStreet] = lEStreet->text();
        value[VCardAdrLocality] = lELocality->text();
        value[VCardAdrRegion] = lERegion->text();
        value[VCardAdrPostalCode] = lEPostalCode->text();
        value[VCardAdrCountry] = lECountry->text();
        vCardItem->setData(keyString,value.join(";"));
    }
    setModified(false);
}
//dom    |  (heimische Lieferadresse)
//intl   |  (internationale Lieferadresse)
//postal |  (Postadresse)
//parcel |  (Paketadresse)
void AdrWidget::setVCardItem(VCardItem *tCardItem){
    VCardWidget::setVCardItem(tCardItem);
    itemType = vCardItem->key(0);
    if ( objectDefine ) {
        optionList = objectDefine->options;
        translation = objectDefine->translations;
        QString itemName = objectDefine->item[1];
        selectDialog->setLabel(tr("Type of")+" "+itemName);
        tBDelete->setToolTip(tr("Delete")+" "+itemName);
        //lEProperty->setLabelText(itemName);
        //createParameter(objectDefine->extraParameter);
    }
//    if ( itemType == "ADR") {
//        optionList << "USER DEFINED" << "DOM" << "INTL" << "POSTAL" << "PARCEL" ;
//        translation << tr("USER DEFINED") << tr("DOM") << tr("INTL") << tr("POSTAL") << tr("PARCEL");
//        selectDialog->setLabel(tr("Type of address"));
//    }
    selectDialog->setLineEdit(lEType->lineEdit());
    selectDialog->setOptionList(&translation);

    lEPoBox->setText( vCardItem->value(VCardAdrPoBox) );
    lEExtAddress->setText( vCardItem->value(VCardAdrExtAddress) );
    lEStreet->setText( vCardItem->value(VCardAdrStreet) );
    lELocality->setText( vCardItem->value(VCardAdrLocality) );
    lERegion->setText( vCardItem->value(VCardAdrRegion) );
    lEPostalCode->setText( vCardItem->value(VCardAdrPostalCode) );
    lECountry->setText( vCardItem->value(VCardAdrCountry) );
    int index = 1;
    QString key = vCardItem->key(index++);
    while ( !key.isEmpty() ) {
        bool doSkip = false;
        int pos = key.indexOf('=');
        if ( pos > -1 ) {
            if (( key.left(pos) == "CHARSET") | ( key.left(pos) == "ENCODING")) {
                doSkip = true;
            }
        }
        if ( !doSkip ) {
            if (key == "PREF") {
                cBPreferred->setChecked(true);
            } else if (key == "WORK") {
                cBWork->setChecked(true);
            } else if (key == "HOME") {
                cBHome->setChecked(true);
            } else {
                int index = optionList.indexOf(key);
                if ( index > 0 ) {
                    lEType->setText(translation[index]);
                } else {
                    if ( key.left(2) == "X-") {
                        key = key.mid(2);
                    }
                    lEType->setText(key);
                }
            }
        }
        key = vCardItem->key(index++);
    }
    setModified(false);
}
