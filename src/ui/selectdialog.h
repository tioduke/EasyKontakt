#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include "ui_selectdialog.h"
#include <QStringList>
#include <QLineEdit>

namespace Ui {
class SelectDialog;
}

class SelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectDialog(QWidget *parent = 0);
    ~SelectDialog();
    void setOptionList(QStringList *list);
    void setLineEdit(QLineEdit *line);
    void setLabel(QString text);
public slots:
    void open(void);
private slots:
    void add(void);
    void ok(void);
    void rowChanged(int index);
    void lEUserValidator(QString text);
private:
    Ui::SelectDialog *ui;
    QLineEdit *lineEdit;
    QString selectedString;
    QStringList *optionList;
};

#endif // SELECTDIALOG_H
