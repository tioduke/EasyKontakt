#include "vcardeditor.h"
#include <QClipboard>
#include <QDirIterator>
#include <QMessageBox>
#include <QFileDialog>

#include "../export/export.h"
#include "../export/exportfritz.h"

VCardEditor::VCardEditor(QString fileName,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VCardEditor)

{
    ui->setupUi(this);

    nameWidget = NULL;
    nameLayout = new QVBoxLayout(ui->nameWidget);
    nameLayout->setSpacing(0);
    nameLayout->setContentsMargins(0,0,0,0);

    photoLayout = new QHBoxLayout(ui->photoWidget);
    photoLayout->setSpacing(0);
    photoLayout->setContentsMargins(0,0,0,0);

    oldRow = 0;
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);

    objectTable = new StringGridModel(this);
    readVCardDefines();

//    if ( objectTable->rowCount() == 0 ) {
//        objectTable->appendRow(QStringList() << "ADR"     << tr("Address")    << ""       << QString::number(VCard_Adr)     << "false");
//        objectTable->appendRow(QStringList() << "EMAIL"   << tr("EMail")      << ""       << QString::number(VCard_Tel)     << "false");
//        objectTable->appendRow(QStringList() << "PHOTO"   << tr("Photo")      << ""       << QString::number(VCard_Photo)   << "exclusive");
//        objectTable->appendRow(QStringList() << "TEL"     << tr("Phone No.")  << "+49 "   << QString::number(VCard_Tel)     << "false");
//    }
    objectTable->sort(VALUE_COLUMN);

    vCardFileName = fileName;
    vCardUserFileName = fileName;
    vCardList = new VCardList(this);
    ui->listView->setModel(vCardList);

    ui->cBSortType->addItems(vCardList->sortItems());
    ui->cBSortType->setCurrentIndex(Sort_FullName);

    connect(ui->cBSortType,SIGNAL(activated(int)),vCardList,SLOT(setSortType(int)));
    connect(ui->listView->selectionModel(),SIGNAL(currentChanged(const QModelIndex &,const QModelIndex &)),this,SLOT(readVCard(const QModelIndex &,const QModelIndex &)));
    connect(ui->listView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(listViewClicked(const QModelIndex &)));
    connect(ui->tBSave,SIGNAL(clicked()),this,SLOT(saveFile()));
    connect(ui->tBSaveAs,SIGNAL(clicked()),this,SLOT(saveFileAs()));
    connect(ui->tBSaveUser,SIGNAL(clicked()),this,SLOT(saveUserFile()));
    connect(ui->tBImport,SIGNAL(clicked()),this,SLOT(importFile()));
    connect(ui->tBSwap,SIGNAL(clicked()),this,SLOT(swapNames()));
    connect(ui->tBUndo,SIGNAL(clicked()),this,SLOT(undo()));
    connect(ui->tBDeleteVCard,SIGNAL(clicked()),this,SLOT(deleteVCard()));
    connect(ui->tBNewVCard,SIGNAL(clicked()),this,SLOT(createVCard()));
    connect(ui->tBCopyVCard,SIGNAL(clicked()),this,SLOT(copyVCard()));
    connect(ui->tBPasteVCard,SIGNAL(clicked()),this,SLOT(pasteVCard()));
    connect(ui->tBQrCode,SIGNAL(clicked()),this,SLOT(createVCardImg()));
    connect(ui->tBClose,SIGNAL(clicked()),this,SLOT(closeEditor()));
    connect(ui->cBUser,SIGNAL(editTextChanged(QString)),this,SLOT(userNameChanged(QString)));
    connect(ui->tBConfig,SIGNAL(clicked()),this,SLOT(showConfig()));
    connect(ui->tBNewUser,SIGNAL(clicked()),this,SLOT(newUser()));

    connect(ui->tBSearch,SIGNAL(clicked()),this,SLOT(findNextItem()));
    connect(ui->lESearch,SIGNAL(textChanged(QString)),this,SLOT(findNextItem(QString)));

