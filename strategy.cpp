#include "strategy.h"

//размер вложенных папок
quint32 listFolder ( QString path )
{
    QDir currentFolder( path );

    quint32 totalsize = 0;

    currentFolder.setFilter( QDir::AllEntries);
    currentFolder.setSorting( QDir::Name );

    QFileInfoList folderitems( currentFolder.entryInfoList() );

    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;

        if ( i.isDir() )
            totalsize += listFolder( path+"/"+iname );
        else
            totalsize += i.size();
    }
    return totalsize;
}

//размер текущей папки
quint32 sizeFolder ( QString path )
{
    QDir currentFolder( path );

    quint32 totalsize = 0;

    currentFolder.setFilter( QDir::AllEntries);
    currentFolder.setSorting( QDir::Name );

    QFileInfoList folderitems( currentFolder.entryInfoList() );

    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;
        totalsize += i.size();
    }
    return totalsize;
}

//рамер файлов по их типам
Element sizeType (QString path, Element tmp)
{
    QDir currentFolder( path );
    currentFolder.setFilter( QDir::AllEntries);
    currentFolder.setSorting( QDir::Name );

    QFileInfoList folderitems( currentFolder.entryInfoList() );

    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;
        if(i.isDir())
        {
            tmp = sizeType(path+"/"+iname, tmp);
        }
        if(i.isFile())
        {
            if(tmp.elementName.empty())
            {
                tmp.elementName<<i.completeSuffix();
                tmp.elementSize<<i.size();
            }
            else
            {
                bool chek = false;
                foreach (QString str, tmp.elementName)
                {
                    if(i.completeSuffix() == str)
                    {
                        tmp.elementSize[tmp.elementName.indexOf(str,0)] += i.size();
                        chek=true;
                    }
                }
                if(!chek)
                {
                    tmp.elementName<<i.completeSuffix();
                    tmp.elementSize<<i.size();
                }
            }
        }
    }
    return tmp;
}


//стретегия по папкам
void folderSrtategy::UseStrategy(const QModelIndex &index, QFileSystemModel *model, Element& element)
{
    element.elementName.clear();
    element.elementSize.clear();

    QFileInfo fileinfo = model->fileInfo(index);
    QDir currentFolder( fileinfo.filePath() );
    currentFolder.setFilter( QDir::Dirs);
    currentFolder.setSorting( QDir::Name );
    QFileInfoList folderitems( currentFolder.entryInfoList() );
    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;

        if ( i.isDir() )
        {
            element.elementName<<iname;
            element.elementSize<<listFolder(fileinfo.filePath()+"/"+iname);
        }
        else
            continue;
    }
    //размер файлов текущей папки
    element.elementName<<"current folder";
    element.elementSize<<sizeFolder(fileinfo.filePath());
}

//стратегия по типам файлов
void fileTypeStrategy::UseStrategy(const QModelIndex &index, QFileSystemModel *model, Element& element)
{
    element.elementName.clear();
    element.elementSize.clear();
    QFileInfo fileinfo = model->fileInfo(index);
    QDir currentFolder( fileinfo.filePath() );
    currentFolder.setFilter( QDir::AllEntries);
    currentFolder.setSorting( QDir::Name );
    QFileInfoList folderitems( currentFolder.entryInfoList() );
    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;
        if(i.isFile())
        {
            if(element.elementName.empty())
            {
                element.elementName<<i.completeSuffix();
                element.elementSize<<i.size();
            }
            else
            {
                bool chek = false;
                foreach (QString str, element.elementName)
                {
                    if(i.completeSuffix() == str)
                    {
                        element.elementSize[element.elementName.indexOf(str,0)] += i.size();
                        chek=true;
                    }
                }
                if(!chek)
                {
                    element.elementName<<i.completeSuffix();
                    element.elementSize<<i.size();
                }
            }
        }
        if(i.isDir())
        {
            element = sizeType(fileinfo.filePath()+"/"+iname, element);
        }
    }
}
