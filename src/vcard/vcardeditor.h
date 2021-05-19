#ifndef VCARDEDITOR_H
#define VCARDEDITOR_H

#include <QSortFilterProxyModel>
#include <QProcess>
#include <QString>

#include "ui_vcardeditor.h"

#include "vcardlist.h"
#include "../items/telwidget.h"
#include "../items/datewidget.h"
#include "../items/notewidget.h"
#include "../items/namewidget.h"
#include "../items/adrwidget.h"
#include "../items/fotowidget.h"
#include "../items/fallbackwidget.h"
#include "vcardeditor.h"
#include "../items/imagewidget.h"
#include "../defines.h"
#include "../func.h"
#include "../ui/checklist.h"
#include "../stringgridmodel.h"

namespace Ui {
class VCardEditor;
}


class VCardEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit VCardEditor(QString fileName, QWidget *parent = 0);
    ~VCardEditor();
    QString fileName(void);
    void setFileName(QString fName);
    QString userName(void);
    void setUserName(QString uName);
signals:
    void closeMe(VCardEditor *);
    void modified(bool);
    void modifyUndo(void);
public slots:
    void createVCard(QString text,bool goToNewLine = true);
    void saveFile(void);
    void saveFileAs(void);
    void saveUserFile(void);
    void importFile(void);
    void swapNames(void);
    void setModified(bool state = true);
    void sort(void);
    void closeEditor(void);
    void undoWidget(void);
    void undo(void);
private slots:
    void readVCard(const QModelIndex &index,const QModelIndex &oldIndex = QModelIndex());
    void listViewClicked(const QModelIndex &index);
    void deleteVCardWidget(VCardWidget *w);
    void addObject(QString name = "");
    void modifyListView(QString text);
    void deleteVCard(void);
    void createVCard(void);
    void copyVCard(void);
    void createVCardImg(void);
    void pasteVCard(void);
    void userNameChanged(QString name);
    void goNext(void);
    void saveUserContactFile(void);
    void showConfig(void);
    void newUser(void);
    void selectUserList(bool on);
    void findNextItem(QString text = "");
private:
    void readVCardDefines(void);
    void createUserList(void);
    void readUserFile(void);
    int setCurrentRow(int row = 0);
    int setCurrentRowFromSource(int row = 0);
    void addWidgetToLayout(QWidget *w = NULL);
    int findItemDefine(QString key);
    QModelIndex indexOfSourceRow(int row);
    VCardWidget *addVCardWidget(VCardTypes type,VCardItem *item = NULL,QString key = "");
    bool checkVCardItem(VCardItem *item,VCardItemDefines *def);
    Ui::VCardEditor *ui;

    QString vCardFileName;
    QString vCardUserFileName;
    QString userContactFilter;

    VCardList *vCardList;
    VCard *vCard;
    //QSortFilterProxyModel *proxyModel;
    QList<VCardWidget *> vCardWidgets;
    VCardWidget *nameWidget;
    CheckList *conf;
    int oldRow;
    StringGridModel *objectTable;
    QList<VCardItemDefines *> objectDefines;
    QVBoxLayout *nameLayout;
    QHBoxLayout *photoLayout;
};

#endif // VCARDEDITOR_H
