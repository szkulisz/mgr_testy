#include "worker.h"
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <iostream>
#include <pthread.h>
#include "posixtimer.h"
#include "globals.h"
#include <time.h>

long long mBuf[2000];
int mBufIdx = 0;
bool mBufFlagDownReady = false;
bool mBufFlagUpReady = false;

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

Worker::Worker(int loop, bool notificate, int period, bool save,
                 int whichTimer, bool highPrio, bool load, QString name, QObject *parent)
    : QObject(parent),
      mNotificate(notificate),
      mHighPrio(highPrio),
      mLoopNumber(loop),
      mPeriod(period),
      mName(name)
{


    mQTimer = new QTimer();
    connect(mQTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(mQTimer, SIGNAL(timeout()), this, SLOT(onTimeoutLog()));
    mPosixTimer = new PosixTimer();
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(onTimeoutLog()));

    mProfiler.setPeriod(mPeriod);
    QString temp;
    if (load) {
        temp = "_with_";
    } else {
        temp = "_without_";
    }
    mProfiler.setFileName("logs/" + QString::number(mPeriod) + temp + mName + ".txt");
    mProfiler.setSave(save);
    mProfiler.startProfiling();

    if (!whichTimer) {
        mQTimer->start(mPeriod);
    } else {
        mPosixTimer->start(mPeriod);
    }

}

Worker::~Worker()
{
    delete mPosixTimer;
    delete mQTimer;
}

long long Worker::getDifferenceInNanoseconds()
{
    return mProfiler.getDifferenceInNanoseconds();
}

void Worker::onTimeout() {
    mProfiler.updateProfiling();
//        std::cout << std::setiosflags(std::ios::right) << std::resetiosflags(std::ios::left) << std::setw(10);
//        std::cout << mProfiler.getDifferenceInMicroseconds() << mName.toStdString() << std::endl;
//    mProfiler.logToFile();
//    mBuf[mBufIdx++] = mProfiler.getDifferenceInMicroseconds();
//    if (mBufIdx >= 2000) {
//        mBufIdx = 0;
//        mBufFlagUpReady = true;
//    }
//    if (mBufIdx == 1000) {
//        mBufFlagDownReady = true;
//    }
    ++mCounter;
    if (gWhichBufferIsActive.load() == 0) {
        gDiffBuffer1[mCounter] = mProfiler.getDifferenceInNanoseconds();
        if (gDiffBuffer1.size() >= 1000) {
            if (gWhichBufferIsActive.testAndSetAcquire(0,1) == false) {
                std::cout << "Blad zmiany bufora!" << std::endl;
                QCoreApplication::quit();
            }
            if (gBufferFull.testAndSetAcquire(0,1) == false) {
                std::cout << "Bufor ciagle pelny!" << std::endl;
                QCoreApplication::quit();
            }
        }
    } else {
        gDiffBuffer2[mCounter] = mProfiler.getDifferenceInNanoseconds();
        if (gDiffBuffer2.size() >= 1000) {
            if (gWhichBufferIsActive.testAndSetAcquire(1,0) == false) {
                std::cout << "Blad zmiany bufora!" << std::endl;
                QCoreApplication::quit();
            }
            if (gBufferFull.testAndSetAcquire(0,1) == false) {
                std::cout << "Bufor ciagle pelny!" << std::endl;
                QCoreApplication::quit();
            }
        }
    }
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
            ((mPeriod>=1000) ? true : (mCounter%(1000/mPeriod) == 0)) ){
        std::cout << mCounter << ' ' << mName.toStdString() << std::endl;
    }
    if (mCounter >= mLoopNumber) {
        int sts;
        struct sched_param param;
        sts = sched_getparam(0, &param);
        CHECK(sts,"sched_getparam");
        std::cout << "Koniec watku " << mName.toStdString() << " o priorytecie " << param.sched_priority << std::endl;
        std::cout << "overrunCounter: " << mOverrunCounter <<std::endl;
        std::cout << "maxOverrun: " << mMaxOverrun << std::endl;
        std::cout << "mMaxNano: " << mMaxNano << std::endl;
        std::cout << "maxCounter: " << mMaxCounter << std::endl;
        mQTimer->stop();
        mPosixTimer->stop();
        gContinueLogging.store(0);
        emit done();
    }
}

void Worker::onTimeoutLog()
{
//    emit timeout(mProfiler.getDifferenceInNanoseconds());
//    emit timeout(5);
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
    std::cout << param.sched_priority<< " elo z worker" << QThread::currentThreadId() << "prio " << param.sched_priority << std::endl;
}
