#include "telwidget.h"
#include "../defines.h"


//#include "ui_telwidget.h"

TelWidget::TelWidget(int row,VCardItemDefines *def,QWidget *parent) :
    VCardWidget(row,def,parent)
{
    setupUI();
}
void TelWidget::toggleMaxView(){
    VCardWidget::toggleMaxView();
    foreach (QLineEdit *edit, editLines) {
        edit->setVisible(showMax);
    }
}

void TelWidget::setupUI(void){
    setupUIBefore();

    hLayout = new QHBoxLayout(this);
    gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0,0,0,0);
    addLayout = new QHBoxLayout();

    tBAddItems = new QToolButton;
    tBAddItems->setEnabled(false);
    connect(tBAddItems,SIGNAL(clicked()),this,SLOT(toggleMaxView()));

    toggleMaxView();

    lEProperty = createLabelEdit(this,tr("PhoneNumber"));
    gridLayout->addWidget(tBAddItems,0,0,Qt::AlignBottom);
    gridLayout->addWidget(lEProperty,0,1,Qt::AlignBottom);
    gridLayout->addWidget(lEType,0,2,Qt::AlignBottom);
    gridLayout->addLayout(addLayout,1,1);

    hLayout->addLayout(gridLayout);
    hLayout->addWidget(wCheckBoxes);

    hLayout->addWidget(tBUndo);
    hLayout->addWidget(tBDelete);

    setupUIAfter();
}

void TelWidget::save(void){
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
        QString valueString = lEProperty->text();
        QString separator = ";";
        foreach (QLineEdit *edit,editLines ) {
            if ( edit->text().size() ) {
                valueString += separator+edit->text();
                separator = ",";
            }
        }
        vCardItem->setData(keyString,valueString);
    }
    setModified(false);
}
void TelWidget::createParameter(int count){
    if ( count > 0 ) {
        tBAddItems->setEnabled(true);
        for ( int i = 0; i < count; i++) {
            editLines << createLineEdit(this);
            addLayout->addWidget(editLines[i]);
            editLines[i]->setVisible(false);
        }
    }
}

void TelWidget::setVCardItem(VCardItem *tCardItem){
    VCardWidget::setVCardItem(tCardItem);
    itemType = vCardItem->key(0);
    if ( objectDefine ) {
        optionList = objectDefine->options;
        translation = objectDefine->translations;
        QString itemName = objectDefine->item[1];
        selectDialog->setLabel(tr("Type of")+" "+itemName);
        tBDelete->setToolTip(tr("Delete")+" "+itemName);
        lEProperty->setLabelText(itemName);
        createParameter(objectDefine->extraParameter);
    }

    selectDialog->setLineEdit(lEType->lineEdit());
    selectDialog->setOptionList(&translation);

    lEProperty->setText(vCardItem->value(0));
    QStringList values = vCardItem->value(1).split(",");
    for (int i = 0; i < editLines.count(); i++) {
        if ( values.count() > i ) {
            editLines[i]->setText(values[i]);
        }
    }

    int index = 1;
    QString key = vCardItem->key(index++);
    cBHome->setChecked(true);
    while ( !key.isEmpty() ) {
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
        key = vCardItem->key(index++);
    }
    setModified(false);

}
