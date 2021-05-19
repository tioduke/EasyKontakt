#include "stringgridmodel.h"
#include "func.h"

static int columnToSort;
bool lessThan(const QStringList s1,const QStringList s2){
    return s1.at(columnToSort).toLower() < s2.at(columnToSort).toLower();
}
bool moreThan(const QStringList s1,const QStringList s2){
    return s1.at(columnToSort).toLower() > s2.at(columnToSort).toLower();
}


StringGridModel::StringGridModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    keyColumn = 0;
    valueColumn = 1;
}
int StringGridModel::rowCount ( const QModelIndex & /*parent*/) const{
    return table.count();
}

int StringGridModel::columnCount ( const QModelIndex & /*parent*/) const{
    if ( rowCount() ) {
        return table[0].count();
    }
    return 0;
}

QVariant StringGridModel::data ( const QModelIndex & idx, int role ) const{
    int row = idx.row();
    int col = idx.column();
    if ( role == Qt::DisplayRole) {
        if ( row < table.count() ) {
            if ( col < table[row].count() ) {
                return table[row][col];
            }
        }
    }
    return QVariant();
}

QVariant StringGridModel::headerData ( int section, Qt::Orientation orientation, int role ) const{
    if ( role == Qt::DisplayRole) {
        if ( orientation == Qt::Horizontal ) {
            if ( section < horizontalHeader.count() ) {
                return horizontalHeader[section];
            }
            return QString::number(section);
        } else {
            if ( section < verticalHeader.count() ) {
                return verticalHeader[section];
            }
            return QString::number(section);
        }
    }
    return QVariant();
}

void StringGridModel::sort ( int column, Qt::SortOrder order){
    //QList<QStringList> table;
    columnToSort = column;
    if ( order == Qt::AscendingOrder) {
        qStableSort(table.begin(),table.end(),lessThan);
    } else {
        qStableSort(table.begin(),table.end(),moreThan);
    }
    emit layoutChanged();
}
void StringGridModel::changeValue(int col, int row, QString text){
    (table[row])[col] = text;
    QModelIndex modelIndex = index(row,col,QModelIndex());
    emit dataChanged(modelIndex,modelIndex);
}

void StringGridModel::setText(int col, int row, QString newText){
    expandTableSize(col,row);
    changeValue(col,row,newText);
}
bool StringGridModel::textIsValid(void){
    return !textError;
}

QString StringGridModel::text(int col, int row){
    if ( isValidCell(col,row)) {
        textError = false;
        return table[row][col];
    }
    textError = true;
    return tr("Error:Col[%1]Row[%2]out of range").arg(QString::number(col),QString::number(row));
}

void StringGridModel::expandTableSize(int col, int row){
    bool rowSizeChanged = false;
    bool colSizeChanged = false;

    int count = row - rowCount();
    if ( count > -1 ) {
        int first = rowCount();
        beginInsertRows(QModelIndex(),first,first+count);
        rowSizeChanged = true;
        QStringList list;
        for ( int idx = 0; idx < columnCount(); idx++) {
            list << "";
        }
        while ( row >= rowCount() ) {
            table << list;
        }
    }
    count = col - columnCount();
    if ( count > -1 ) {
        int first = columnCount();
        beginInsertColumns(QModelIndex(),first,first+count);
        colSizeChanged = true;
        for ( int idx = 0; idx < rowCount(); idx++) {
            while ( col >= table[idx].count() ) {
                table[idx] << "";
                colSizeChanged = true;
            }
        }
    }
    if ( colSizeChanged ) endInsertColumns();
    if ( rowSizeChanged ) endInsertRows();
}
void StringGridModel::appendRow(QStringList list){
    int lastRow = rowCount();
    int columns = columnCount();
    if ( list.count() > columnCount() ) {
        columns = list.count();
    }
    expandTableSize(columns-1,rowCount());
    for ( int col = 0; col < list.count(); col++) {
        changeValue(col,lastRow,list[col]);
        //table[lastRow][col] = list[col];
    }
}

void StringGridModel::appendColumn(QStringList list){
    int lastColumn = columnCount();
    expandTableSize(lastColumn,list.count()-1);
    for ( int row = 0; row < list.count(); row++) {
        changeValue(lastColumn,row,list[row]);
        //table[row][lastColumn] = list[row];
    }
}
void StringGridModel::setRowCells(int row, QStringList list){
    int col = columnCount();
    if ( col < list.count() ) {
        col = list.count();
    }
    if ( row >= rowCount() ) {
        expandTableSize(col-1,row);
    }
    for ( int col = 0; col < list.count(); col++) {
        changeValue(col,row,list[col]);
        //table[row][col] = list[col];
    }
}

