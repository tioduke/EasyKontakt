#include "qlabeledit.h"
#include <QToolButton>
#include "../func.h"

QLabelEdit::QLabelEdit(QWidget *parent,LabelType type, int direction) :
    QWidget(parent)
{
    labelType = type;
    layoutDirection = direction;
    QWidget *editWidget = NULL;
    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins ( 0,0,0,0 );
    gridLayout->setSpacing(0);

    label = new QLabel(this);
    label->setObjectName("labelEditLabel");

    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins ( 0,0,0,0 );
    //hLayout->addWidget(label);

    switch (labelType) {
    case LineEditType: {
        lEdit = new QLineEdit(this);
        lEdit->setObjectName("labelEditLineEdit");
        //lEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        editWidget = lEdit;
        hLayout->addStretch();
        break; }
    case DateEditType: {
        dtEdit = new QDateEdit(QDate::currentDate(),this);
        dtEdit->setDisplayFormat("dd.MM.yyyy");
        dtEdit->setObjectName("labelEditDTEdit");
        dtEdit->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
        editWidget = dtEdit;
        break; }
    case TimeEditType: {
        dtEdit = new QTimeEdit(QTime::currentTime(),this);
        dtEdit->setDisplayFormat("hh:mm:ss");
        dtEdit->setObjectName("labelEditDTEdit");
        dtEdit->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
        editWidget = dtEdit;
        break; }
    case DateTimeEditType: {
        dtEdit = new QDateTimeEdit(QDateTime::currentDateTime(),this);
        dtEdit->setDisplayFormat("dd.MM.yyyy - hh:mm");
        dtEdit->setObjectName("labelEditDTEdit");
        dtEdit->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
        editWidget = dtEdit;
        break; }
    case IntEditType: {
        sbEdit = new QSpinBox(this);
        sbEdit->setObjectName("labelEditIntEdit");
        sbEdit->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
        //lEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        editWidget = sbEdit;
        break; }
    }
    int gridIndex = 0;
    if ( layoutDirection == Qt::Horizontal) {
        gridLayout->addWidget(label,0,gridIndex,Qt::AlignBottom);
        gridLayout->addLayout(hLayout,1,gridIndex++,Qt::AlignBottom);
        gridLayout->addWidget(editWidget,0,gridIndex++,1,2,Qt::AlignBottom);
        editWidgetRow = 0;
        editWidgetCol = gridIndex;
        //spacer = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed);
        //gridLayout->addItem(spacer,0,gridIndex++);
    } else {
        gridLayout->addWidget(label,gridIndex,0,Qt::AlignBottom);
        gridLayout->addLayout(hLayout,gridIndex++,0,Qt::AlignBottom);
        editWidgetCol = 2;
        editWidgetRow = gridIndex;
        gridLayout->addWidget(editWidget,gridIndex++,0,1,2,Qt::AlignBottom);
        //spacer = new QSpacerItem(1,1,QSizePolicy::Fixed,QSizePolicy::Expanding);
        //gridLayout->addItem(spacer,gridIndex++,0);
    }
    switch (labelType) {
    case DateEditType:
    case DateTimeEditType: {
        calendar = new QCalendarWidget();
        minimumDate = calendar->minimumDate();
        connect(calendar,SIGNAL(clicked(QDate)),this,SLOT(calendarClicked(QDate)));
        tBCalendarSelect = new QToolButton();
        tBCalendarSelect->setText("...");
        tBCalendarSelect->setToolTip(tr("open calendar"));
        connect(tBCalendarSelect,SIGNAL(clicked()),this,SLOT(openCalendar()));
        addWidget(tBCalendarSelect);
        if ( layoutDirection == Qt::Vertical ) {
            gridLayout->addItem(new QSpacerItem(10,5,QSizePolicy::MinimumExpanding),editWidgetRow,editWidgetCol++);
        }
    }
    default:;
    }
    //editWidget->setStyleSheet("margin: 0px; padding:0px;");
}
void QLabelEdit::openCalendar(void){
//    currentDateTimeEdit = lEEvent->dateTimeEdit();
    calendar->setSelectedDate(dtEdit->date()/*minimumDate*/);
    calendar->show();
}
void QLabelEdit::calendarClicked(QDate date){
    dtEdit->setDate(date);
    calendar->hide();
}

