#include "worker.h"
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <iostream>
#include <pthread.h>
#include "posixtimer.h"


#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

QMutex Worker::mMutex;

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
//    connect(mQTimer, SIGNAL(timeout()), this, SLOT(onTimeoutLog()));
    mPosixTimer = new PosixTimer();
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
//    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(onTimeoutLog()));
    mQTimerLog = new QTimer();
    connect(mQTimerLog, SIGNAL(timeout()), this, SLOT(onTimeoutLog()));

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
    mQTimerLog->start(mPeriod*100);

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
    mMutex.lock();
    mDifferences.push_front(mProfiler.getDifferenceInNanoseconds());
    mMutex.unlock();

    ++mCounter;
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

        mQTimer->stop();
        mPosixTimer->stop();
        onTimeoutLog();
        emit done();
    }
}

void Worker::onTimeoutLog()
{
    mMutex.lock();
    std::deque<long long> copy = mDifferences;
    mDifferences.clear();
    mMutex.unlock();
//    emit timeout(mProfiler.getDifferenceInNanoseconds());
    emit timeout(copy);
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
}
