#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include<QFileInfo>
#include<QVector>
#include <QAbstractItemModel>

class FilesystemModel
{

    enum Columns
    {
        RamificationColumn, // столбец, по которому производится ветвление, всегда первый.
                            // Другого варианта не поддерживает QTreeView
        NameColumn = RamificationColumn, // столбец с именем узла
        ModificationDateColumn, // столбец с датой изменения узла
        SizeColumn, // столбец с размером файла
        ColumnCount // число столбцов
    };
public:

    FilesystemModel();
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
     QAbstractItemModel hasIndex(int row, int column, const QModelIndex &parent) const;
    QFileInfo fileInfo; // информация об узле
    QVector<QVariant> children; // список дочерних узлов
    QVariant* Parent; // ссылка на родительский узел
    typedef QVector<QVariant> NodeInfoList;
    NodeInfoList _nodes; // список корневых узлов файловых систем
    bool mapped; // производился ли поиск дочерних узлов.
};

#endif // FILESYSTEMMODEL_H
