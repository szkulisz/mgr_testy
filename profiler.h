#ifndef PROFILER_H
#define PROFILER_H

#include <time.h>
#include <QFile>

#include <QString>
#include <QObject>
#include <QThread>

class QTextStream;

class LoggerHelper : public QObject
{
    Q_OBJECT
public:
    explicit LoggerHelper(QObject *parent = 0);
    explicit LoggerHelper(int, QString, QObject *parent = 0);
    ~LoggerHelper();

public slots:
    void log(long long difference);
    void atThreadStart();



private:
    QFile mLogFile;
    QString mFileName;
    QTextStream *mLogStream = nullptr;
    unsigned int mLoops;
    long long *mLogTable;
    unsigned int mLogTableIdx=0;


};

class Profiler : public QObject
{
    Q_OBJECT
public:
    explicit Profiler(QObject *parent = 0);
    explicit Profiler(int loops,int period, bool save, QObject *parent = 0);
    ~Profiler();

    void startProfiling();
    void updateProfiling();
    int getDifferenceInSeconds();
    int getDifferenceInMiliseconds();
    int getDifferenceInMicroseconds();
    long long getDifferenceInNanoseconds();
    void logToFile();

    void setPeriod(int period);
    void startLogging(int loops, bool save, const QString &fileName);

signals:
    void log(long long);

private:
    bool mSave;
    int mPeriod;
    timespec mTimePrevious, mTimeActual, mTimerDifference;
    QThread mLoggerThread;
    LoggerHelper *mLogger = nullptr;
    int mLoops;

    timespec countDifference(timespec start, timespec stop);

};





#endif // PROFILER_H
