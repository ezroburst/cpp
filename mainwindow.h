#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QFileSystemModel"
#include "QDir"
#include "QGridLayout"
#include "QtCharts/QPieSeries"
#include "QtCharts/QChartView"
#include "QtCharts/QChart"
#include "strategy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void PrintTable(Ui::MainWindow *ui,Element element);
    void PrintPieChart(Ui::MainWindow *ui,Element element);
    void PrintBarChart(Ui::MainWindow *ui,Element element);
public slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_exit_triggered();

    void on_aboutApp_triggered();

    void reprint(const QModelIndex &, const QModelIndex &);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;

};

#endif // MAINWINDOW_H
