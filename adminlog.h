#ifndef ADMINLOG_H
#define ADMINLOG_H

#include <QObject>
#include <QString>
#include <string.h>
#include <QFile>
#include <QDebug>
#include <QDataStream>

struct Log    //存储单条日志
{
    Log();
    Log(QString data,QString version,QString type,QString content);

    void SetLogData(QString newLogData){logData = newLogData;}
    QString GetLogData(){return logData;}

    void SetSoftwareVer(QString newSoftwareVer){softwareVersion = newSoftwareVer;}
    QString GetSoftwareVer() {return softwareVersion;}

    void SetLogType(QString newLogType){logType = newLogType;}
    QString GetLogType(){return logType;}

    void SetLogContent(QString newLogContent){logContent = newLogContent;}
    QString GetLogContent(){return logContent;}

    void Output();
    QString logData;
    QString softwareVersion;
    QString logType;
    QString logContent;
};

class AdminLog : public QObject
{
    Q_OBJECT
public:
    explicit AdminLog(QObject *parent = nullptr);
    bool ReadFile(const QString& fileName);    //读取文件中的日志信息
    bool OutPutToXML();  //将特定日志信息以XML格式写到文件中去
    void OutputLogToConsole(const Log& log);
    bool IsNewStr(const QString& newLine);   //判断是当前读入的字符串是否为新的日志记录
    Log* GetNewLog(const QString& str1);    //返回一个含有各字段信息的Log对象的指针
    void EmitSignalToMainwindowsIfReadSucc();
    void OutputToConsole();    //for testing
signals:
    void SendLogListToMainWindows(const QList<Log>& logList,int start);//将数据传送到mainwindows类,start 是logList中开始显示的日志的起始位置
public slots:

private:
    QList<Log> logList;
    int lastIndex;    //更新logList之前的logList中最后一条日志的下标
};

#endif // ADMINLOG_H
