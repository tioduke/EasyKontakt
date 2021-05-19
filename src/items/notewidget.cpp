#include "notewidget.h"

NoteWidget::NoteWidget(int row,VCardItemDefines *def,QWidget *parent) :
    VCardWidget(row,def,parent)
{
    setupUI();
}
void NoteWidget::toggleMaxView(){
    VCardWidget::toggleMaxView();
    if ( !showMax ) {
        widgetHeight = 50;
    } else {
        widgetHeight = 250;
    }
    tEdit->setMaximumHeight(widgetHeight);
    tEdit->setMinimumHeight(widgetHeight);
}

void NoteWidget::setupUI(void){
    setupUIBefore();

    hLayout = new QHBoxLayout(this);
//    gridLayout = new QHBoxLayout();
    gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0,0,0,0);
    //addLayout = new QHBoxLayout();

    tBAddItems = new QToolButton;
    tBAddItems->setEnabled(true);
    connect(tBAddItems,SIGNAL(clicked()),this,SLOT(toggleMaxView()));

    tEdit = new QTextEdit();
    tEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    connect(tEdit,SIGNAL(textChanged()),this,SLOT(setModified()));

//    gridLayout->addWidget(tBAddItems/*,0,0,Qt::AlignBottom*/);
//    gridLayout->addWidget(tEdit/*,0,1,Qt::AlignBottom*/);
//    gridLayout->addWidget(lEType/*,0,2,Qt::AlignBottom*/);
    gridLayout->addWidget(tBAddItems,0,0,Qt::AlignBottom);
    gridLayout->addWidget(tEdit,0,1,Qt::AlignBottom);
    gridLayout->addWidget(lEType,0,2,Qt::AlignBottom);

    hLayout->addLayout(gridLayout);
    hLayout->addWidget(wCheckBoxes);

    hLayout->addWidget(tBUndo);
    hLayout->addWidget(tBDelete);

    setupUIAfter();
    toggleMaxView();
}

void NoteWidget::save(void){
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
        QString valueString = tEdit->toPlainText().replace("\n","\\n");

        vCardItem->setData(keyString,valueString);
    }
    setModified(false);
}

void NoteWidget::setVCardItem(VCardItem *tCardItem){
    VCardWidget::setVCardItem(tCardItem);
    itemType = vCardItem->key(0);
    if ( objectDefine ) {
        optionList = objectDefine->options;
        translation = objectDefine->translations;
        QString itemName = objectDefine->item[1];
        selectDialog->setLabel(tr("Type of")+" "+itemName);
        tBDelete->setToolTip(tr("Delete")+" "+itemName);
    }

    selectDialog->setLineEdit(lEType->lineEdit());
    selectDialog->setOptionList(&translation);

    int valueIndex = 0;
    tEdit->setPlainText(vCardItem->value(valueIndex++).replace("\\n","\n"));

    int index = 1;
    QString key = vCardItem->key(index++);

    cBPreferred->setVisible(false);
    cBPreferred->setChecked(false);
    while ( !key.isEmpty() ) {
        bool doSkip = false;
        int pos = key.indexOf('=');
        if ( pos > -1 ) {
            if (( key.left(pos) == "CHARSET") | ( key.left(pos) == "ENCODING")) {
                doSkip = true;
            }
        }
        if ( !doSkip ) {
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
        }
        key = vCardItem->key(index++);
        }
    setModified(false);
}