void StringGridModel::setColumnCells(int col, QStringList list){
    int row = rowCount();
    if ( row < list.count() ) {
        row = list.count();
    }
    if ( col >= columnCount() ) {
        expandTableSize(col, row-1);
    }
    for ( int idx = 0; idx < list.count(); idx++ ) {
        changeValue(col,idx,list[idx]);
        //table[idx][col] = list[idx];
    }
}

void StringGridModel::setHorizontalHeader(QStringList list){
    horizontalHeader.clear();
    horizontalHeader.append(list);
    if ( list.count() > columnCount() ) {
        expandTableSize(list.count()-1,rowCount()-1);
    }
}

void StringGridModel::setVerticalHeader(QStringList list) {
    verticalHeader.clear();
    verticalHeader.append(list);
    if ( list.count() > rowCount() ) {
        expandTableSize(columnCount()-1,list.count()-1);
    }
}

int StringGridModel::columnIndexOf(int row, QString text,int from ,Qt::CaseSensitivity cs){
    if ( isValidRow(row) ) {
        if ( cs == Qt::CaseInsensitive) {
            text = text.toUpper();
        }
        for ( int idx = from; idx < columnCount(); idx++) {
            QString ref = table[row][idx];
            if ( cs == Qt::CaseInsensitive) {
                ref = ref.toUpper();
            }
            if ( ref == text ) {
                return idx;
            }
        }
    }
    return -1;
}
int StringGridModel::rowIndexOf(int col, QString text,int from,Qt::CaseSensitivity cs){
    if ( isValidColumn(col) ) {
        if ( cs == Qt::CaseInsensitive) {
            text = text.toUpper();
        }
        for ( int idx = from; idx < rowCount(); idx++) {
            QString ref = table[idx][col];
            if ( cs == Qt::CaseInsensitive) {
                ref = ref.toUpper();
            }
            if ( ref == text ) {
                return idx;
            }
        }
    }
    return -1;
}
QModelIndex StringGridModel::indexOf(QString text,QModelIndex fromIndex,Qt::CaseSensitivity cs){
    int row = 0;
    int col = 0;
    if (fromIndex.isValid() ) {
        row = fromIndex.row();
        col = fromIndex.column();
    }
    if ( isValidCell(col,row) ) {
        if ( cs == Qt::CaseInsensitive) {
            text = text.toUpper();
        }
        /// read row for row
        col++;
        for ( int r = row; r < rowCount(); r++) {
            for ( int c = col; c < columnCount(); c++) {
                QString ref = table[r][c];
                if ( cs == Qt::CaseInsensitive ) {
                    ref = ref.toUpper();
                }
                if ( ref == text ) {
                    return index(r,c,QModelIndex());
                }
            }
            col = 0;
        }
    }
    return QModelIndex();
}

QString StringGridModel::value(QString key,int valueIndex){
    int col = valueColumn+valueIndex;
    int row = rowIndexOf(keyColumn,key);
    return text(col,row);
}

bool StringGridModel::isValidColumn(int idx){
    return ( idx > -1) & (idx < columnCount());
}

bool StringGridModel::isValidRow(int idx){
    return ( idx > -1) & (idx < rowCount());
}
bool StringGridModel::isValidCell(int col, int row){
    return isValidRow(row) & isValidColumn(col);
}

QStringList StringGridModel::column(int col){
    if ( isValidColumn(col) ) {
        QStringList list;
        for ( int idx = 0; idx < rowCount(); idx++) {
            list << table[idx][col];
        }
        return list;
    }
    return QStringList();
}

void StringGridModel::loadGridFromFile(QString fileName, GridOrientation orientation){
    table.clear();
    QString text = loadFromFile(fileName);
    QStringList lines = text.split("\n");
    foreach ( QString line,lines) {
        if ( line.trimmed().size() ) {
            if ( orientation == GridRow ) {
                appendRow(line.split(";"));
            } else {
                appendColumn(line.split(";"));
            }
        }
    }
}

void StringGridModel::saveGridToFile(QString fileName, GridOrientation orientation){
    QString text = "";
    if ( orientation == GridRow ) {
        for ( int row = 0; row < rowCount(); row++) {
            text += table[row].join(";")+"\n";
        }
    } else {
        for ( int col = 0; col < columnCount(); col++) {
            QString separator = "";
            for ( int row = 0; row < rowCount(); row++) {
                text += separator+table[row][col];
                separator = ";";
            }
            text += "\n";
        }
    }
    saveToFile(fileName,text);
}
