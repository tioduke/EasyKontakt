#ifndef VCARDLIST_H
#define VCARDLIST_H

#include <QObject>
#include <QStringList>
#include <QList>

#include "../func.h"
#include "vcarditem.h"
#include "vcard.h"


enum CustomRoles {
    IndexRole = Qt::UserRole
};

enum SortType {
    Sort_FullName,
    Sort_GivenName,
    Sort_FamilyName,
    Sort_None
};

class VCardList : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit VCardList(QObject *parent = 0);
    void sort( int sType, Qt::SortOrder order = Qt::AscendingOrder);

    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const ;
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    int rowCount ( const QModelIndex & parent = QModelIndex()  ) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex());
    void contentUpdate(int row = -1);
    int append(VCard *vCard);
    QStringList contactList(void);
    int count(void);
    bool isChecked(int row);
    bool read(QString fileName = "");
    bool removeOne(VCard *vCard);
    void setChecked(int row,bool checkState);
    void setFileName(QString fileName);
    void setShowCheckBoxes(bool state);
    QStringList sortItems(void); // Sortierung
    SortType sortType(void);
    QString text(void);
    QString text(int row);
    QString value(QString key, int index, int memberIndex = 0);
    VCard *vCard(int index);
signals:
    void sortItAgain(void);
    
public slots:
    void setSortType(int type);

private:
//    QString sortStr(VCard *vCard) const;
//    QString joinString(QString first, QString last, QString separator = " ") const;
    bool parse(void);
    QString vCardFileName;
    bool vCardModified;
    QStringList lines;
    QString rawText;
    QList<VCard *> vCards;
    QStringList items;
    SortType mSortType;
    bool showCheckBoxes;
};

#endif // VCARDLIST_H
