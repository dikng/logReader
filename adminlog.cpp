#include "adminlog.h"
#include <QDataStream>
#include <QFile>
#include <QString>
#include <iostream>
#include <QRegularExpression>
#include <QDebug>

AdminLog::AdminLog(QObject *parent) : QObject(parent)
{
    lastIndex = 0;
}

/*   //primitive version
bool AdminLog::ReadFile()
{
    char space = ' ';
    bool flag;
    QString str2;

    QFile file("E:\\Qt_Creator'_Code\\logReader\\all_info.txt");
//    QFile file("U:\\QtFile\\logReader\\all_info.txt");
    if(!file.open(QIODevice::ReadOnly ))
        qDebug() << file.errorString();
    QTextStream in(&file);

    while(!file.atEnd())
//    for(int i= 0; i< 10000&&(!file.atEnd());i++)
    {
        QString str = nullptr;
        in >> str;

        if(!(flag = IsNewStr(str))){
            str2 = str2 + space +str;
        }
        else{
            Log *temp = GetNewLog(str2);
            if(nullptr != temp)
                logList.append(*temp);
            str2 = str;
        }

    }
//    this->OutputToConsole();
    return true;
}*/


/*使用atEnd()作为读文件结束条件并不能将整个文件读入，
 * 遗漏的行数和文件内总行数成正比
 */
bool AdminLog::ReadFile(const QString& fileName)
{
    char space = ' ';
    bool flag;
    QString str2;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly ))
        qDebug() << file.errorString();
    QTextStream in(&file);
    while(!file.atEnd())
    {
        QString str = nullptr;
        in >> str;
        if(!(flag = IsNewStr(str))){
            str2 = str2 + space +str;
        }
        else{
            Log *temp = GetNewLog(str2);
            if(nullptr != temp)
                logList.append(*temp);
            str2 = str;
        }

    }
    file.close();
    this->OutputToConsole();
    emit SendLogListToMainWindows(logList,lastIndex);
    lastIndex = lastIndex + logList.size() - 1;

    return true;
}

bool AdminLog::IsNewStr(const QString &newLine)
{
    QRegularExpression re("\\d{4}-\\d{2}-\\d{2}");
    QRegularExpressionMatch match = re.match(newLine);
    if(match.hasMatch())
        return true;
    else
        return false;
}

Log *AdminLog::GetNewLog(const QString &str1)
{
    QString pattern("\\[([-0-9\\s:,]+)\\]\\[([0-9\\.]+)\\]([a-zA-Z]+)\\s(.+)");
    QRegularExpression re(pattern);
    QRegularExpressionMatch match = re.match(str1);
    if(match.hasMatch()){
        Log *newLog = new Log();
        newLog->logData = match.captured(1);
        newLog->softwareVersion = match.captured(2);
        newLog->logType = match.captured(3);
        newLog->logContent = match.captured(4);
        return newLog;
    }
    else
        return nullptr;
}

void AdminLog::EmitSignalToMainwindowsIfReadSucc()
{
    //被废弃
}

void AdminLog::OutputToConsole()
{
    for(auto iter = logList.begin();iter != logList.end();iter++){
        iter->Output();
    }
}

Log::Log()
{
    this->logData = nullptr;
    this->softwareVersion = nullptr;
    this->logType = nullptr;
    this->logContent = nullptr;
}

Log::Log(QString data, QString version, QString type, QString content)
{
    this->SetLogData(data);
    this->SetSoftwareVer(version);
    this->SetLogType(type);
    this->SetLogContent(content);
}

void Log::Output()
{
    qDebug() << logData <<softwareVersion << logType  << logContent <<endl;
}

