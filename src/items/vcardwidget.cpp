#include "vcardwidget.h"
#include "../defines.h"

VCardWidget::VCardWidget(int row,VCardItemDefines *def,QWidget *parent) :
    QFrame(parent)
{
    objectDefine = def;
    showMax = true;
    vCardItem = NULL;
    setAutoFillBackground(true);
    QPalette p = palette();
    if ( row % 2) {
        p.setBrush(QPalette::Window,p.alternateBase());
    } else {
        p.setBrush(QPalette::Window,p.base());
    }
    setPalette(p);
    setObjectName("VCardWidget"+QString::number(row));
}
void VCardWidget::clear(void){
    lEType->setText("");
    cBWork->setAutoExclusive(false);
    cBHome->setAutoExclusive(false);

    cBWork->setChecked(false);
    cBHome->setChecked(false);
    cBPreferred->setChecked(false);

    cBWork->setAutoExclusive(true);
    cBHome->setAutoExclusive(true);
}
void VCardWidget::setDefaultValues(void){

}

void VCardWidget::setVCardItem(VCardItem *tCardItem){
    vCardItem = tCardItem;
    vCardItem->setWidget(this);
    clear();
    if ( objectDefine) {
        if ( objectDefine->options.count() == 0) {
            lEType->setVisible(false);
            tBSelect->setVisible(false);
            wCheckBoxes->setVisible(false);
        }
    }
}
void VCardWidget::setVCardItemDefine(VCardItemDefines *def){
    objectDefine = def;
}

void VCardWidget::save(void){
}
void VCardWidget::setHeight(int h){
    setMinimumHeight(h);
//    setMaximumHeight(h);
}

void VCardWidget::toggleMaxView(){
    showMax = !showMax;
    QString text = ":/img/img/go-bottom.png";
    if ( showMax) {
        text = ":/img/img/go-up.png";
    }
    tBAddItems->setIcon(QIcon(text));
}

void VCardWidget::setupUIBefore(void){
    //setHeight(50);
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum));
    setFrameStyle(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    font.setPointSize(8);

    tBSelect = new QToolButton(this);
    tBSelect->setText("...");
    lEType = createLabelEdit(this,tr("Type"),vCardColumn2,"DEFAULT");
    lEType->addWidget(tBSelect);

    tBDelete = new QToolButton(this);
    tBDelete->setIcon(QIcon(":/img/img/close.png"));
    tBDelete->setIconSize(QSize(24,24));
    tBDelete->setObjectName("tBDelete");
    connect(tBDelete,SIGNAL(clicked()),this,SLOT(deleteMeLater()));

    tBUndo = new QToolButton(this);
    tBUndo->setIcon(QIcon(":/img/img/edit-undo.png"));
    tBUndo->setIconSize(QSize(24,24));
    tBUndo->setObjectName("tBUndo");
    tBUndo->setToolTip(tr("Undo item"));
    connect(tBUndo,SIGNAL(clicked()),this,SLOT(undo()));

    wCheckBoxes = new QWidget();
    hLayoutCheckboxes = new QHBoxLayout(wCheckBoxes);
    hLayoutCheckboxes->setSpacing(0);
    hLayoutCheckboxes->setContentsMargins(0,0,0,0);
    vLayoutCheckboxes = new QVBoxLayout();
    vLayoutCheckboxes->setSpacing(0);
    vLayoutCheckboxes->setContentsMargins(0,0,0,0);

    cBWork = createCheckBox(this,tr("Work"),0,true);
    cBHome = createCheckBox(this,tr("Home"),0,true);
    cBPreferred = createCheckBox(this,tr("preferred"));
    cBPreferred->setObjectName("cBPreferred");

    lEType->addLabelWidget(cBPreferred);
//    vLayoutCheckboxes->addWidget(cBPreferred);
    vLayoutCheckboxes->addWidget(cBHome);
    vLayoutCheckboxes->addWidget(cBWork);
    hLayoutCheckboxes->addLayout(vLayoutCheckboxes);

    selectDialog = new SelectDialog(this);
    connect(tBSelect,SIGNAL(clicked()),selectDialog,SLOT(open()));

    tBAddItems = new QToolButton;
    //tBAddItems->setIconSize(QSize(33,33));
    tBAddItems->setVisible(false);
    connect(tBAddItems,SIGNAL(clicked()),this,SLOT(toggleMaxView()));

}
void VCardWidget::setupUIAfter(void){
    layout()->setContentsMargins(2, 0, 0, 0);
    tBUndo->setEnabled(false);

}
VCardItem *VCardWidget::getVCardItem(void){
    return vCardItem;
}
void VCardWidget::undo(void){
    setModified(false);
    vCardItem->undo();
    setVCardItem(vCardItem);
    //tBUndo->setEnabled(false);
    emit modifyUndo();
}

