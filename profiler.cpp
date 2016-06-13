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
        delete mLogStream;
        delete mLogTable;
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

void Profiler::saveLogFile()
{
    if (mSave) {
        mLogFile.setFileName(fileName);
        mLogFile.open(QFile::WriteOnly | QFile::Text);
        mLogStream = new QTextStream(&mLogFile);
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
        mLogTable = new long long[loops];
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


