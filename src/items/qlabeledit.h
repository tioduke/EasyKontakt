#ifndef QLABELEDIT_H
#define QLABELEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QTimeEdit>
#include <QLabel>
#include <QToolButton>
#include <QBoxLayout>
#include <QSpacerItem>
#include <QCalendarWidget>

enum LabelType {
    LineEditType,
    DateEditType,
    TimeEditType,
    DateTimeEditType,
    IntEditType
};

class QLabelEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QLabelEdit(QWidget *parent = 0,LabelType type = LineEditType, int direction = Qt::Vertical);
    void setWidth(int w);
    void setText(QString text);
    void setLabelText(QString text);
    void setLabelFont(QFont font);
    QString text(void);
    QLineEdit *lineEdit(void);
    QDateTimeEdit *dateTimeEdit(void);
    QSpinBox *spinBoxEdit(void);
    void addWidget(QWidget *widget);
    void addStretch(void);
    void addLabelWidget(QWidget *widget);
signals:
    
public slots:

private slots:
    void openCalendar(void);
    void calendarClicked(QDate date);
private:
    QToolButton *tBCalendarSelect;
    QDateTimeEdit *dtEdit;
    QSpinBox *sbEdit;
    QLineEdit *lEdit;
    QLabel *label;
    QHBoxLayout *hLayout;
    QSpacerItem *spacer;
    LabelType labelType;
    QGridLayout *gridLayout;

    QCalendarWidget *calendar;
    QDate minimumDate;

    int layoutDirection;

    int editWidgetRow;
    int editWidgetCol;
};

#endif // QLABELEDIT_H
