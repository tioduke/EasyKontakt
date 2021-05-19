#ifndef STRINGGRIDMODEL_H
#define STRINGGRIDMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>

enum GridOrientation {
    GridRow,
    GridColumn
};

class StringGridModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StringGridModel(QObject *parent = 0);
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

    void expandTableSize(int col, int row);
    void appendRow(QStringList list);
    void appendColumn(QStringList list);
    void setRowCells(int row, QStringList list);
    void setColumnCells(int col, QStringList list);
    void setText(int col, int row, QString newText = "");
    QString text(int col, int row);
    bool textIsValid(void);

    void setHorizontalHeader(QStringList list);
    void setVerticalHeader(QStringList list);

    int columnIndexOf(int row, QString text,int from = 0,Qt::CaseSensitivity cs = Qt::CaseSensitive);
    int rowIndexOf(int col, QString text,int from = 0,Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QModelIndex indexOf(QString text,QModelIndex fromIndex = QModelIndex(),Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString value(QString key,int valueIndex = 0);

    QStringList column(int col);

    void loadGridFromFile(QString fileName, GridOrientation orientation = GridRow);
    void saveGridToFile(QString fileName, GridOrientation orientation = GridRow);
signals:
    
public slots:

private:
    bool isValidColumn(int idx);
    bool isValidRow(int idx);
    bool isValidCell(int col, int row);
    void changeValue(int col, int row, QString text);
    QList<QStringList> table;
    QStringList horizontalHeader;
    QStringList verticalHeader;

    int keyColumn;
    int valueColumn;
    bool textError;

};

#endif // TABLEMODEL_H
