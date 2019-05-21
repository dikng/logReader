#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <QLineEdit>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    adminLog = new AdminLog();
    connect(adminLog,SIGNAL(SendLogListToMainWindows(const QList<Log>&)),this,SLOT(DisplayAll(const QList<Log>&)));
    adminLog->EmitSignalToMainwindowsIfReadSucc();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayOne(const Log& log,int serial)
{
    QTableWidgetItem *itemSerial = new QTableWidgetItem(QString::number(serial + 1));
    QTableWidgetItem *itemLogData = new QTableWidgetItem(log.logData);
    QTableWidgetItem *itemSoftwareVersion = new QTableWidgetItem(log.softwareVersion);
    QTableWidgetItem *itemlogType = new QTableWidgetItem(log.logType);
    QTableWidgetItem *itemlogContent = new QTableWidgetItem(log.logContent);

    ui->tableWidget->setItem(serial,0,itemSerial);
    ui->tableWidget->setItem(serial,1,itemLogData);
    ui->tableWidget->setItem(serial,2,itemSoftwareVersion);
    ui->tableWidget->setItem(serial,3,itemlogType);
    ui->tableWidget->setItem(serial,4,itemlogContent);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);

    QStringList headList;
    headList << QStringLiteral("Serial") << QStringLiteral("Time") << QStringLiteral("SoftwareVersion")
             << QStringLiteral("Type") << QStringLiteral("Content");

    ui->tableWidget->setHorizontalHeaderLabels(headList);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:whitesmoke;}");
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    for(int i = 0;i < 4 ;i++){
        ui->tableWidget->item(serial,i)->setTextAlignment(Qt::AlignCenter);
        if(0 == serial)
            ui->tableWidget->resizeColumnToContents(i);
    }
}

void MainWindow::InitTableHead()
{
    QFont font = ui->tableWidget->horizontalHeader()->font();  //设置表头字体加粗
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置表格不可编辑
    int width = ui ->tableWidget->width();
    ui->tableWidget->setColumnCount(5);

    QStringList headList;
    headList << QStringLiteral("Serial") << QStringLiteral("Time") << QStringLiteral("SoftwareVersion")
             << QStringLiteral("Type") << QStringLiteral("Content");

    ui->tableWidget->setHorizontalHeaderLabels(headList);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:whitesmoke;}");
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);


//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //强行将各个字段的宽度一样

}

void MainWindow::DisplayAll(const QList<Log> &logList)
{

    int serial,row,column;
    serial = 0;
    row = logList.size();
    column = 4;    //默认日志分为四个字段
    this->InitTableHead();
    ui->tableWidget->setRowCount(row);
//    ui->tableWidget->resizeRowsToContents();   //设定所有Row的宽度根据单元格内文本框度自行调整
//    ui->tableWidget->resizeColumnsToContents();
    for( auto  iter = logList.cbegin();iter != logList.cend(); iter++){
        DisplayOne(*iter,serial++);

    }

}

void MainWindow::logFind()
{
    findDialog->show();
}

void MainWindow::findNext()
{
    QString searchKeyWord = lineEdit->text();

    //未完成搜索功能
}
