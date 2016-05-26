#include "program.h"
#include <QCoreApplication>
#include <QTimer>
#include <time.h>
#include <QFile>
#include <iostream>
#include <iomanip>
#include "profiler.h"
#include "posixtimer.h"
#include "timingthread.h"

Program::Program(int loop, int notification, int period, bool save,
                 int whichTimer, int whichThread, QObject *parent)
    : QObject(parent),
      mLoopNumber(20),
      mNotificationNumber(notification)
{
    mQTimer = new QTimer(this);
    connect(mQTimer, SIGNAL(timeout()), this, SLOT(update()));
    mPosixTimer = new PosixTimer();
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(update()));

    whichThread = 2;

    if( (whichThread == 0) ||(whichThread == 2)) {
        mProfiler.setPeriod(period);
        mProfiler.insertToFileName("prog_");
        mProfiler.setSave(save);
        mProfiler.startProfiling();
        if (!whichTimer) {
            mQTimer->start(period);
        } else {
            mPosixTimer->start(period);
        }
    }
    if( (whichThread == 1) ||(whichThread == 2)) {
        mTimingThread = new TimingThread(loop, notification, period,
                                         save, whichTimer, this);
//        mTimingThread->start(QThread::TimeCriticalPriority);
    }



}

Program::~Program()
{
    delete mPosixTimer;
    delete mQTimer;
    delete mTimingThread;
}

void Program::update() {
    static int counter;

    mProfiler.updateProfiling();
        std::cout << std::setiosflags(std::ios::right) << std::resetiosflags(std::ios::left) << std::setw(10);
        std::cout << mProfiler.getDifferenceInMicroseconds() << " p" << std::endl;
    mProfiler.logToFile();



    if (counter%mNotificationNumber == 0)
//        std::cout << '\r' << counter << std::flush;
    if (counter >= mLoopNumber) {
        std::cout << std::endl;
        if (mTimingThread) {
            mTimingThread->quit();
            mTimingThread->wait();
        }
//            while( !mTimingThread->isFinished());
        QCoreApplication::quit();
    }
    ++counter;


}