// Nur bei Qt5
//    connect(ui->cBUser,SIGNAL(currentTextChanged(QString)),this,SLOT(userNameChanged(QString)));
    connect(ui->cBUser,SIGNAL(currentIndexChanged(QString)),this,SLOT(userNameChanged(QString)));
    connect(ui->cBSelectNone,SIGNAL(clicked(bool)),this,SLOT(selectUserList(bool)));
    connect(ui->cBSelectAll,SIGNAL(clicked(bool)),this,SLOT(selectUserList(bool)));

    if ( vCardFileName.size() ) {
        vCardList->read(vCardFileName);
        vCardList->setSortType(ui->cBSortType->currentIndex());
        ui->listView->setCurrentIndex(vCardList->index(0,0));
    }

    ui->listView->setMinimumWidth(100);
    QList<int> sizes ;
    sizes << 250;
    sizes << 550;
    ui->splitter->setSizes(sizes);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);

    ui->cBAddObject->addItem( tr("add Object") );
    //ui->cBAddObject->addItems(objectTable->column(VALUE_COLUMN));

    QStringList keys = objectTable->column(VALUE_COLUMN);
    QStringList subKeys = objectTable->column(SUBKEY_COLUMN);
    for ( int index = 0; index < keys.count(); index++) {
        QString key = keys[index];
        QString subKey = subKeys[index];
        ui->cBAddObject->addItem( key );
        ui->cBAddObject->setItemData(index+1,subKey,Qt::ToolTipRole);
        //ui->cBAddObject->setItemData(index+1,subKeys[index],Qt::ToolTipRole);
    }
    connect(ui->cBAddObject,SIGNAL(currentIndexChanged(QString)),this,SLOT(addObject(QString)));
    createUserList();
    conf = new CheckList();
    conf->setupList(objectTable->column(VALUE_COLUMN),true);

    if ( vCardList->rowCount() == 0 ) {
        createVCard();
    }
}
VCardEditor::~VCardEditor() {
    saveUserFile();
    delete objectTable;
    delete ui;
}

void VCardEditor::findNextItem(QString text){
    if ( text.size() ) {
        ui->lESearch->setText(text.toUpper());
    }
    QString findText = ui->lESearch->text().toUpper();
    if ( findText.size() ) {
        int startIndex = ui->listView->currentIndex().row()+1;
        if ( (text.size() > 0) | (startIndex >= vCardList->rowCount()) ) {
            startIndex = 0;
        }
        for ( int row = startIndex; row < vCardList->rowCount() ; row++) {
            QString name = vCardList->text(row).toUpper();
            if ( name.indexOf(findText) > -1 ) {
                ui->listView->setCurrentIndex(vCardList->index(row,0));
                return;
            }
        }
        for ( int row = 0; row < startIndex ; row++) {
            QString name = vCardList->text(row).toUpper();
            if ( name.indexOf(findText) > -1 ) {
                ui->listView->setCurrentIndex(vCardList->index(row,0));
                return;
            }
        }
    }
}

void VCardEditor::newUser(void){
    ui->cBUser->addItem(tr("New user"));
    ui->cBUser->setCurrentIndex(ui->cBUser->count()-1);
}
void VCardEditor::selectUserList(bool unchecked){
    // cBSelectNone bringt true!
    ui->cBSelectAll->setChecked(Qt::Checked);
    ui->cBSelectNone->setChecked(Qt::Unchecked);
    int state = Qt::Checked;
    if ( unchecked )  state = Qt::Unchecked;
    for ( int row = 0; row < vCardList->rowCount(); row++) {
        vCardList->setChecked(row,state);
    }
}

