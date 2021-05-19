#ifndef VCARDWIDGET_H
#define VCARDWIDGET_H

#include <QWidget>
#include <QFrame>

#include "../vcard/vcarditem.h"
#include "../ui/selectdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include "qlabeledit.h"


struct VCardItemDefines {
    QString name;
    QStringList item;
    QStringList options;
    QStringList translations;
    QStringList type;
    QStringList value;
    QStringList valueLabel;
    int extraParameter;
    int size;
};

class VCardWidget : public QFrame
{
    Q_OBJECT
public:
    explicit VCardWidget(int row,VCardItemDefines *def,QWidget *parent = 0);

    virtual void setVCardItem(VCardItem *tCardItem);
    virtual void setDefaultValues(void);
    void setVCardItemDefine(VCardItemDefines *def);
    VCardItem *getVCardItem(void);
    bool hasModified(void);
    QString key(void);
    void clear(void);
public slots:
    virtual void save(void);
    virtual void toggleMaxView();
    void setModified(QString str = "");
    void setModified(QDateTime dateTime);
    void setModified(bool newState);
    void undo(void);
signals:
    void deleteMe(VCardWidget *w);
    void modified(bool);
    void modifiedName(QString newName);
    void modifyUndo(void);
private slots:
    void deleteMeLater(void);

signals:
    

protected:
    void setupUIBefore(void);
    void setupUIAfter(void);
    void setHeight(int h);
    QLabel *createLabel(QWidget *parent,QString text,int width = 0);
    QLineEdit *createLineEdit(QWidget *parent, int width = 0);
    QLabelEdit *createLabelEdit(QWidget *parent,QString labelText = "",int width = 0,QString type = "");
    QCheckBox *createCheckBox(QWidget *parent, QString text, int width = 0,bool autoexclusive = false);

    QToolButton *tBDelete;
    QToolButton *tBUndo;

    QLabelEdit *lEType;
    QToolButton *tBSelect;
    QToolButton *tBAddItems;
    bool showMax;

    QCheckBox *cBWork;
    QCheckBox *cBHome;
    QCheckBox *cBPreferred;

    QVBoxLayout *vLayoutCheckboxes;
    QHBoxLayout *hLayoutCheckboxes;
    QWidget *wCheckBoxes;

    VCardItem *vCardItem;

    bool modifiedState;
    QFont font;

    SelectDialog *selectDialog;
    VCardItemDefines *objectDefine;
};




#endif // VCARDWIDGET_H
