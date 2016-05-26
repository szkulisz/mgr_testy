#include "timingthread.h"
#include <iostream>
#include <iomanip>
#include <QCoreApplication>
#include <QTimer>
#include "posixtimer.h"

TimingThread::TimingThread(int loop, int notification, int period, bool save, int whichTimer, QObject *parent)
    : QThread(parent),
      mLoopNumber(10),
      mNotificationNumber(notification),
      mPeriod(period),
      mWhichTimer(whichTimer)
{

    mQTimer = new QTimer(this);
    connect(mQTimer, SIGNAL(timeout()), this, SLOT(update()));
    mPosixTimer = new PosixTimer();
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(update()));
    mProfiler.setPeriod(mPeriod);
    mProfiler.insertToFileName("thr_");
    mProfiler.setSave(save);

    mProfiler.startProfiling();
    if (!mWhichTimer) {
        this->mQTimer->start(mPeriod);
    } else {
        this->mPosixTimer->start(mPeriod);
    }

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));


}

TimingThread::~TimingThread()
{
    delete mPosixTimer;
    delete mQTimer;
}

void TimingThread::run()
{
    this->exec();
}



void TimingThread::update()
{
    static int counter;

    mProfiler.updateProfiling();
        std::cout << std::setiosflags(std::ios::right) << std::resetiosflags(std::ios::left) << std::setw(10);
        std::cout << mProfiler.getDifferenceInMicroseconds() << " t " << std::endl;
    mProfiler.logToFile();



    if (counter%mNotificationNumber == 0)
//        std::cout << '\r' << counter << std::flush;
    if (counter >= mLoopNumber) {
        std::cout << std::endl;
//        this->exit();
    }
    ++counter;


}