void VCardEditor::readVCardDefines(void){
    VCardItemDefines *define = NULL;
    QString text = loadFromFile(getIniPath()+"/VCardItems.conf");
    if ( text.isEmpty() ) {
        text = loadFromFile(getDocPath()+"/VCardItems.conf");
        saveToFile(getIniPath()+"/VCardItems.conf",text);
    }
    QStringList lines = text.split("\n");
    QStringList subKeys;
    foreach (QString line,lines) {
        QStringList items = line.split("=");
        items << "";
        items[0] = items[0].trimmed().toUpper();
        items[1] = items[1].trimmed();
        if ( items[0][0] == '[') {
            define = new VCardItemDefines;
            define->name = items[0].mid(1,items[0].size()-2);
            objectDefines << define;
        } else if (items[0] == "ITEM") {
            QStringList parameter = items[1].split(";");
            if ( parameter.count() > EXCLUSIVE_COLUMN) {
                bool ok;
                int type = parameter[TYPE_COLUMN].toInt(&ok);
                if ( ok && ( type > -1) ) {
                    subKeys << "";
                    if ( parameter[KEY_COLUMN].isEmpty() ) {
                        parameter[KEY_COLUMN] = define->name;
                    }
                    if ( parameter[VALUE_COLUMN].isEmpty() ) {
                        parameter[VALUE_COLUMN] = parameter[KEY_COLUMN];
                    }
//                    if ( parameter[VALUE_COLUMN].isEmpty() ) {
//                        QString line = parameter.join(";");
//                        if ( line.indexOf("xxxxqer") > -1) {
//                            parameter << "test";
//                        }
//                    }
                    objectTable->appendRow(parameter);
                }
            }
            define->item = parameter;
        } else if (items[0] == "OPTIONLIST") {
            if ( items[1].size() ) {
                define->options = items[1].split(";");
            }
        } else if (items[0] == "TYPE") {
            if ( items[1].size() ) {
                define->type = items[1].split(";");
                subKeys[subKeys.count()-1] = define->type[0];
            }
        } else if (items[0] == "VALUE") {
            if ( items[1].size() ) {
                define->value = items[1].split(";");
            }
        } else if (items[0] == "VALUELABEL") {
            if ( items[1].size() ) {
                define->valueLabel = items[1].split(";");
            }
        } else if (items[0] == "TRANSLATION") {
            define->translations = items[1].split(";");
        } else if (items[0] == "EXTRAPARAMETER") {
            bool ok;
            define->extraParameter=items[1].toInt(&ok);
        }
    }
//    QString line = objectTable->column(VALUE_COLUMN).join(";");
//    if ( line.indexOf("xxxxqer") > -1) {
//        subKeys << "test";
//    }

    objectTable->appendColumn(subKeys);
}

void VCardEditor::userNameChanged(QString name){
    vCardList->setShowCheckBoxes(!name.isEmpty());
    if ( name.size() )  {
        readUserFile();
    }
}
void VCardEditor::undo(void){
    vCardList->removeRows(0,vCardList->rowCount());
    while ( vCardWidgets.count() ) {
        delete vCardWidgets.takeLast();
    }
    if ( vCardFileName.size() ) {
        vCardList->read(vCardFileName);
        vCardList->setSortType(ui->cBSortType->currentIndex());
        ui->listView->setCurrentIndex(vCardList->index(0,0));
        userNameChanged(ui->cBUser->currentText());
    }
    ui->tBUndo->setEnabled(false);
    ui->tBSave->setEnabled(false);
}
void VCardEditor::undoWidget(void){
    if ( QObject::sender() == nameWidget ) {
        vCard->undo();
        QModelIndex index = ui->listView->currentIndex();
        readVCard(index);
        emit modifyUndo();
    }
}

void VCardEditor::setModified(bool state){
    if ( state && (!nameWidget->hasModified())  ) {
        nameWidget->setModified(true);
    }
    for ( int index = 0; index < vCardList->rowCount(); index ++) {
        if ( vCardList->vCard(index)->hasModified() ) {
            state = true;
        }
    }
    ui->tBUndo->setEnabled(state);
    ui->tBSave->setEnabled(state);
    emit modified(state);
}
void VCardEditor::setFileName(QString fName){
    vCardFileName = fName;
    vCardList->setFileName(vCardFileName);
}

