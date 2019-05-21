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
    void Conn();    //初始化信号与槽的连接

private:
    Ui::MainWindow *ui;
    AdminLog *adminLog;

private slots:
    void DisplayAll(const QList<Log>& logList,int start);

protected:
    void dragEnterEvent(QDragEnterEvent *event);  //拖动进入事件
    void dropEvent(QDropEvent *event);   //放下事件

    //以下函数未使用
    void keyPressEvent(QKeyEvent *event); //键盘按下事件
    void keyReleaseEvent(QKeyEvent *event);  // 键盘释放事件

signals:
    void ReadFile(const QString& fileName);    //包含文件名的信号
};

#endif // MAINWINDOW_H
