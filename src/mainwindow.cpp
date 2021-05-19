#include "mainwindow.h"
#include "version.h"
#include "func.h"
#include "aboutbox.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamReader>
#include "import/import.h"
#include "import/importfritz.h"
#include "import/importcsv.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    setPalette(qApp->palette());
    ui->setupUi(this);

    setWindowTitle("EasyKontakt "+VERSION);
//    help = new WHelpInfo();
//    help->init("EasyKontakt.html");
    help = new HelpBrowser(getDocPath());

    connect(ui->tBOpen,SIGNAL(clicked()),this,SLOT(openEditor()));
    connect(ui->tBAbout,SIGNAL(clicked()),this,SLOT(showAbout()));
    connect(ui->tBSaveAll,SIGNAL(clicked()),this,SLOT(saveAll()));
    connect(ui->tBNew,SIGNAL(clicked()),this,SLOT(newEditor()));
    connect(ui->tBHelp,SIGNAL(clicked()),this,SLOT(showHelp()));
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeEditor(int)));


//    addVCardEditor(QDir::homePath()+"/kontakte.vcf");
//    addVCardEditor(QDir::homePath()+"/kontakteNeu.vcf");
    readIniFile();
    if ( ui->tabWidget->count() == 0 ) {
        QString text = loadFromFile(QDir::homePath()+"/example.vcf");
        if ( text.isEmpty() ) {
            text = loadFromFile(getDocPath()+"/example.vcf");
            saveToFile(QDir::homePath()+"/example.vcf",text);
            addVCardEditor(QDir::homePath()+"/example.vcf");
        }
    }
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabsClosable(true);
}

MainWindow::~MainWindow()
{
    if ( ui->tBSaveAll->isEnabled() ) {
        saveAll();
    }
    saveIniFile();
    delete ui;
}
void MainWindow::showHelp(void){
    help->loadAndShow("EasyKontakt.html");
}

void MainWindow::readIniFile(void){
    int left = 50;
    int top = 50;
    int width = 800;
    int height = 600;
    bool ok;
    QStringList lines = loadFromFile(getIniFile()).split("\n");
    foreach(QString line,lines) {
        line = line.trimmed();
        int pos = line.indexOf("=");
        QString value = (line.mid(pos+1)).trimmed();
        if (line.indexOf("VCARD-FILE") == 0) {
            addVCardEditor(value);
        } else if ( line.indexOf("VCARD-DATEFORMAT") == 0) {
            setVCardDateFormat(value.trimmed());
        } else if ( line.indexOf("VCARD-USER") == 0) {
            vCardEditors.last()->setUserName(value.trimmed());
        } else if ( line.indexOf("LEFT") == 0) {
            left = value.toInt(&ok);
        } else if ( line.indexOf("TOP") == 0) {
            top = value.toInt(&ok);
        } else if ( line.indexOf("WIDTH") == 0) {
            width = value.toInt(&ok);
        } else if ( line.indexOf("HEIGHT") == 0) {
            height = value.toInt(&ok);
        }
    }
    setGeometry(left,top,width,height);
}

void MainWindow::saveIniFile(void){
    QString out = "";
    QRect rect = geometry();
    out += "LEFT="+QString::number(rect.x())+"\n";
    out += "TOP="+QString::number(rect.y())+"\n";
    out += "WIDTH="+QString::number(rect.width())+"\n";
    out += "HEIGHT="+QString::number(rect.height())+"\n";
    out += "VCARD-DATEFORMAT="+vCardDateFormat(-1)+"\n";

    foreach(VCardEditor *editor,vCardEditors) {
        out += "VCARD-FILE="+editor->fileName()+"\n";
        out += "VCARD-USER="+editor->userName()+"\n";
    }
    saveToFile(getIniFile(),out);
}