QString VCardEditor::fileName(void){
    return vCardFileName;
}
void VCardEditor::setUserName(QString uName){
    if ( uName.size() ) {
        int index = ui->cBUser->findText(uName);
        if ( index > -1) {
            ui->cBUser->setCurrentIndex(index);
        }
    }
}
QString VCardEditor::userName(void){
    return ui->cBUser->currentText();
}
void VCardEditor::showConfig(void){
    conf->open();
}

void VCardEditor::closeEditor(void){
    saveFile();
    emit closeMe(this);
}

void VCardEditor::addWidgetToLayout(QWidget *w){
    int count = ui->vCardLayout->count();
    ui->vCardLayout->insertWidget(count-1,w);
}
int VCardEditor::findItemDefine(QString key){
    for ( int index = 0 ; index < objectDefines.count(); index++) {
        if (objectDefines[index]->name == key) {
            return index;
        }
    }
    return -1;
}

bool VCardEditor::checkVCardItem(VCardItem *item,VCardItemDefines *def){
    if ( def->type.count() > 2) {
       QString type = def->type[0];
       QString section = def->type[1];
       bool ok;
       int index = def->type[2].toInt(&ok);
       if ( ok ) {
           QString subKey = "";
           if ( section == "KEY") {
              subKey = item->key(index);
           } else {
              subKey = item->value(index);
           }
           if ( subKey != type ) {
               return false;
           }
       }
    }
    return true;
}

VCardWidget *VCardEditor::addVCardWidget(VCardTypes type,VCardItem *item,QString key) {

    int row = ui->vCardLayout->count();
    VCardWidget *vCardWidget = 0;
    VCardItemDefines *def = NULL;
    int objectDefineIndex = findItemDefine(key);
    if ( objectDefineIndex > -1 ) {
        def = objectDefines[objectDefineIndex];
    }
    switch (type) {
        case VCard_Hide:    break;
        case VCard_Name:     vCardWidget = new NameWidget(row,NULL,this); nameWidget = vCardWidget; break;
        case VCard_Tel:      vCardWidget = new TelWidget(row,def,this); break;
        case VCard_Photo:    vCardWidget = new FotoWidget(row,def,this); break;
        case VCard_Adr:      vCardWidget = new AdrWidget(row,def,this); break;
        case VCard_DateTime: vCardWidget = new DateWidget(row,def,this); break;
        case VCard_Note:     vCardWidget = new NoteWidget(row,def,this); break;
        case VCard_MultiLine:vCardWidget = new FallBackWidget(row,def,this); break;
        case VCard_FallBack: vCardWidget = new FallBackWidget(row,NULL,this); break;
    }
    if (vCardWidget) {
        vCardWidgets << vCardWidget;
        vCardWidget->setVCardItem(item);
        if ( type == VCard_Name ) {
            if ( vCard->hasModified() ) {
                vCardWidget->setModified(true);
            }
            connect(vCardWidget,SIGNAL(modifiedName(QString )),this,SLOT(modifyListView(QString)));
            nameLayout->addWidget(vCardWidget);
        } else if ( type == VCard_Photo ) {
            photoLayout->addWidget(vCardWidget);
            ui->nameFrame->setMaximumHeight(vCardWidget->height()+3);
        } else {
            addWidgetToLayout(vCardWidget);
        }
        connect(vCardWidget,SIGNAL(deleteMe(VCardWidget *)),this,SLOT(deleteVCardWidget(VCardWidget *)));
        connect(vCardWidget,SIGNAL(modified(bool)),this,SLOT(setModified(bool)));
        connect(vCardWidget,SIGNAL(modifyUndo()),this,SLOT(undoWidget()));
    }
    return vCardWidget;
}
void VCardEditor::modifyListView(QString text){
    int index = 0;
    VCardItem *vCardItem = vCard->vCardItem("FN", &index);
    vCardItem->setData("FN",text);
    // sort();
}

