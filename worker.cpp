#include "worker.h"
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <iostream>
#include <pthread.h>
#include "posixtimer.h"

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
    connect(mQTimer, SIGNAL(timeout()), this, SLOT(update()));
    mPosixTimer = new PosixTimer();
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(update()));

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
}

void Worker::update() {
//    mProfiler.write(QString("początek"));
    mProfiler.updateProfiling();
//        std::cout << std::setiosflags(std::ios::right) << std::resetiosflags(std::ios::left) << std::setw(10);
//        std::cout << mProfiler.getDifferenceInMicroseconds() << mName.toStdString() << std::endl;
//    mProfiler.write(QString("update"));
    mProfiler.logToFile();
//    mProfiler.write(QString("zalogowano"));


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
        std::cout << "Koniec wątku " << mName.toStdString() << " o priorytecie " << param.sched_priority << std::endl;

        mQTimer->stop();
        mPosixTimer->stop();
        emit done();
    }
//    mProfiler.write(QString("koniec"));
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

