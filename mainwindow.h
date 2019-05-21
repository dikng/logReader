#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adminlog.h"
namespace Ui {
class MainWindow;
}

class QLineEdit;
class QDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void DisplayOne(const Log& log,int serial);
    void InitTableHead();    //初始化日志的字段名称

private:
    Ui::MainWindow *ui;
    QLineEdit *lineEdit;

//暂时未使用
    QDialog *findDialog;
    AdminLog *adminLog;
//暂时未使用

private slots:
    void DisplayAll(const QList<Log>& logList );
    void logFind();
    void findNext();
};

#endif // MAINWINDOW_H