void VCardEditor::sort(void){
    vCardList->setSortType(ui->cBSortType->currentIndex());
}
void VCardEditor::readVCard(const QModelIndex &index,const QModelIndex &oldIndex){
    bool saveIt = ( nameWidget ) ? nameWidget->hasModified() : false;
    while( vCardWidgets.count() ) {
        VCardWidget *item = vCardWidgets.takeLast();
        if ( saveIt ) {
            item->save();
        }
        disconnect(item);
        ui->vCardLayout->removeWidget(item);
        delete item;
    }
    // VCardItems zeigt noch auf das oben gelöschten VCard-NameWidget -> alle auf NULL
    if ( oldIndex.isValid() ) { // Nur wenn sich der Index ändert, es also einen alten Index gibt
        int vCardItemIndex = 0;
        while ( VCardItem *vCardItem = vCard->vCardItem(vCardItemIndex++)) {
            vCardItem->setWidget(NULL);

        }
    }
    int vCardIndex = vCardList->data(index,IndexRole).toInt();
    vCard = vCardList->vCard(vCardIndex);
    if ( vCard ) {
        QString doNotShow = "-FN-";
        for ( int index = 0; index < objectDefines.count(); index++) {
            QString key = objectDefines[index]->name;
            VCardTypes type = VCard_Name;
            if ( key != "N") {
                type = (VCardTypes) objectDefines[index]->item[TYPE_COLUMN].toInt();
            }
            int vCardIndex = 0;
            if ( type == VCard_Hide ) {
               doNotShow += key + "-";
            } else {
                while (VCardItem *item = vCard->vCardItem(key,&vCardIndex)) {
                    if ( checkVCardItem(item,objectDefines[index])) {
                        addVCardWidget(type,item,key);
                    }
                }
            }
        }
        int vCardIndex = 0;
        while (VCardItem *item = vCard->vCardItem(vCardIndex++)) {
            if  ( item->widget() == NULL) {
                if ( doNotShow.indexOf("-"+item->key()+"-") < 0 ) {
                    addVCardWidget(VCard_FallBack,item,item->key());
                }
            }
        }

    }

}
void VCardEditor::listViewClicked(const QModelIndex &index){
    int row = index.row();
    if (row == oldRow ) {
        vCardList->setData(index,true,Qt::CheckStateRole);
    }
    oldRow = row;
}

void VCardEditor::copyVCard(void){
    if ( vCard) {
        QString newText = vCard->text();
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(newText);
    }
}

void VCardEditor::pasteVCard(void){
    QClipboard *clipboard = QApplication::clipboard();
    QString text = clipboard->text();
    if ( text.indexOf("BEGIN:VCARD") == 0 ) {
        createVCard(text);
    }
}
// qrencode -o qr_mecard.png 'MECARD:N:Max Mustermann;TEL:+49-123-45678;EMAIL:muster@mann.de;URL:http://home.page.com;ADR:Adresse;BDAY:19701228;NICK:musti;NOTE:keine Notizen;;'
void VCardEditor::createVCardImg(void){
// 4296 Zeichen soll ein QRCode maximal aufnehmen können
    for ( int index = 0; index < vCardWidgets.count(); index++) {
        vCardWidgets[index]->save();
    }
    QString fileName = QDir::homePath()+"/vcard.png";
    QString vCardText = vCard->qrCodeText();
    QStringList args;
    //args << "-S";
    //args << "-v 16";
    args << "-o";
    args << fileName;
    args << vCardText;
    QString cmd = "qrencode";
#ifdef Q_OS_WIN
        cmd = "qrcode.exe";
#endif
    int result =  QProcess::execute(cmd,args);
    if ( result == 0) {
        ImageWidget *i = new ImageWidget(fileName,vCard->value("FN"),vCardText);
        i->exec();
        delete i;
    } else {
        QString errorText = tr("unknown error");
        if ( result == -1 ) errorText = cmd+" "+tr("crashes");
        if ( result == -2 ) errorText = cmd+" "+tr("is not available");
        QMessageBox::critical(this,errorText,tr("Error: executing")+" "+cmd+" \n"+tr("maybe you have not installed qrencoder"));
    }
}

