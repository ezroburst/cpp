#include "QBarSeries"
#include "QBarCategoryAxis"
#include "QValueAxis"
#include "vector"
#include <QVector>
#include <QTableView>
#include <QMessageBox>
#include "strategy.h"
#include <QAbstractItemModel>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include "QTableView"
#include "QDir"
#include "QList"
#include "QGridLayout"
#include "QtCharts/QPieSeries"
#include "QtCharts/QChartView"
#include "QtCharts/QChart"
#include "QBarSet"

void printt(const QModelIndex &, const QModelIndex &)
{
    QMessageBox m;
    m.setText("text");
    m.show();
}

Element element;
using namespace QtCharts;
typedef QList<QString> DirList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
    model->setRootPath(QDir::rootPath());

    ui->treeView->setModel(model);

//    for(int i=1; i<model->columnCount();i++)
//    {
//        ui->treeView->hideColumn(i);//отражаем в дереве только имена
//    }

    ui->comboBox->addItem("по папкам");
    ui->comboBox->addItem("по файлам");

    ui->comboBox_2->addItem("таблица");

    ui->widget->setContentsMargins(0,0,0,0);
    ui->treeView->model()->dataChanged(ui->treeView->currentIndex(), ui->treeView->currentIndex());
    QObject::connect(model,
            SIGNAL(dataChanged ( const QModelIndex &, const QModelIndex &)),
            this, SLOT(reprint(const QModelIndex &, const QModelIndex &)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


//сортировка элементов по возрастанию размера
void sortElement(Element& element)
{
    Element tmp = element;
    std::sort(element.elementSize.begin(), element.elementSize.end());
    for(int i=0; i<element.elementName.size(); i++)
    {
        for(int j=0; j<tmp.elementName.size(); j++)
        {
            if(element.elementSize[i]==tmp.elementSize[j])
            {
                if(element.elementName[i] != tmp.elementName[j])
                {
                    element.elementName[i] = tmp.elementName[j];
                    tmp.elementSize.remove(j);
                    tmp.elementName.removeAt(j);
                    break;
                }
            }
        }
    }
}

//выборка топ 5
void TopElement(Element& element)
{
    Element tmp;
    tmp.elementName.clear();
    tmp.elementSize.clear();
    if(element.elementName.size()>5)
    {
        tmp.elementName<<"other";
        tmp.elementSize<<0;
        for(int i=0; i<element.elementName.size()-4; i++)
        {
            tmp.elementSize[0]+=element.elementSize[i];
        }
        for(int i=element.elementName.size()-4; i<element.elementName.size(); i++)
        {
            tmp.elementName<<element.elementName[i];
            tmp.elementSize<<element.elementSize[i];
        }
        element.elementName.clear();
        element.elementSize.clear();
        element = tmp;
    }
}
//клик на дерево
void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QFileInfo fileinfo = model->fileInfo(index);
    if(ui->comboBox->currentText()=="по папкам")
    {
        strategy* p = new  strategy(new folderSrtategy);
        p->UseStrategy(ui->treeView->currentIndex(), model, element);
        if(ui->comboBox_2->currentText()=="таблица")
            PrintTable(ui, element);

        delete p;
    }
    if(ui->comboBox->currentText()=="по файлам")
    {
        strategy* p = new  strategy(new fileTypeStrategy);
        p->UseStrategy(ui->treeView->currentIndex(), model, element);
        if(ui->comboBox_2->currentText()=="таблица")
            PrintTable(ui, element);

        delete p;
    }
}
QVBoxLayout *vlayout = new QVBoxLayout;
QHBoxLayout *hlayout = new QHBoxLayout;
//вывод информации в виде таблицы
void MainWindow::PrintTable(Ui::MainWindow *ui, Element element)
{

    sortElement(element);
    TopElement(element);
    QTableView * tableView;
    tableView = new QTableView(ui->widget);
    //Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("Имя");
    horizontalHeader.append("Размер");
    QStandardItemModel *Tmodel = new QStandardItemModel;
    QStandardItem *item;
    Tmodel->setHorizontalHeaderLabels(horizontalHeader);
    int rowcounter = 0;
    for(int i=0; i<element.elementSize.size(); i++)
    {
        if(element.elementSize[i]!=0)
        {
            item = new QStandardItem(element.elementName[i]);
            Tmodel->setItem(rowcounter, 0, item);

            item = new QStandardItem(QString::number(element.elementSize[i]));
            Tmodel->setItem(rowcounter, 1, item);
            rowcounter++;
        }
    }
    tableView->setModel(Tmodel);
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);

    //QVBoxLayout *vlayout = new QVBoxLayout;
    //QHBoxLayout *hlayout = new QHBoxLayout;
    if(vlayout->count()!=0)
    {
        while(vlayout->count())
        {
          QLayoutItem * item = vlayout->itemAt(0);
          vlayout->removeItem(item);
        }
    }
    vlayout->setMargin(0);
    hlayout->setMargin(0);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(tableView);
    ui->widget->setLayout(vlayout);

}


//изменение вида вывода информации
void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if(arg1=="таблица")
    {
        PrintTable(ui, element);
        ui->statusBar->showMessage("выбранна таблица");
    }

}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1=="по папкам")
    {
        strategy* p = new  strategy(new folderSrtategy);
        p->UseStrategy(ui->treeView->currentIndex(), model, element);
        if(ui->comboBox_2->currentText()=="таблица")
            PrintTable(ui, element);

        delete p;
    }
    if(arg1=="по файлам")
    {
        strategy* p = new  strategy(new fileTypeStrategy);
        p->UseStrategy(ui->treeView->currentIndex(), model, element);
        if(ui->comboBox_2->currentText()=="таблица")
            PrintTable(ui, element);


        delete p;
    }
}


void MainWindow::on_exit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_aboutApp_triggered()
{
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setWindowTitle("О программе");
    msgBox->setText("Какая то информация о программе");
    msgBox->exec();
    delete msgBox;
}

void MainWindow::reprint(const QModelIndex & index, const QModelIndex & index1)
{
    QFileInfo fileinfo = model->fileInfo(index);
    if(ui->comboBox->currentText()=="по папкам")
    {
        strategy* p = new  strategy(new folderSrtategy);
        p->UseStrategy(ui->treeView->currentIndex(), model, element);
        if(ui->comboBox_2->currentText()=="таблица")
            PrintTable(ui, element);


        delete p;
    }
    if(ui->comboBox->currentText()=="по файлам")
    {
        strategy* p = new  strategy(new fileTypeStrategy);
        p->UseStrategy(ui->treeView->currentIndex(), model, element);
        if(ui->comboBox_2->currentText()=="таблица")
            PrintTable(ui, element);


        delete p;
    }
}
