#include "profiler.h"
#include <cmath>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <iostream>

Profiler::Profiler()
    : Profiler(1000, false)
{

}

Profiler::Profiler(int period, bool save) :
    mSave(save),
    mPeriod(period)
{
    setFileName("logs/" + QDateTime::currentDateTime().toString("dd_MM_yy__hh_mm_ss") + ".txt");
}

Profiler::~Profiler()
{
    if (mSave) {
        delete mLogStream;
        std::cout << mByteArray.size() << std::endl;
        mLogFile.write(mByteArray);
        if (mLogFile.isOpen()) {
            mLogFile.close();
        }
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

qlonglong Profiler::getDifferenceInNanoseconds()
{
    qlonglong periodInNs = mPeriod*1000000;
    return (1000000000*mTimerDifference.tv_sec + mTimerDifference.tv_nsec) - periodInNs;
}

void Profiler::logToFile()
{
    if (mSave) {
//        *mLogStream << mTimeActual.tv_sec << '.';
//        mLogFile.write("\n");
//        mLogStream->setFieldWidth(9);
//        mLogStream->setPadChar('0');
//        *mLogStream << mTimeActual.tv_nsec;
//        mLogStream->setFieldWidth(0);
        *mLogStream << getDifferenceInNanoseconds() << "\n";
//        mLogStream->flush();
    }
}

void Profiler::setPeriod(int period)
{
    mPeriod = period;
}

void Profiler::setSave(bool save)
{
    mSave = save;
    if (mSave) {
        mLogFile.setFileName(mFileName);
        mLogFile.open(QFile::WriteOnly | QFile::Text);

        mLogStream = new QTextStream(&mByteArray);
    }
}


void Profiler::setFileName(const QString &fileName)
{
    mFileName = fileName;
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