void VCardEditor::createVCard(void){
    QString text;
    //text += "BEGIN:VCARD\r\n";
    text += "VERSION:2.1\r\n";
    text += "FN:Vorname Name\r\n";
    text += "N:Name;Vorname;;;\r\n";
    text += "TEL:+49\r\n";
    //text += "END:VCARD";
    createVCard(text);
}
void VCardEditor::createVCard(QString text,bool goToNewLine){
    QStringList list = text.split("\r\n");
    int start = 0;
    if ( list[0].toUpper() == "BEGIN:VCARD") {
        start++;
    }
    VCard *vCard = new VCard(this,&list,start);
    int row = vCardList->append(vCard);
    if ( goToNewLine ) {
        //sort();
        setCurrentRowFromSource(row);
    }
}

void VCardEditor::deleteVCardWidget(VCardWidget *w){
    setModified(true);
    disconnect(w,SIGNAL(deleteMe(VCardWidget *)));
    vCard->deleteVCardItem(w->getVCardItem());
    ui->vCardLayout->removeWidget(w);
    int index = vCardWidgets.indexOf(w);
    if ( index > -1 ) {
        vCardWidgets.takeAt(index);
    }
    w->deleteLater();
}
void VCardEditor::addObject(QString name ) {
    bool ok;
    VCardItem *item = NULL;

    int index = objectTable->rowIndexOf(VALUE_COLUMN,name);
    QString key = objectTable->text(KEY_COLUMN,index);
    QString parameter = objectTable->text(PARAM_COLUMN,index);
    VCardTypes type = (VCardTypes) objectTable->text(TYPE_COLUMN,index).toInt(&ok);
    if (objectTable->text(EXCLUSIVE_COLUMN,index) == "exclusive") {
        ok = ok & (vCard->vCardItem(key) == NULL);
    }
    if ( ok ) {
        item = vCard->addVCardItem(key,parameter);
        VCardWidget *vCardWidget = addVCardWidget(type,item,key);
        vCardWidget->setDefaultValues();
    }
    ui->cBAddObject->setCurrentIndex(0);
}
void VCardEditor::createUserList(void){
    QString path = getIniPath();
     ui->cBUser->addItem("");
    QDirIterator it(path, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        it.next();
        QString ext = it.fileInfo().completeSuffix().toLower();
        if (ext == "vcu" ) {
            ui->cBUser->addItem(it.fileInfo().baseName());
        }
    }
}

void VCardEditor::saveUserFile(void){
    QString user = ui->cBUser->currentText();
    if ( user.size() ) {
        QString fileName = getIniPath()+user+".vcu";
        QString text = "";
        text += "USERFILENAME="+vCardUserFileName+"\n";
        text += "USERFILEFILTER="+userContactFilter+"\n";
        if ( !conf->allItemsChecked() ) {
            QString separator = "";
            text += "ITEMS=";
            for ( int index = 0; index < conf->count(); index++ ) {
                if ( !conf->isUnchecked(index)) {
                    text += separator+conf->itemText(index);
                    separator = ";";
                }
            }
            text += "\n";
        }
        QString separator = "";
        text += "REQUIREDITEMS=";
        for ( int index = 0; index < conf->count(); index++ ) {
            if ( conf->isChecked(index)) {
                text += separator+conf->itemText(index);
                separator = ";";
            }
        }
        text += "\n";
        for ( int index = 0; index < vCardList->count(); index++) {
            if ( vCardList->isChecked(index) ) {
                text += vCardList->value("FN",index)+"\n";
            }
        }
        saveToFile(fileName,text);
    }
}
void VCardEditor::readUserFile(void){
    QString userName = ui->cBUser->currentText();
    conf->setWindowTitle(userName);
    QString userFile = loadFromFile(getIniPath()+userName+".vcu");
    if ( userFile.isEmpty() ) {
        for ( int index = 0; index < vCardList->count(); index++) {
            vCardList->setChecked(index,true) ;
        }
        return;
     }
    QStringList userList = userFile.split("\n");
    for ( int index = 0; index < userList.count(); index++) {
        QStringList items = userList[index].split("=");
        if ( items.count() == 2) {
            if ( items[0]  == "ITEMS") {
                QStringList keys = items[1].split(";");
                conf->setPartiallyCheckStates(&keys);
            } else if ( items[0]  == "REQUIREDITEMS") {
                QStringList keys = items[1].split(";");
                conf->setCheckStates(&keys);
            } else if ( items[0]  == "USERFILENAME") {
                vCardUserFileName = items[1];
            } else if ( items[0]  == "USERFILEFILTER") {
                userContactFilter = items[1];
            }
        }
    }
    for ( int index = 0; index < vCardList->count(); index++) {
        QString text = vCardList->value("FN",index);
        vCardList->setChecked(index,userList.indexOf(text) > -1) ;
    }
}

