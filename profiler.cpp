#include "profiler.h"
#include <cmath>
#include <QDateTime>
#include <QTextStream>
#include <QThread>
#include <QFile>
#include <iostream>

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }


Profiler::Profiler(QObject *parent)
    : Profiler(1000, false, parent)
{
}

Profiler::Profiler(int loops, int period, bool save, QObject *parent)
    : QObject(parent),
      mSave(save),
      mPeriod(period),
      mLoops(loops)
{
}

Profiler::~Profiler()
{
    if (mSave) {
        delete mLogger;
        mLoggerThread.quit();
        mLoggerThread.wait();
    }

}

void Profiler::startProfiling()
{
    clock_gettime(CLOCK_MONOTONIC, &mTimePrevious);
}

void Profiler::updateProfiling()
{
    clock_gettime(CLOCK_MONOTONIC, &mTimeActual);
    mTimerDifference = countDifference( mTimePrevious, mTimeActual );
    mTimePrevious = mTimeActual;
}

int Profiler::getDifferenceInSeconds()
{
    return std::round(getDifferenceInNanoseconds() / 1000000000.0);
}

int Profiler::getDifferenceInMiliseconds()
{
    return std::round(getDifferenceInNanoseconds() / 1000000.0);
}

int Profiler::getDifferenceInMicroseconds()
{
//    return std::round(getDifferenceInNanoseconds() / 1000.0);
    return getDifferenceInNanoseconds() / 1000;
}

long long Profiler::getDifferenceInNanoseconds()
{
    long long periodInNs = mPeriod*1000;
    return (1000000000*mTimerDifference.tv_sec + mTimerDifference.tv_nsec) - periodInNs;
}

void Profiler::logToFile()
{
    if (mSave) {
        emit log(getDifferenceInNanoseconds());
    }
}

void Profiler::setPeriod(int period)
{
    mPeriod = period;
}

void Profiler::startLogging(int loops, bool save, const QString &fileName)
{
    mSave = save;
    if (mSave) {
        mLogger = new LoggerHelper(loops, fileName);
        mLogger->moveToThread(&mLoggerThread);
        connect(&mLoggerThread,&QThread::started,mLogger,&LoggerHelper::atThreadStart);
        connect(this,&Profiler::log,mLogger,&LoggerHelper::log);
        mLoggerThread.setObjectName("LoggerThread");
        mLoggerThread.start();
    }
}



timespec Profiler::countDifference(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}



LoggerHelper::LoggerHelper(QObject *parent)
    : QObject(parent)
{
}

LoggerHelper::LoggerHelper(int loops, QString fileName, QObject *parent) :
    QObject(parent),
    mFileName(fileName),
    mLoops(loops)
{
    mLogTable = new long long[loops];
    mLogFile.setFileName(mFileName);
    mLogFile.open(QFile::WriteOnly | QFile::Text);
    mLogStream = new QTextStream(&mLogFile);
}

LoggerHelper::~LoggerHelper()
{
//    for (auto val : mLogTable)
    std::cout << "\nzapis do pliku" << std::endl;
    for(unsigned int i=1; i<mLoops-1; ++i)
        *mLogStream << mLogTable[i] << "\n";
    delete mLogStream;
    if (mLogFile.isOpen())
        mLogFile.close();
    std::cout << "koniec zapisu" << std::endl;
}

void LoggerHelper::log(long long difference)
{
//    *mLogStream << difference << "\n";
    if (mLogTableIdx <= mLoops) {
        mLogTable[mLogTableIdx++] = difference;
    } else {
        std::cout << "Przekroczyles rozmiar tablicy..." << std::endl;
    }
}

void LoggerHelper::atThreadStart()
{
    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = sched_get_priority_min(SCHED_OTHER);
    sts = sched_setscheduler(0, SCHED_OTHER, &param);
    CHECK(sts,"sched_setscheduler");
    std::cout << "LoggerThread ma ID: " << QThread::currentThreadId() << " i priorytet: " << param.sched_priority << std::endl;
}
