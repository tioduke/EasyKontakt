#include "namewidget.h"

NameWidget::NameWidget(int row,VCardItemDefines *def,QWidget *parent) :
    VCardWidget(row,def,parent)
{
    setupUI();
}

void NameWidget::setupUI(void){
    setupUIBefore();
    tBDelete->setVisible(false);
    cBWork->setVisible(false);
    cBHome->setVisible(false);
    cBPreferred->setVisible(false);
    lEType->setVisible(false);

    tBSwap = new QToolButton();
    tBSwap->setIconSize(QSize(30,30));
    tBSwap->setIcon(QIcon(":/img/img/view-refresh.png"));
    tBSwap->setToolTip(tr("exchange given name with family name"));
    connect(tBSwap,SIGNAL(clicked()),this,SLOT(swapName()));

    lENameGiven = createLabelEdit(this,tr("Given Name"));
    lENameAdditional = createLabelEdit(this,tr("Additional Names"));
    lENameFamily = createLabelEdit(this,tr("Family Name"));
    lENamePrefix = createLabelEdit(this,tr("Honorary Title"),vCardColumn1);
    lENameSuffix = createLabelEdit(this,tr("Suffix"),vCardColumn1);

    hLayout = new QHBoxLayout(this);
    //hLayout->setSpacing(0);

    hLayout->addWidget(lENameGiven);
    hLayout->addWidget(lENameAdditional);
    hLayout->addWidget(lENameFamily);
    hLayout->addWidget(lENamePrefix);
    hLayout->addWidget(lENameSuffix);

    hLayout->addWidget(tBSwap);
    hLayout->addWidget(tBUndo);

    setupUIAfter();
    tBUndo->setToolTip(tr("Undo contact"));
}
void NameWidget::swapName(void){
    QString name = lENameFamily->text();
    lENameFamily->setText(lENameGiven->text());
    lENameGiven->setText(name);
    setModified(true);
}

QString addName(QString text) {
    text = text.trimmed();
    if ( text.isEmpty() ) {
        return "";
    }
    return " "+text.replace(','," ");
}

void NameWidget::save(void){
    if ( modifiedState ) {
        QStringList value;
        value << "" << "" << "" << "" << "";
        value[VCardNameGiven] = lENameGiven->text();
        value[VCardNameAdditional] = lENameAdditional->text();
        value[VCardNameFamily] = lENameFamily->text();
        value[VCardNamePrefix] = lENamePrefix->text();
        value[VCardNameSuffix] = lENameSuffix->text();
        vCardItem->setData("N",value.join(";"));
    }
    QString name = addName(lENamePrefix->text()).trimmed();
    name += addName(lENameGiven->text());
    name += addName(lENameAdditional->text());
    name += addName(lENameFamily->text());
    name += addName(lENameSuffix->text());
    emit modifiedName(name);
    setModified(false);
}

void NameWidget::setVCardItem(VCardItem *tCardItem){
    VCardWidget::setVCardItem(tCardItem);
    lENameGiven->setText( vCardItem->value(VCardNameGiven) );
    lENameAdditional->setText( vCardItem->value(VCardNameAdditional) );
    lENameFamily->setText( vCardItem->value(VCardNameFamily) );
    lENamePrefix->setText( vCardItem->value(VCardNamePrefix) );
    lENameSuffix->setText( vCardItem->value(VCardNameSuffix) );
    setModified(false);
}
