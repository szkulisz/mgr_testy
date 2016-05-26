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
#include <QDebug>

Program::Program(int loop, int notification, int period, bool save,
                 int whichTimer, QString name, QObject *parent)
    : QObject(parent),
      mLoopNumber(loop),
      mNotificationNumber(notification),
      mName(name)
{


    mQTimer = new QTimer(this);
    connect(mQTimer, SIGNAL(timeout()), this, SLOT(update()));
    mPosixTimer = new PosixTimer();
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(update()));



//
        mProfiler.setPeriod(period);
        mProfiler.insertToFileName(name);
        mProfiler.setSave(save);
        mProfiler.startProfiling();
        if (!whichTimer) {
            mQTimer->start(period);
        } else {
            mPosixTimer->start(period);
        }
//    }
//    if( (whichThread == 1) ||(whichThread == 2)) {
//        mTimingThread = new TimingThread(loop, notification, period,
//                                         save, whichTimer, this);
////        mTimingThread->start(QThread::TimeCriticalPriority);
//    }



}

Program::~Program()
{
    delete mPosixTimer;
    delete mQTimer;
    delete mTimingThread;
}

void Program::update() {

    mProfiler.updateProfiling();
//        std::cout << std::setiosflags(std::ios::right) << std::resetiosflags(std::ios::left) << std::setw(10);
        std::cout << mProfiler.getDifferenceInMicroseconds() << mName.toStdString() << std::endl;
    mProfiler.logToFile();

//    std::cout <<"From program update: "<<QThread::currentThreadId();

    ++mCounter;

    if (mCounter%mNotificationNumber == 0){
//        std::cout << '\r' << counter << std::flush;
    }
    if (mCounter >= mLoopNumber) {
        std::cout << std::endl;
        mQTimer->stop();
        mPosixTimer->stop();
        emit finito();

//        if (mTimingThread && mTimingThread->isRunning()) {
//            mTimingThread->quit();
//            mTimingThread->wait();
//        }
//        QCoreApplication::quit();
    }



}

