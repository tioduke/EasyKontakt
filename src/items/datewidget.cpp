#include "datewidget.h"
#include "../defines.h"

DateWidget::DateWidget(int row,VCardItemDefines *def,QWidget *parent) :
    VCardWidget(row,def,parent)
{
    setupUI();
}
void DateWidget::toggleMaxView(){
    VCardWidget::toggleMaxView();
    foreach (QLineEdit *edit, editLines) {
        edit->setVisible(showMax);
    }
}

void DateWidget::setupUI(void){
    setupUIBefore();

    hLayout = new QHBoxLayout(this);
    gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0,0,0,0);
    //addLayout = new QHBoxLayout();

    tBAddItems = new QToolButton;
    tBAddItems->setEnabled(false);
    connect(tBAddItems,SIGNAL(clicked()),this,SLOT(toggleMaxView()));

    toggleMaxView();

    lEEvent = createLabelEdit(this,"DATETIME");
    lEEventEnd = createLabelEdit(this,"DATETIME");
    QString t = objectDefine->item[PARAM_COLUMN];
//    if ( ( t != NULL) & (( t == "$DATE") | ( t == "$DATETIME")) ) {
//       tBCalendarSelect = new QToolButton(this);
//       tBCalendarSelect->setText("...");
//       connect(tBCalendarSelect,SIGNAL(clicked()),this,SLOT(openCalendar()));
//       lEEvent->addWidget(tBCalendarSelect);

//       tBCalendarSelect2 = new QToolButton(this);
//       tBCalendarSelect2->setText("...");
//       connect(tBCalendarSelect2,SIGNAL(clicked()),this,SLOT(openCalendar2()));
//       lEEventEnd->addWidget(tBCalendarSelect2);
//    }
    connect(lEEvent->dateTimeEdit(),SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(updateEventEnd(QDateTime)));
    gridLayout->addWidget(tBAddItems,0,0,Qt::AlignBottom);
    gridLayout->addWidget(lEEvent,0,1,Qt::AlignBottom);
    gridLayout->addWidget(lEEventEnd,0,2,Qt::AlignBottom);
    gridLayout->addItem(new QSpacerItem(10,5,QSizePolicy::MinimumExpanding),0,3);
    gridLayout->addWidget(lEType,0,4,Qt::AlignBottom);
    //gridLayout->addLayout(addLayout,1,1);

    hLayout->addLayout(gridLayout);
    hLayout->addWidget(wCheckBoxes);

    hLayout->addWidget(tBUndo);
    hLayout->addWidget(tBDelete);

    setupUIAfter();
}

void DateWidget::save(void){
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
        QString valueString = lEEvent->text();
        if ( objectDefine->item[2] == "$DATETIME") {
           valueString += ";"+lEEventEnd->text();
        }

        vCardItem->setData(keyString,valueString);
    }
    setModified(false);
}

void DateWidget::setVCardItem(VCardItem *tCardItem){
    bool modifiedDate = false;
    VCardWidget::setVCardItem(tCardItem);
    itemType = vCardItem->key(0);
    if ( objectDefine ) {
        optionList = objectDefine->options;
        translation = objectDefine->translations;
        QString itemName = objectDefine->item[1];
        selectDialog->setLabel(tr("Type of")+" "+itemName);
        tBDelete->setToolTip(tr("Delete")+" "+itemName);
        lEEvent->setLabelText(itemName);
        if ( objectDefine->item[2] == "$DATETIME") {
            lEEventEnd->setLabelText(tr("End"));
        } else {
            lEEventEnd->setVisible(false);
        }
    }

    selectDialog->setLineEdit(lEType->lineEdit());
    selectDialog->setOptionList(&translation);

    int valueIndex = 0;
    //    lEEvent->setText(vCardItem->value(valueIndex++));
    //    if ( objectDefine->item[2] == "$DATETIME") {
    //        lEEventEnd->setText(vCardItem->value(valueIndex++));
    //    }
    QString valueText = vCardItem->value(valueIndex++);
    lEEvent->setText(valueText);
    modifiedDate = modifiedDate || (valueText != lEEvent->text());
    if ( objectDefine->item[2] == "$DATETIME") {
        valueText = vCardItem->value(valueIndex++);
        lEEventEnd->setText(valueText);
        modifiedDate = modifiedDate || (valueText != lEEventEnd->text());
    }

    int index = 1;
    QString key = vCardItem->key(index++);

    cBPreferred->setVisible(false);
    cBPreferred->setChecked(false);
    cBHome->setChecked(true);

    while ( !key.isEmpty() ) {
        if (key == "WORK") {
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
    setModified(modifiedDate);
}
void DateWidget::updateEventEnd(QDateTime dateTime){
    if ( lEEventEnd->dateTimeEdit()->dateTime() < dateTime ) {
       lEEventEnd->dateTimeEdit()->setDateTime(dateTime);
    }
}