void VCardEditor::importFile(void){
    QString filter;
    QString filters = "VCard "+tr("Files")+" (*.vcf)";
            filters += ";;Fritz.box "+tr("Files")+" (*.xml)";
            filters += ";;Thunderbird "+tr("-Comma separated")+" "+tr("Files")+" (*.csv)";
            filters += ";;Thunderbird "+tr("-Tabulator separated")+" "+tr("Files")+" (*.txt)";
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
           tr("Open contact data file"), QDir::homePath(), filters, &filter);
    for ( int i = 0; i < fileNames.count(); i++) {
        vCardList->read(fileNames[i]);
        setModified(true);
    }
}
void VCardEditor::swapNames(void){
//    for ( int index = 0; index < vCardWidgets.count(); index++) {
//        NameWidget *nameWidget = qobject_cast<NameWidget *>(vCardWidgets[index]);
//        if ( nameWidget ) {
//            nameWidget->swapName();
//        }
//    }
    for ( int index = 0; index < vCardList->count(); index++) {
        VCard *card = vCardList->vCard(index);
        int itemIndex = 0;
        VCardItem *item;
        while ( (item = card->vCardItem(itemIndex++)) ) {
            QWidget *w = item->widget();
            NameWidget *nameWidget = qobject_cast<NameWidget *>(w);
            if ( nameWidget ) {
                nameWidget->swapName();
            } else {
                if ( item->key() == "N" ) {
                    item->swapValues(0,1);
                }
            }
        }
    }
//    ui->cBSortType->setCurrentIndex(Sort_FamilyName);
//    QApplication::processEvents();
//    ui->cBSortType->setCurrentIndex(Sort_FullName);
    //vCardList->contentUpdate();
    vCardList->setSortType(Sort_FullName);
    QApplication::processEvents();
}

void VCardEditor::saveFile(void){
    if ( ! ui->tBSave->isEnabled() ) {
        return;
    }
    saveUserFile();
    QString text = "";
    for ( int index = 0; index < vCardWidgets.count(); index++) {
        (vCardWidgets[index])->save();
    }
    for ( int index = 0; index < vCardList->count(); index++) {
        VCard *card = vCardList->vCard(index);
        text += "BEGIN:VCARD\r\n";
        int itemIndex = 0;
        VCardItem *item;
        while ( (item = card->vCardItem(itemIndex++)) ) {
            text += item->vCardData()+"\r\n";
        }
        text += "END:VCARD\r\n";
    }
    saveToFile(vCardFileName,text);
    ui->tBSave->setEnabled(false);
    ui->tBUndo->setEnabled(false);
}

void VCardEditor::saveUserContactFile(void){
   saveFileAs();
}

