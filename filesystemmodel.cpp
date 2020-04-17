#include "filesystemmodel.h"
#include<QAbstractItemModel >
void FilesystemModel::fetchRootDirectory()
{
    const QFileInfoList drives = QDir::drives();
    qCopy(drives.begin(), drives.end(), std::back_inserter(_nodes));
}

FilesystemModel::FilesystemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    fetchRootDirectory();
}

bool QAbstractItemModel::hasIndex(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || column < 0)
        return false;
    return row < rowCount(parent) && column < columnCount(parent);
}
QModelIndex FilesystemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if (!parent.isValid()) { // запрашивают индексы корневых узлов
        return createIndex(row, column, const_cast<QObject*>(&_nodes[row]));
    }

    QObject* parentInfo = static_cast<QObject*>(parent.internalPointer());
    return createIndex(row, column, &parentInfo->children[row]);
}
QModelIndex FilesystemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    QObject* childInfo = static_cast<QObject*>(child.internalPointer());
    QObject* parentInfo = childInfo->parent;
    if (parentInfo != 0) { // parent запрашивается не у корневого элемента
        return createIndex(findRow(parentInfo), RamificationColumn, parentInfo);
    }
    else {
        return QModelIndex();
    }
}

int FilesystemModel::findRow(const QObject *nodeInfo) const
{
    const NodeInfoList& parentInfoChildren = nodeInfo->parent != 0 ? nodeInfo->parent->children: _nodes;
    NodeInfoList::const_iterator position = qFind(parentInfoChildren, *nodeInfo);
    return std::distance(parentInfoChildren.begin(), position);
}
int FilesystemModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return _nodes.size();
    }
    const QObject* parentInfo = static_cast<const QObject*>(parent.internalPointer());
    return parentInfo->children.size();
}

int FilesystemModel::columnCount(const QModelIndex &) const
{
    return ColumnCount;
}
QVariant FilesystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const NodeInfo* nodeInfo = static_cast<NodeInfo*>(index.internalPointer());
    const QFileInfo& fileInfo = nodeInfo->fileInfo;

    switch (index.column()) {
    case NameColumn:
        return nameData(fileInfo, role);
    case ModificationDateColumn:
        if (role == Qt::DisplayRole) {
            return fileInfo.lastModified();
        }
        break;
    case SizeColumn:
        if (role == Qt::DisplayRole) {
            return fileInfo.isDir()? QVariant(): fileInfo.size();
        }
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant FilesystemModel::nameData(const QFileInfo &fileInfo, int role) const
{
    switch (role) {
    case Qt::EditRole:
        return fileInfo.fileName();
    case Qt::DisplayRole:
        if (fileInfo.isRoot()) {
            return fileInfo.absoluteFilePath();
        }
        else if (fileInfo.isDir()){
            return fileInfo.fileName();
        }
        else {
            return fileInfo.completeBaseName();
        }
    default:
        return QVariant();
    }
    Q_UNREACHABLE();
}
