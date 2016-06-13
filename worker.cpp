#include "worker.h"
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <iostream>
#include <pthread.h>
#include "posixtimer.h"
#include <time.h>

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

Worker::Worker(int loop, bool notificate, int period, bool save,
                 int whichTimer, bool highPrio, QString name, QObject *parent)
    : QObject(parent),
      mNotificate(notificate),
      mHighPrio(highPrio),
      mLoopNumber(loop),
      mPeriod(period),
      mName(name)
{

    mQTimer = new QTimer();
    connect(mQTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    mPosixTimer = new PosixTimer();
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    mProfiler.setPeriod(mPeriod);
    mProfiler.startLogging(loop, save, "logs/" + mName + ".txt");
    mProfiler.startProfiling();

    if (!whichTimer) {
        mQTimer->start(mPeriod/1000);
    } else {
        mPosixTimer->start(mPeriod);
    }

}

Worker::~Worker()
{
    delete mPosixTimer;
    delete mQTimer;
}

void Worker::onTimeout() {
    mProfiler.updateProfiling();
    mProfiler.logToFile();

    ++mCounter;
    mTest = timer_getoverrun(mPosixTimer->mTimerID);
    if (mTest != 0 && mCounter!=1) {
        mOverrunCounter++;
        if (mTest>mMaxOverrun){
            mMaxOverrun = mTest;
            mMaxNano = mProfiler.getDifferenceInNanoseconds();
            mMaxCounter = mCounter;
        }
    }

    if (mNotificate &&
            ((mPeriod>=1000000) ? true : (mCounter%(1000000/mPeriod) == 0)) ){
        std::cout << "\r" << mCounter << ' ' << mName.toStdString() << std::flush;
    }
    if (mCounter >= mLoopNumber) {
        std::cout << "overrunCounter: " << mOverrunCounter <<std::endl;
        std::cout << "maxOverrun: " << mMaxOverrun << std::endl;
        std::cout << "mMaxNano: " << mMaxNano << std::endl;
        std::cout << "maxCounter: " << mMaxCounter << std::endl;
        mQTimer->stop();
        mPosixTimer->stop();
        emit done();
    }
}


void Worker::atThreadStart()
{
    int high_priority, sts, policy;
    struct sched_param param;
    sts = pthread_getschedparam(pthread_self(), &policy, &param);
    CHECK(sts,"pthread_getschedparam");
    high_priority = sched_get_priority_max(SCHED_FIFO);
    if (mHighPrio){
        param.sched_priority = high_priority;
    } else {
        param.sched_priority = high_priority/2;
    }
    sts = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    CHECK(sts,"pthread_setschedparam");
    std::cout << "TimerThread ma ID: " << QThread::currentThreadId() << " i priorytet: " << param.sched_priority << std::endl;
}
