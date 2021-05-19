#include "selectdialog.h"

SelectDialog::SelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDialog)
{
    ui->setupUi(this);
    lineEdit = NULL;
    optionList = NULL;
    connect(ui->tBCancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->tBAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(ui->tBOk,SIGNAL(clicked()),this,SLOT(ok()));
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(rowChanged(int)));
    connect(ui->lEUser,SIGNAL(textEdited(QString)),this,SLOT(lEUserValidator(QString)));
}
SelectDialog::~SelectDialog(){
    delete ui;
}
void SelectDialog::lEUserValidator(QString text){
    QString newText = "";
    QString doNotUse = ";,:";
    for (int i = 0; i < text.size(); i++) {
        QChar ch = text[i];
        if ( doNotUse.indexOf(ch) == -1) {
            if ( ch <= '~') {
                newText += ch.toUpper();
            }
        }
    }
    ui->lEUser->setText(newText);
}

void SelectDialog::open(void){
    int index = 0;
    if (lineEdit) {
        QString text = lineEdit->text();
        int pos = text.lastIndexOf(',');
        text = text.mid(pos+1);
        pos = optionList->indexOf(text);
        if ( pos > -1) {
            index = pos;
        } else {
            ui->lEUser->setText(text);
        }
    }
    ui->listWidget->setCurrentRow(index);
    exec();
}

void SelectDialog::setLabel(QString text){
    setWindowTitle(text);
}

void SelectDialog::setOptionList(QStringList *list){
    optionList = list;
    ui->listWidget->clear();
    foreach (QString line,*list) {
        ui->listWidget->addItem(line);
    }
}
void SelectDialog::setLineEdit(QLineEdit *line){
    lineEdit = line;
}

void SelectDialog::add(void){
    selectedString = ui->listWidget->currentItem()->text();
    if ( ui->listWidget->currentRow() == 0 ) {
        selectedString = ui->lEUser->text();
    }
    if ( lineEdit ) {
        QString oldText = lineEdit->text();
        if ( oldText.indexOf(selectedString) < 0) {
            lineEdit->setText(oldText+","+selectedString);
        }
    }
    accept();
}

void SelectDialog::ok(void){
    selectedString = ui->listWidget->currentItem()->text();
    if ( ui->listWidget->currentRow() == 0 ) {
        selectedString = ui->lEUser->text();
    }
    if ( lineEdit ) {
        lineEdit->setText(selectedString);
    }
    accept();
}
void SelectDialog::rowChanged(int index){
    ui->lEUser->setEnabled( index == 0);
}