void MainWindow::showAbout(void){
    showAboutBox();
}
void MainWindow::saveAll(void){
    foreach(VCardEditor *editor,vCardEditors) {
        editor->saveFile();
    }
    ui->tBSaveAll->setEnabled(false);
}
bool MainWindow::isAlreadyOpen(QString fileName){
    for ( int i = 0; i < vCardEditors.count(); i++ ) {
        if ( vCardEditors[i]->fileName() == fileName ) {
            ui->tabWidget->setCurrentIndex(i);
            return true;
        }
    }
    return false;
}

VCardEditor *MainWindow::addVCardEditor(QString fileName){
    int pos = fileName.lastIndexOf("/");
    VCardEditor *vCardEditor = new VCardEditor(fileName,this);
    vCardEditors << vCardEditor;
    ui->tabWidget->addTab(vCardEditor,QIcon(),fileName.mid(pos+1));

    connect(vCardEditor,SIGNAL(closeMe(VCardEditor *)),this,SLOT(deleteVCardEditor(VCardEditor *)));
    connect(vCardEditor,SIGNAL(modified(bool)),ui->tBSaveAll,SLOT(setEnabled(bool)));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

    return vCardEditor;

}
void MainWindow::deleteVCardEditor(VCardEditor *vCardEditor){
    for (int index = 0; index < vCardEditors.count(); index++ ) {
        if ( vCardEditors[index] == vCardEditor ) {
            vCardEditors.takeAt(index);
        }
    }
    for (int index = 0; index < ui->tabWidget->count(); index++ ) {
        if ( ui->tabWidget->widget(index) == vCardEditor ) {
            ui->tabWidget->removeTab(index);
        }
    }
    vCardEditor->deleteLater();
}
void MainWindow::newEditor(void){
    QString filters = "VCard "+tr("Files")+" (*.vcf)";
    QString fileName = QFileDialog::getSaveFileName(this,
           tr("Create contact data file"), QDir::homePath(),filters);
    if ( !fileName.isEmpty() ) {
        if ( fileName.indexOf(".vcf") == -1 ) {
            fileName += ".vcf";
        }
        addVCardEditor(fileName);
    }
}
void MainWindow::closeEditor(int index){
    vCardEditors[index]->closeEditor();
}

void MainWindow::openEditor(void){
    QString filter;
    QString filters = "VCard "+tr("Files")+" (*.vcf)";
            filters += ";;Fritz.box "+tr("Files")+" (*.xml)";
            filters += ";;Thunderbird "+tr("-Comma separated")+" "+tr("Files")+" (*.csv)";
            filters += ";;Thunderbird "+tr("-Tabulator separated")+" "+tr("Files")+" (*.txt)";
    QString fileName = QFileDialog::getOpenFileName(this,
           tr("Open contact data file"), QDir::homePath(), filters, &filter);
    if ( !fileName.isEmpty() ) {
        if ( filter.indexOf("VCard") > -1 ) {
            addVCardEditor(fileName);
        } else {
            VCardEditor *vCardEditor = addVCardEditor("");
            QString vcfFileName = fileName;
            int pos = fileName.lastIndexOf('.');
            QString suffix = fileName.mid(pos);
            vcfFileName.replace(suffix,".vcf");
            vCardEditor->setFileName(vcfFileName);
            pos = vcfFileName.lastIndexOf("/");
            ui->tabWidget->setTabText(ui->tabWidget->count()-1,vcfFileName.mid(pos+1));
            Import *import = NULL;
            if ( filter.indexOf("Fritz.box") > -1 ) {
                import = new ImportFritz();
            } else if ( filter.indexOf("Thunderbird") > -1 ) {
                import = new ImportCSV();
                //        } else if ( (filter.indexOf("Thunderbird") > -1) & (suffix.toLower() == ".csv")) {
                //            import = new ImportCSV();
                //        } else if ( (filter.indexOf("Thunderbird") > -1) & (suffix.toLower() == ".txt")) {
                //            import = new ImportCSV();
            }
            if ( import ) {
                connect(import,SIGNAL(addVCard(QString,bool)),vCardEditor,SLOT(createVCard(QString,bool)));
                import->read(fileName);
                disconnect(import);
                delete import;
                vCardEditor->setModified(true);
                vCardEditor->sort();
            }
        }
    }
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}