//QLabelEdit::QLabelEdit(QWidget *parent,LabelType type, int direction) :
//    QWidget(parent)
//{
//    labelType = type;
//    hLayout = new QHBoxLayout();
//    hLayout->setContentsMargins ( 0,0,0,0 );
//    if ( direction == Qt::Horizontal) {
//        xLayout = new QHBoxLayout(this);
//        spacer = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed);
//    } else {
//        xLayout = new QVBoxLayout(this);
//        spacer = new QSpacerItem(1,1,QSizePolicy::Fixed,QSizePolicy::Expanding);
//    }
//    xLayout->setSpacing(0);
//    xLayout->setContentsMargins ( 0,0,0,0 );
//    switch (labelType) {
//    case LineEditType: {
//        lEdit = new QLineEdit(this);
//        lEdit->setObjectName("labelEditLineEdit");
//        hLayout->addWidget(lEdit);
//        break; }
//    case DateEditType: {
//        dtEdit = new QDateEdit(QDate::currentDate(),this);
//        dtEdit->setDisplayFormat("dd.MM.yyyy");
//        dtEdit->setObjectName("labelEditDTEdit");
//        hLayout->addWidget(dtEdit);
//        break; }
//    case TimeEditType: {
//        dtEdit = new QTimeEdit(QTime::currentTime(),this);
//        dtEdit->setDisplayFormat("hh:mm:ss");
//        dtEdit->setObjectName("labelEditDTEdit");
//        hLayout->addWidget(dtEdit);
//        break; }
//    case DateTimeEditType: {
//        dtEdit = new QDateTimeEdit(QDateTime::currentDateTime(),this);
//        dtEdit->setDisplayFormat("dd.MM.yyyy - hh:mm");
//        dtEdit->setObjectName("labelEditDTEdit");
//        hLayout->addWidget(dtEdit);
//        break; }
//    }
//    label = new QLabel(this);
//    label->setObjectName("labelEditLabel");
//    xLayout->addWidget(label);
//    xLayout->addLayout(hLayout);
//    xLayout->addItem(spacer);
//}
void QLabelEdit::addWidget(QWidget *widget){
//    hLayout->addWidget(widget);
    if ( widget->objectName().isEmpty() ) {
        widget->setObjectName("labelEditWidget");
        QToolButton *tButton = qobject_cast<QToolButton*>(widget);
        if ( tButton ) {
            tButton->setIcon(QIcon(":/img/img/go-jump-left.png"));
            tButton->setObjectName("selectButton");
        }

    }
    if ( layoutDirection == Qt::Horizontal) {
        gridLayout->addWidget(widget,0,editWidgetCol++,Qt::AlignBottom);
    } else {
        gridLayout->addWidget(widget,editWidgetRow,editWidgetCol++,Qt::AlignBottom);
    }
}
void QLabelEdit::addStretch(void){
    if ( layoutDirection == Qt::Horizontal) {
        spacer = new QSpacerItem(1,1,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
        gridLayout->addItem(spacer,0,editWidgetCol++);
    } else {
        spacer = new QSpacerItem(1,1,QSizePolicy::Fixed,QSizePolicy::MinimumExpanding);
        gridLayout->addItem(spacer,editWidgetRow,editWidgetCol++);
    }
}

void QLabelEdit::addLabelWidget(QWidget *widget){
    hLayout->addWidget(widget);
}

void QLabelEdit::setWidth(int w) {
    /*lEdit->*/setMinimumWidth(w);
    /*lEdit->*/setMaximumWidth(w);
}
void QLabelEdit::setLabelFont(QFont font){
    label->setFont(font);
}

void QLabelEdit::setLabelText(QString text){
    label->setText(text);
}

void QLabelEdit::setText(QString text){
    switch (labelType) {
    case LineEditType: {
        lEdit->setText(text);
        break;
    }
    case DateEditType: {
        int index = 0;
        QString dateFormat = "dummy";
        while ( !dateFormat.isEmpty() ) {
            dateFormat = vCardDateFormat(index++);
            QDateTime datetime = QDateTime::fromString(text,dateFormat);
            if ( datetime.isValid() || datetime == QDateTime.currentDateTime() ) {
                dtEdit->setDate(datetime.date());
                break;
            }
        }

//        dtEdit->setDate(QDate::fromString(text,VCardDateFormat));
//        dtEdit->setDate(QDate::fromString(text,"yyyyMMdd"));
        break;
    }
    case TimeEditType: {
        dtEdit->setTime(QTime::fromString(text,"hhmmss"));
        break;
    }
    case DateTimeEditType: {
        dtEdit->setDateTime(QDateTime::fromString(text,"yyyyMMdd-hhmmss"));
        break;
    }
    case IntEditType: {
        sbEdit->setValue(text.toInt());
        break;
    }
    }
}

QString QLabelEdit::text(void){
    switch (labelType) {
    case LineEditType:     return lEdit->text();
    case DateEditType:  {
        QString format = vCardDateFormat(0);
        if ( format.isEmpty() ) {
            return dtEdit->dateTime().toString("yyyyMMdd");
        } else {
            return dtEdit->dateTime().toString(format);
        }
    }
    case TimeEditType:     return dtEdit->time().toString("hhmmss");
    case DateTimeEditType: return dtEdit->dateTime().toString("yyyyMMdd-hhmmss");
    case IntEditType:     return QString::number(sbEdit->value());
    }
    return "";
}
QLineEdit *QLabelEdit::lineEdit(void){
    return lEdit;
}
QDateTimeEdit *QLabelEdit::dateTimeEdit(void){
    return dtEdit;
}
QSpinBox *QLabelEdit::spinBoxEdit(void){
    return sbEdit;
}
