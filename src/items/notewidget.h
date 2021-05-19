#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H
#include "vcardwidget.h"
#include <QTextEdit>

class NoteWidget : public VCardWidget
{
    Q_OBJECT
public:
    explicit NoteWidget(int row,VCardItemDefines *def,QWidget *parent = 0);
    void setVCardItem(VCardItem *tCardItem);
public slots:
    void save(void);
    void toggleMaxView();
private slots:

private:
    void setupUI(void);
    QStringList optionList;
    QStringList translation;
    QString itemType;

    QHBoxLayout *hLayout;
    //QHBoxLayout *gridLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *hLayoutType;

    QTextEdit *tEdit;
    int widgetHeight;
};

#endif // NOTEWIDGET_H
