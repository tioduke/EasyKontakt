#include "fallbackwidget.h"
#include "../defines.h"

FallBackWidget::FallBackWidget(int row,VCardItemDefines *def,QWidget *parent) :
    VCardWidget(row,def,parent)
{
    setupUI();
}
FallBackWidget::~FallBackWidget() {
    while ( !lEList.isEmpty() ) {
        delete lEList.takeLast();
    }
}

void FallBackWidget::toggleMaxView(){
    VCardWidget::toggleMaxView();
    foreach (QLabelEdit *lEdit,lEList) {
        if ( lEdit->text().isEmpty() ) {
            lEdit->setVisible(showMax);
        } else {
            lEdit->setVisible(true);
        }
    }
}
void FallBackWidget::setupUI(void){
    setupUIBefore();

    vLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout(this);
//    gridLayout = new QGridLayout();
//    gridLayout->setVerticalSpacing(0);
//    gridLayout->setContentsMargins(0,0,0,0);

    tBAddItems = new QToolButton;
    tBAddItems->setEnabled(false);
    connect(tBAddItems,SIGNAL(clicked()),this,SLOT(toggleMaxView()));

    if ( (objectDefine != NULL) && (objectDefine->value.count() > 0) ) {
        int index = 0;
        foreach ( QString typeString, objectDefine->value) {
            typeString = typeString.trimmed().toUpper();
            QString label = typeString;
            if ( objectDefine->valueLabel.count() > index ) {
                label = objectDefine->valueLabel[index++];
            }
            if ( label.toUpper() == "TYPE") {
                label = objectDefine->name;
            }
            QLabelEdit *lEdit = createLabelEdit(this,label,0,typeString);
            if ( typeString == "TYPE" ) {
                if ( objectDefine->type.count() ) {
                    lEdit->setText(objectDefine->type[0]);
                    lEdit->setText("objectDefine->type[0]");
                }
            } else if ( typeString == "INT" ) {
                lEdit->setText("1");
            }
            lEList << lEdit;
            vLayout->addWidget(lEdit);
        }
        int count = objectDefine->extraParameter;
        while ( index < count) {
            QLabelEdit *lEdit = 0;
            lEdit = createLabelEdit(this,objectDefine->name+" -> "+tr("Item")+":"+QString::number(++index));
            lEList << lEdit;
            vLayout->addWidget(lEdit);
        }
    } else {
    }
    lEType->setVisible(false);
    wCheckBoxes->setVisible(false);

    toggleMaxView();

//    gridLayout->addWidget(tBAddItems,0,0,Qt::AlignBottom);
//    gridLayout->addLayout(vLayout,0,1,Qt::AlignBottom);
//    gridLayout->addWidget(lEType,0,3,Qt::AlignBottom);

//    hLayout->addLayout(gridLayout);
    hLayout->addWidget(tBAddItems);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(wCheckBoxes);

    hLayout->addWidget(tBUndo);
    hLayout->addWidget(tBDelete);
    tBDelete->setToolTip(tr("Delete item"));


    setupUIAfter();
}


void FallBackWidget::save(void){
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
        QString value = "";
        QString separator = "";
        foreach (QLabelEdit *lEdit,lEList) {
            value += separator + lEdit->text();
            separator = ";";
        }
         vCardItem->setData(keyString,value);
    }
    setModified(false);
}
void FallBackWidget::setDefaultValues(void){
    int index = 0;
    foreach ( QString typeString, objectDefine->value) {
        typeString = typeString.trimmed().toUpper();
        QLabelEdit *lEdit = lEList[index++];
        if ( typeString == "TYPE" ) {
            if ( objectDefine->type.count() ) {
                lEdit->setText(objectDefine->type[0]);
            }
        } else if ( typeString == "INT" ) {
            lEdit->setText("1");
        }
        if ( lEdit->text().isEmpty() ) {
            lEdit->setVisible(showMax);
            tBAddItems->setEnabled(true);
        } else {
            lEdit->setVisible(true);
        }
    }

}
void FallBackWidget::setVCardItem(VCardItem *tCardItem){
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

    QString key = vCardItem->key();
    for ( int index = 0; index < vCardItem->valueCount(); index++) {
        QLabelEdit *lEdit = 0;
        if ( lEList.count() <= index ) {
            lEdit = createLabelEdit(this,key+" -> "+tr("Item")+":"+QString::number(index+1));
            lEList << lEdit;
            vLayout->addWidget(lEdit);
        } else {
            lEdit = lEList.at(index);
        }
        lEdit->setText( vCardItem->value(index));
        if ( lEdit->text().isEmpty() ) {
            lEdit->setVisible(showMax);
            tBAddItems->setEnabled(true);
        } else {
            lEdit->setVisible(true);
        }
    }

    int index = 1;
    key = vCardItem->key(index++);
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
