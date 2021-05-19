#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>

#include "vcard/vcardlist.h"
#include "items/telwidget.h"
#include "items/namewidget.h"
#include "items/fotowidget.h"
#include "vcard/vcardeditor.h"
#include "defines.h"
#include "help/help.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    VCardEditor *addVCardEditor(QString fileName);
    bool isAlreadyOpen(QString fileName);

private slots:
    void deleteVCardEditor(VCardEditor *vCardEditor);
    void openEditor(void);
    void closeEditor(int index);
    void newEditor(void);
    void showAbout(void);
    void saveAll(void);
    void showHelp(void);
private:
    void readIniFile(void);
    void saveIniFile(void);
    Ui::MainWindow *ui;

    QList<VCardEditor *> vCardEditors;
    Help *help;
};

#endif // MAINWINDOW_H
