#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <QLineEdit>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QUrl>
#include <QMimeData>
#include <QPainter>
#include <QString>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    adminLog = new AdminLog();
    Conn();

/*
    adminLog->EmitSignalToMainwindowsIfReadSucc();*/

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayOne(const Log& log,int serial)    //显示log
{
//    qDebug() <<"enter DisplayOne()" << serial;
    QTableWidgetItem *itemSerial = new QTableWidgetItem(QString::number(serial + 1));
    QTableWidgetItem *itemLogData = new QTableWidgetItem(log.logData);
    QTableWidgetItem *itemSoftwareVersion = new QTableWidgetItem(log.softwareVersion);
    QTableWidgetItem *itemlogType = new QTableWidgetItem(log.logType);
    QTableWidgetItem *itemlogContent = new QTableWidgetItem(tr(log.logContent.toStdString().c_str()));

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
        if(2 > serial)
            ui->tableWidget->resizeColumnToContents(i);
    }
}

void MainWindow::InitTableHead()
{
    QFont font = ui->tableWidget->horizontalHeader()->font();  //设置表头字体加粗
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置表格不可编辑
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

void MainWindow::Conn()
{
    connect(adminLog,SIGNAL(SendLogListToMainWindows(const QList<Log>&,int )),this,SLOT(DisplayAll(const QList<Log>&,int)));   //将logList发送到MianWindows中显示
    connect(this,&MainWindow::ReadFile,adminLog,&AdminLog::ReadFile); //用于响应拖拽文件
}

void MainWindow::DisplayAll(const QList<Log> &logList,int start)
{
    qDebug() << QString("进入DispalyAll响应函数");
    int serial,row,column;
    serial = 0;
    row = logList.size();
    column = 4;    //默认日志分为四个字段
    this->InitTableHead();

    ui->tableWidget->setRowCount(row);
    for( auto  iter = logList.cbegin();iter != logList.cend(); iter++){
/*            qDebug() << iter->logData <<"-" <<iter->softwareVersion <<"-"
                     << iter->logType << "-" << iter->logContent <<endl;*/
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

void MainWindow::dragEnterEvent(QDragEnterEvent *event)   //拖动进入事件
{

    if(event->mimeData()->hasUrls())     //拖动的数据中是否含有URL
        event->acceptProposedAction();    //接收事件
    else
        event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)   //放下事件
{

    const QMimeData *mimeData = event->mimeData();   //获取拖动数据
    if(mimeData->hasUrls()){
        QList<QUrl>urlList = mimeData->urls();   //获取文件URl
        QString fileName = urlList.at(0).toLocalFile();
        if(!fileName.isEmpty()){   //不为空则开始讲文件路径传送到指定函数进行读取
            emit ReadFile(fileName);
        }
    }
}
