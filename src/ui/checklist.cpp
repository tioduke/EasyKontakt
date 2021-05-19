#include <QCheckBox>
#include "checklist.h"
#include "ui_checklist.h"

CheckList::CheckList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckList)
{
    ui->setupUi(this);
    connect(ui->tBCancel,SIGNAL(clicked()),this,SLOT(canceled()));
    connect(ui->tBSave,SIGNAL(clicked()),this,SLOT(hide()));
    connect(ui->tBSave,SIGNAL(clicked()),this,SIGNAL(selected()));

    connect(ui->cBNone,SIGNAL(clicked()),this,SLOT(setCheckBoxes()));
    connect(ui->cBRequired,SIGNAL(clicked()),this,SLOT(setCheckBoxes()));
    connect(ui->cBSelect,SIGNAL(clicked()),this,SLOT(setCheckBoxes()));
    ui->cBNone->setFocusPolicy(Qt::NoFocus);
    ui->cBRequired->setFocusPolicy(Qt::NoFocus);
    ui->cBSelect->setFocusPolicy(Qt::NoFocus);
    setCheckBoxes();
    ui->cBSelect->setFocusPolicy(Qt::NoFocus);
    ui->cBRequired->setFocusPolicy(Qt::NoFocus);
}

CheckList::~CheckList()
{
    delete ui;
}
void CheckList::setCheckBoxes(void){
    ui->cBNone->setCheckState(Qt::Unchecked);
    ui->cBSelect->setCheckState(Qt::PartiallyChecked);
    ui->cBRequired->setCheckState(Qt::Checked);
    QCheckBox *cBox = dynamic_cast<QCheckBox *>(sender());
    if ( ( cBox == ui->cBNone) | ( cBox == ui->cBSelect) ) {
        for ( int row = 0; row < ui->listWidget->count(); row++) {
            checkBoxItem(row)->setCheckState(cBox->checkState());
        }
    }
}
QStringList CheckList::checkedItems(void){
    getCheckedItemList();
    return checkedItemList;
}
void CheckList::getCheckedItemList(void){
    checkedItemList.clear();
    for ( int index = 0; index < ui->listWidget->count() ; index++) {
        if ( isChecked(index) ) {
            checkedItemList << itemText(index);
        }
    }
}
QStringList CheckList::partiallyCheckedItems(void){
    getPartiallyCheckedItemList();
    return partiallyCheckedItemList;
}
void CheckList::getPartiallyCheckedItemList(void){
    partiallyCheckedItemList.clear();
    for ( int index = 0; index < ui->listWidget->count() ; index++) {
        if ( isPartiallyChecked(index) ) {
            partiallyCheckedItemList << itemText(index);
        }
    }
}

void CheckList::open(void){
    getCheckedItemList();
    getPartiallyCheckedItemList();
    show();
}
void CheckList::canceled(void){
    setPartiallyCheckStates(&partiallyCheckedItemList);
    setCheckStates(&checkedItemList);
    hide();
}

QCheckBox *CheckList::checkBoxItem(int index){
    QListWidgetItem *widgetItem= ui->listWidget->item(index);
    return qobject_cast<QCheckBox*> (ui->listWidget->itemWidget(widgetItem));
}

void CheckList::setupList(QStringList list,bool tristate){
    foreach ( QString itemText,list) {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        QCheckBox *itemCheckBox = new QCheckBox(ui->listWidget);
        itemCheckBox->setText(itemText);
        if ( tristate ) {
            itemCheckBox->setTristate();
        }
        if ( tristate ) {
            itemCheckBox->setCheckState(Qt::PartiallyChecked);
        } else {
            itemCheckBox->setCheckState(Qt::Checked);
        }
        ui->listWidget->setItemWidget(item,itemCheckBox);
        //ui->listWidget->addItem(itemText);
        //ui->listWidget->item(index++)->setCheckState(Qt::Checked);
    }
    setMinimumWidth(250);
    setMaximumWidth(250);
}
void CheckList::setCheckStates(QStringList *list){
    for ( int index = 0; index < ui->listWidget->count() ; index++) {
        QCheckBox *item = checkBoxItem(index);
        QString text = item->text();
        if ( list->indexOf(text) > -1) {
            item->setCheckState(Qt::Checked);
        }
    }
}
void CheckList::setPartiallyCheckStates(QStringList *list){
    for ( int index = 0; index < ui->listWidget->count() ; index++) {
        QCheckBox *item = checkBoxItem(index);
        QString text = item->text();
        if ( list->indexOf(text) > -1) {
            item->setCheckState(Qt::PartiallyChecked);
        } else {
            item->setCheckState(Qt::Unchecked);
        }
    }
}
int CheckList::count(){
    return ui->listWidget->count();
}
bool CheckList::isPartiallyChecked(QString text,bool state){
    for ( int index = 0; index < count(); index++) {
        if ( itemText(index) == text ) {
            return isPartiallyChecked(index);
        }
    }
    return state;
}

bool CheckList::isPartiallyChecked(int index){
    if ( index < count() ) {
        QCheckBox *item = checkBoxItem(index);
        return (item->checkState() == Qt::PartiallyChecked );
    }
    return false;
}
bool CheckList::isChecked(QString text,bool state){
    for ( int index = 0; index < count(); index++) {
        if ( itemText(index) == text ) {
            return isChecked(index);
        }
    }
    return state;
}

bool CheckList::isChecked(int index){
    if ( index < count() ) {
        QCheckBox *item = checkBoxItem(index);
        return (item->checkState() == Qt::Checked );
    }
    return false;
}
bool CheckList::isUnchecked(QString text,bool state){
    for ( int index = 0; index < count(); index++) {
        if ( itemText(index) == text ) {
            return isUnchecked(index);
        }
    }
    return state;
}

bool CheckList::isUnchecked(int index){
    if ( index < count() ) {
        QCheckBox *item = checkBoxItem(index);
        Qt::CheckState state = item->checkState();
        return ( state == Qt::Unchecked );
    }
    return false;
}
QString CheckList::itemText(int index){
    if ( index < count() ) {
        QCheckBox *item = checkBoxItem(index);
        return item->text();
//        return ui->listWidget->item(index)->text();
    }
    return "";

}
bool CheckList::allItemsChecked(void){
    for ( int index = 0; index < ui->listWidget->count() ; index++) {
        if (isUnchecked(index)) {
            return false;
        }
    }
    return true;
}