void VCardWidget::setModified(bool newState){
    tBUndo->setEnabled(newState);
    modifiedState = newState;
    emit modified(newState);
}

void VCardWidget::setModified(QString /*str*/){
    setModified(true);
}
void  VCardWidget::setModified(QDateTime /*dateTime*/){
    setModified(true);
}

bool VCardWidget::hasModified(void){
    return modifiedState;
}
void VCardWidget::deleteMeLater(void){
    emit deleteMe(this);
}
QLabel *VCardWidget::createLabel(QWidget *parent,QString text,int width){
    QLabel *label = new QLabel(parent);
    label->setText(text);
    label->setFont(font);
    if ( width ) {
        label->setMinimumWidth(width);
        label->setMaximumWidth(width);
    }
    //label->setMinimumHeight(VCardEditHeight());
    //label->setMaximumHeight(VCardEditHeight());
    return label;
}
QLineEdit *VCardWidget::createLineEdit(QWidget *parent,int width){
    QLineEdit *lineEdit = new QLineEdit(parent);
    if ( width ) {
        lineEdit->setMinimumWidth(width);
        lineEdit->setMaximumWidth(width);
    }
    //lineEdit->setMinimumHeight(VCardEditHeight());
    //lineEdit->setMaximumHeight(VCardEditHeight());
    connect(lineEdit,SIGNAL(textChanged(QString )),this,SLOT(setModified(QString )));
    return lineEdit;
}
QLabelEdit *VCardWidget::createLabelEdit(QWidget *parent,QString labelText,int width,QString typeString){
    LabelType type = LineEditType;
    if ( (objectDefine != NULL) /*& (labelText == "DATETIME")*/) {
        QString t = objectDefine->item[PARAM_COLUMN];
        if ( !typeString.isEmpty() ) {
            t = "$"+typeString.toUpper();
        }
        if ( t == "$DATE") type = DateEditType;
        else if ( t == "$TIME") type = TimeEditType;
        else if ( t == "$DATETIME") type = DateTimeEditType;
        else if ( t == "$INT") type = IntEditType;
    }
    QLabelEdit *labelEdit = new QLabelEdit(parent,type);
    if ( typeString.toUpper() == "TYPE") {
        labelEdit->lineEdit()->setReadOnly(true);
    }
    if ( width ) {
        labelEdit->setWidth(width);
    }
    if ( labelText.size() ) {
        labelEdit->setLabelText(labelText);
    }
    if ( type == LineEditType ) {
        connect(labelEdit->lineEdit(),SIGNAL(textChanged(QString )),this,SLOT(setModified(QString )));
    } else if ( type == IntEditType ) {
        connect(labelEdit->spinBoxEdit(),SIGNAL(valueChanged(QString )),this,SLOT(setModified(QString )));
    } else {
        connect(labelEdit->dateTimeEdit(),SIGNAL(dateTimeChanged(QDateTime )),this,SLOT(setModified(QDateTime )));
    }
    return labelEdit;
}
QCheckBox *VCardWidget::createCheckBox(QWidget *parent, QString text, int width, bool autoexclusive){
    QCheckBox *checkBox = new QCheckBox(parent);
    checkBox->setText(text);
    checkBox->setFont(font);
    checkBox->setAutoExclusive(autoexclusive);
    if ( width ) {
        checkBox->setMinimumWidth(width);
        checkBox->setMaximumWidth(width);
    }
    //checkBox->setIconSize(QSize(VCardEditHeight(),VCardEditHeight()));
    //checkBox->setMaximumHeight(VCardEditHeight());
    connect(checkBox,SIGNAL(clicked()),this,SLOT(setModified()));
    return checkBox;
}
QString VCardWidget::key(void){
    return vCardItem->key();
}
