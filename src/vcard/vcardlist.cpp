#include "vcardlist.h"
#include "../defines.h"

static SortType sortingType;

QString joinString(QString first, QString last, QString separator) {
    QString result = first;
    if ( !(result.isEmpty()|last.isEmpty()) ) {
        result += separator;
    }
    return result+last;
}
QString sortStr(VCard *vCard, SortType sortType) {
    QString dataString = "";
    switch (sortType) {
    case Sort_GivenName: {
        dataString =  joinString(vCard->value("N",0,VCardNameGiven),vCard->value("N",0,VCardNameAdditional)," ");
        dataString =  joinString(dataString,vCard->value("N",0,VCardNameFamily)," ");
    } break;
    case Sort_FamilyName: {
        dataString =  joinString(vCard->value("N",0,VCardNameGiven),vCard->value("N",0,VCardNameAdditional)," ");
        QString familyName = vCard->value("N",0,VCardNameFamily);
        if ( familyName.size() ) {
            dataString = joinString(familyName," "+dataString,",");
        } else {
            dataString = " "+dataString;
        }
    }break;
    case Sort_FullName:
    case Sort_None:
    default: dataString = vCard->value("FN");
    }
    return dataString;
}

bool lessThan( VCard *s1, VCard *s2){
    return sortStr(s1,sortingType) < sortStr(s2,sortingType);
}
bool moreThan( VCard *s1, VCard *s2){
    return sortStr(s1,sortingType) > sortStr(s2,sortingType);
}


VCardList::VCardList(QObject *parent) :
    QAbstractListModel(parent)
{
    vCardFileName = "";
    rawText = "";
    vCardModified = false;
    mSortType = Sort_None;
    showCheckBoxes = false;
}
void VCardList::sort( int /*sType*/, Qt::SortOrder order){
    sortingType = mSortType;
    if ( order == Qt::AscendingOrder) {
        qStableSort(vCards.begin(),vCards.end(),lessThan);
    } else {
        qStableSort(vCards.begin(),vCards.end(),moreThan);
    }
    //emit layoutChanged();
    contentUpdate();
}

bool VCardList::isChecked(int row){
    return vCards[row]->isChecked();
}
void VCardList::setChecked(int row,bool checkState){
    emit layoutAboutToBeChanged();
    vCards[row]->setChecked(checkState);
    emit layoutChanged();
}

void VCardList::setShowCheckBoxes(bool state){
    emit layoutAboutToBeChanged();
    showCheckBoxes = state;
    emit layoutChanged();
}

int VCardList::rowCount ( const QModelIndex & /*parent*/ ) const {
    return vCards.count();
}
QModelIndex VCardList::index(int row, int column, const QModelIndex & parent){
    return hasIndex(row, column, parent) ? createIndex(row, column)
        : QModelIndex();
}
void VCardList::contentUpdate(int row){
    if ( row == -1 ) {
        emit dataChanged(index(0,0),index(rowCount(),0));
    } else {
        QModelIndex rowIndex = index(row,0);
        if ( rowIndex.isValid()) {
            emit dataChanged(rowIndex,rowIndex);
        }
    }
}


QVariant VCardList::data ( const QModelIndex & index, int role  ) const{
    int row = index.row();
    if ( role == Qt::DisplayRole) {
        QString dataString = "";
        if ( index.column() == 0 ) {
            dataString = sortStr(vCards[row],mSortType);
        } else {
            dataString = QString::number(row);
        }
        return dataString.toUpper();
    } else if ( role == Qt::CheckStateRole) {
        if ( showCheckBoxes ) {
            if ( vCards[index.row()]->isChecked() ) {
                return Qt::Checked;
            }
            return Qt::Unchecked;
        }
    } else if ( role == IndexRole) {
        return row;
    }
    return QVariant();
}
bool VCardList::setData(const QModelIndex &index, const QVariant &value, int role){
    if ( role == Qt::CheckStateRole ) {
        int row = index.row();
        vCards[row]->setChecked(!vCards[row]->isChecked());
        emit dataChanged(index, index);
        return true;
    }
    return QAbstractListModel::setData(index, value, role);
}

