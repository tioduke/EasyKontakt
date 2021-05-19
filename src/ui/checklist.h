#ifndef USERCONF_H
#define USERCONF_H

#include <QWidget>
#include <QCheckBox>

namespace Ui {
class CheckList;
}

class CheckList : public QWidget
{
    Q_OBJECT
    
public:
    explicit CheckList(QWidget *parent = 0);
    ~CheckList();
    void setupList(QStringList list,bool tristate = false);
    void setCheckStates(QStringList *list);
    void setPartiallyCheckStates(QStringList *list);
    int count(void);
    bool isPartiallyChecked(int index);
    bool isPartiallyChecked(QString text,bool state = false);
    bool isChecked(int index);
    bool isChecked(QString text,bool state = false);
    bool isUnchecked(int index);
    bool isUnchecked(QString text,bool state = false);
    QString itemText(int index);
    bool allItemsChecked(void);
    QCheckBox *checkBoxItem(int index);

    QStringList checkedItems(void);
    QStringList partiallyCheckedItems(void);
public slots:
    void open(void);
signals:
    void selected(void);
private slots:
    void canceled(void);
    void setCheckBoxes(void);
private:
    void getCheckedItemList(void);
    void getPartiallyCheckedItemList(void);
    Ui::CheckList *ui;
    QStringList checkedItemList;
    QStringList partiallyCheckedItemList;
};

#endif // USERCONF_H
