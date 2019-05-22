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
void AdminLog::ReadFile(const QString& fileName)   //根据文件名选择对应的文件读取方式
{
    QString patternInfo = {".+?_info.*.txt\\b"};
    QString patternFatal = {".+?_fatal.*.txt\\b"};
    QString patternMessage = {".+?_message.*.txt\\b"};

    QRegularExpression reInfo(patternInfo);
    QRegularExpression reFatal(patternFatal);
    QRegularExpression reMessage(patternMessage);

    if(reInfo.match(fileName).hasMatch())
        ReadFile_info(fileName);
    else if(reMessage.match(fileName).hasMatch())
        ReadFile_message(fileName);
    else if(reFatal.match(fileName).hasMatch())
        ReadFile_fatal(fileName);
    else
        qDebug() << QString("can not read the file");
}

bool AdminLog::IsNewStr_info(const QString &newLine)    //判断newLine是否为一条心的日志
{
    QRegularExpression re("\\d{4}-\\d{2}-\\d{2}");
    QRegularExpressionMatch match = re.match(newLine);
    if(match.hasMatch())
        return true;
    else
        return false;
}

Log *AdminLog::GetNewLog_info(const QString &str1)
{
    QString pattern("\\[([-0-9\\s:,]+)\\]\\[([0-9\\.]+)\\]([a-zA-Z]+)\\s(.+)");
    QRegularExpression re(pattern);
    QRegularExpressionMatch match = re.match(str1);
    if(match.hasMatch()){
        Log *newLog = new Log();
        newLog->logData = match.captured(1);
        newLog->softwareVersion = match.captured(2);
        newLog->logLevel = match.captured(3);
        newLog->logContent = match.captured(4);
        return newLog;
    }
    else
        return nullptr;
}

Log *AdminLog::GetNewLog_message(const QRegularExpressionMatch &match)    //返回一个新的日志对象
{
    Log* newLog = new Log();
    newLog->SetSoftwareVer(match.captured(1));
    newLog->SetLogData(match.captured(2));
    newLog->SetThreadId(match.captured(3));
    newLog->SetlogLevel(match.captured(4));
    newLog->SetErrorCategories(match.captured(5));
    newLog->SetLogContent(match.captured(6));

    return newLog;
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

void AdminLog::ReadFile_info(const QString &fileName)   //读取_info.txt的日志文件
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
        if(!(flag = IsNewStr_info(str))){
            str2 = str2 + space +str;
        }
        else{
            Log *temp = GetNewLog_info(str2);
            if(nullptr != temp)
                logList.append(*temp);
            str2 = str;
        }

    }
    file.close();
    this->OutputToConsole();
    emit SendLogListToMainWindows(logList,lastIndex);    //将日志存入logList之后，发送显示信号到mainwindow对象进行日志显示
    lastIndex = logList.size() - 1;

}

void AdminLog::ReadFile_message(const QString &fileName)    //读取_message.txt的日志文件
{
    QFile file(fileName);
    QString str;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << file.errorString();
    QTextStream in(&file);
    str = in.readAll();

    str.replace(QString("\n"), QString(" "));   //清除日志记录中的特定字符，方便后面进行匹配
    str.replace(QString("*"),QString(""));

//    qDebug() << str;
    PacketCapture_message(str + " ");   //末尾添加空格是为了解决无法捕获最后一条日志的问题

    file.close();
//    this->OutputToConsole();
    emit SendLogListToMainWindows(logList,lastIndex);    //将日志存入logList之后，发送显示信号到mainwindow对象进行日志显示
    lastIndex =logList.size() - 1;
}

void AdminLog::ReadFile_fatal(const QString &fileName)
{
    qDebug() << fileName << endl;
}

void AdminLog::PacketCapture_message(const QString &str)
{
//    QString patternData= {"\\s([0-9\\.]+)\\r\\n.+?\\s([-0-9\\s:,]+)\\s*.+?(\\d+).+?"};
//\s([0-9\.]+)\s*?.+?\s([-0-9\s:,]+)\s.+?(\d+)\]\s*?.+?([a-zA-Z]+)\s*.+?\s(.+)\s*?.+?\s([a-zA-Z]+)\s*(.*)\s*?  用于在线检测
//QString patternData= {"\\s([0-9\\.]+)\\s*?.+?\\s([-0-9\\s:,]+).+?(\\d+)\\]\\s*?.+?([a-zA-Z]+)\\s*.+?\\s(.+)\\s*?.+?\\s([a-zA-Z]+)\\s*(.*)\\s*?"};   
/*    QString patternData= {"\\*+?\\s*.+?\\s*?([0-9\\.]+)\\s*?.+?\\s([-0-9:]+\\s[0-9:,]+)\\s*?.+?(\\d+)\\]\\s*?.+?([a-zA-Z]+)\\s*.+?"
                          "\\s*(\\b[a-zA-Z].*-)\\s*?.+?\\s([a-zA-Z]+)\\s*(.*)\\r\\s+(?=\\*+?)"};*/
/*    QString patternData= {"\\*+?\\s*.+?\\s*?([0-9\\.]+)\\s*?.+?\\s([-0-9:]+\\s[0-9:,]+)\\s*?.+?(\\d+)\\]\\s*?.+?([a-zA-Z]+)\\s*.+?"
                              "\\s*(\\b[a-zA-Z].*-)\\s*?.+?\\s([a-zA-Z]+)\\s*(.*)\\r\\s+(?=\\*+?)"};*/


    QString patternData = {"\\s+?(?<Version>[0-9\\.]+).+?\\s+?(?<Time>[-0-9]+ [0-9:,]+).+?(?<ThreadID>[0-9]+).+?(?<LogLevel>[a-zA-Z]+)"
                           ".+?(?<ErrorClass>[a-zA-Z].+? -)\\s+?.+?\\s+?(?<ErrorContent>.+?)\\s+?(?=应用版本|\\s+?)"};

    int curPos = 0;
    QString tempStr = str;
    QRegularExpression reData(patternData);
//    qDebug() << str << endl;
    QRegularExpressionMatch match = reData.match(tempStr);
    while(match.hasMatch()){
        logList.append(*GetNewLog_message(match));   //添加新日志到日志列表中
//        qDebug() << match.captured() << "*************************" << endl;
        curPos += match.capturedLength(0);             //记录已匹配到的字符串的长度
        tempStr = str.mid(curPos - 1,str.length());   //获取未进行匹配的子串
        match = reData.match(tempStr);
    }
}

Log::Log()
{
    this->logData = nullptr;
    this->softwareVersion = nullptr;
    this->logLevel = nullptr;
    this->logContent = nullptr;
    this->threadID = nullptr;
    this->errorcategories = nullptr;
}

Log::Log(QString data, QString version, QString type, QString content,QString errorcategories,QString threadId)
{
    this->SetLogData(data);
    this->SetSoftwareVer(version);
    this->SetlogLevel(type);
    this->SetLogContent(content);
    this->SetErrorCategories(errorcategories);
    this->SetThreadId(threadId);
}

void Log::Output()
{
    qDebug() << QString("应用版本") << softwareVersion << endl;
    qDebug() << QString("记录时间") << logData << endl;
    qDebug() << QString("线程ID") << threadID << endl;
    qDebug() << QString("日志级别") << logLevel << endl;
    qDebug() << QString("出错类别") << errorcategories << endl;
    qDebug() <<QString("错误描述") << logContent << endl;
    qDebug() << "****************************************************************" << endl;
}