bool VCardList::removeRows(int row, int count, const QModelIndex & parent){
    if ( row < vCards.count() ) {
        emit layoutAboutToBeChanged();
        beginRemoveRows(parent,row,row+(count-1));
        for ( int i = 0; i < count; i++) {
            delete vCards.takeAt(row);
        }
        endRemoveRows();
        emit layoutChanged();
        emit sortItAgain();
        return true;
    }
    return false;
}

bool VCardList::removeOne(VCard *vCard){
    for ( int index = 0; index < vCards.count() ; index++) {
        if ( vCards[index] == vCard ) {
            return removeRows(index,1);
        }
    }
    return false;
}
bool VCardList::insertRows(int row, int count, const QModelIndex & parent){
    if ( row < vCards.count() ) {
        emit layoutAboutToBeChanged();
        beginInsertRows(parent,row,row+(count-1));
        vCards.insert(row,NULL);  // Hier fehlt eien gültige VCard
        endInsertRows();
        emit layoutChanged();
        emit sortItAgain();
        return true;
    }
    return false;
}

int VCardList::append(VCard *vCard){
    int row = vCards.count();
    emit layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(),row,row);
    vCards.insert(row,vCard);
    endInsertRows();
    emit layoutChanged();
    emit sortItAgain();
    return row;
}

void VCardList::setFileName(QString fileName){
    if ( !fileName.isEmpty() ) {
        vCardFileName = fileName;
    }
}
bool VCardList::read(QString fileName) {
    if ( !fileName.isEmpty() ) {
        vCardFileName = fileName;
    }
    if ( vCardFileName.isEmpty() ) {
        return false;
    }
    rawText = loadFromFile(vCardFileName);
    if ( !parse() ) {
        emit layoutChanged();
        return false;
    }
    vCardModified = false;
    emit layoutChanged();
    return true;
}

bool VCardList::parse(void){
    bool result = true;
    if ( rawText.isEmpty() ) {
        return false;
    }
    lines = rawText.split("\r\n");
    if ( lines.count() == 1) {
        lines = rawText.split("\n");
    }
    for (int lineNo = 0; lineNo < lines.count(); lineNo++) {
        int pos;
        QString line = lines[lineNo].toUpper();
        if ( (pos = line.indexOf("BEGIN")) > -1) {
            if ( (line.indexOf("VCARD")) > pos) {
                VCard *vCard = new VCard(this);
                lineNo = vCard->parse(&lines,lineNo+1);
                if ( vCard->valid() ) {
                    vCards << vCard;
                    vCard->setChecked(true);
                } else {
                    delete vCard;
                    if ( lineNo < 0) {
                        return false;
                    }
                }
            }
        }
    }
    return result;
}

QString VCardList::text(void) {
    return rawText;
}
QString VCardList::text(int row) {
    return vCards[row]->value("FN");
}
QStringList VCardList::contactList(void) {
    items.clear();
    foreach(VCard *vCard,vCards) {
        items << vCard->value("FN");

    }
    return items;
}
int VCardList::count(void) {
    return vCards.count();
}
QString VCardList::value(QString key, int index, int memberIndex){
    return vCards[index]->value(key,0,memberIndex);
}
VCard *VCardList::vCard(int index){
    if ((index > -1) & (index < vCards.count())) {
        return vCards[index];
    }
    return NULL;
}
SortType VCardList::sortType(void){
    return mSortType;
}
void VCardList::setSortType(int type){
    mSortType = (SortType) type;
    //emit modelReset();
    /*emit sortItAgain();*/
//    beginResetModel();
    sort(0);
//    endResetModel();
//    emit layoutChanged();
}

//enum SortType {
//    Sort_None,
//    Sort_FullName,
//    Sort_GivenName,
//    Sort_FamilyName
//};
QStringList VCardList::sortItems(void){
    QStringList result;
    result /*<< tr("Sort: None")*/ << tr("Sort: Full Name") << tr("Sort: Given Name") << tr("Sort: Family Name");
    return result;
}