void VCardEditor::saveFileAs(void){
    QString filters = "";
    filters += "Fritz.box - "+tr("Format: Salvatore Dali")+" (*.xml)";
    filters += ";;Fritz.ex - "+tr("Format: Dali Salvatore")+" (*.xml)";
    filters += ";;VCard - "+tr("Format")+" (*.vcf)";
    if ( userContactFilter.isEmpty()  ) {
        userContactFilter = "Fritz.box - "+tr("Format")+" (*.xml)";
        //userContactFilter = "VCard - "+tr("Format")+" (*.vcf)";
    }

    QString fileName = vCardUserFileName;

    fileName = QFileDialog::getSaveFileName(this,tr("Save contact list as file"),fileName , filters,&userContactFilter);
    if ( fileName.size() ) {
        Export *exFormat = NULL;
        for ( int index = 0; index < vCardWidgets.count(); index++) {
            (vCardWidgets[index])->save();
        }
        if ( userContactFilter.indexOf("VCard") > -1 ) {
            exFormat = new Export();
        } else if ( userContactFilter.indexOf("Fritz.box") > -1 ) {
            exFormat = new ExportFritz();
        } else if ( userContactFilter.indexOf("Fritz.ex") > -1 ) {
            ExportFritz *ex = new ExportFritz();
            ex->setExchangeMode(true);
            exFormat = ex;
        }
        if ( exFormat ) {
            QString text = "";
            text += exFormat->intro();
            for ( int index = 0; index < vCardList->count(); index++) {
                QStringList requiredItems = conf->checkedItems();
                VCard *card = vCardList->vCard(index);
                QString cardText = "";
                if ( card->isChecked() ) {
                    cardText += exFormat->contactIntro();
                    int itemIndex = 0;
                    VCardItem *item;
                    while ( (item = card->vCardItem(itemIndex++)) ) {
                        QString key = item->key();
                        int row = requiredItems.indexOf(objectTable->value(key));
                        if ( row > -1) {
                            requiredItems.removeAt(row);
                        }
                        QString checkListKey = objectTable->value(key);
                        if ( objectTable->textIsValid() ) {
                            if ( !conf->isUnchecked(checkListKey,false)) {
                                cardText += exFormat->itemText(item);
                            }
                        } else {
                            cardText += exFormat->itemText(item);
                        }
                    }
                    cardText += exFormat->contactOutro();
                }
                if ( requiredItems.count() == 0) {
                    // alle benötigten Einträge vorhanden
                    text += cardText;
                }
            }
            text += exFormat->outro();
            if ( exFormat->codecName().isEmpty() ) {
                saveToFile(fileName,text);
            } else {
                saveToFile(fileName,text,exFormat->codecName());
            }
            vCardUserFileName = fileName;
        }
    }

}

int VCardEditor::setCurrentRow(int row){
    QModelIndex index = vCardList->index(row,0);
    if ( index.isValid() ) {
        row = index.row();
        ui->listView->setCurrentIndex(index);
    }
    return row;
}
QModelIndex VCardEditor::indexOfSourceRow(int row){
    for ( int i = 0; i < vCardList->count(); i++ ) {
        QModelIndex index = vCardList->index(i,0);
        int modelRow = index.data(IndexRole).toInt();
        if (  modelRow == row ) {
            return index;
        }
    }
    return QModelIndex();
}

int VCardEditor::setCurrentRowFromSource(int row){
    QModelIndex index = vCardList->index(row,0);
    if ( index.isValid() ) {
        row = index.row();
        ui->listView->setCurrentIndex(index);
    }
    return row;
}
void VCardEditor::goNext(void){
    int row = ui->listView->currentIndex().row()+1;
    if ( row == vCardList->count() ) {
        row = 0;
    }
    QModelIndex index = vCardList->index(row,0);
    if ( index.isValid() ) {
        ui->listView->setCurrentIndex(index);
    }
}


void VCardEditor::deleteVCard(void){
    VCard *currentCard = vCard;
    int row = ui->listView->currentIndex().row();
    vCardList->removeOne(currentCard);
    if ( !(row < vCardList->rowCount()) ) {
        row--;
        if (row < 0) {
            return;
        }
    }
    setCurrentRow(row);
}
