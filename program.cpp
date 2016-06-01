#include "program.h"
#include <QCoreApplication>
#include <QTimer>
#include <time.h>
#include <QFile>
#include <iostream>
#include <iomanip>
#include "profiler.h"
#include "posixtimer.h"
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <pthread.h>

QMutex Program::mMutex;

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

Program::Program(int loop, bool notification, int period, bool save,
                 int whichTimer, bool highPrio, QString name, QObject *parent)
    : QObject(parent),
      mLoopNumber(loop),
      mNotificate(notification),
      mName(name),
      mPeriod(period),
      mHighPrio(highPrio)
{

    mQTimer = new QTimer(this);
    connect(mQTimer, SIGNAL(timeout()), this, SLOT(update()));
    mPosixTimer = new PosixTimer(notification);
    connect(mPosixTimer, SIGNAL(timeout()), this, SLOT(update()));

    mProfiler.setPeriod(period);
    mProfiler.insertToFileName(name);
    mProfiler.setSave(save);
    mProfiler.startProfiling();

    if (!whichTimer) {
        mQTimer->start(mPeriod);
    } else {
        mPosixTimer->start(mPeriod);
    }

}

Program::~Program()
{
    delete mPosixTimer;
    delete mQTimer;
}

void Program::update() {
//    mProfiler.write(QString("początek"));
    mProfiler.updateProfiling();
//        std::cout << std::setiosflags(std::ios::right) << std::resetiosflags(std::ios::left) << std::setw(10);
//        std::cout << mProfiler.getDifferenceInMicroseconds() << mName.toStdString() << std::endl;
//    mProfiler.write(QString("update"));
    mProfiler.logToFile();
//    mProfiler.write(QString("zalogowano"));


    ++mCounter;

    if (mNotificate &&
            (mPeriod>=1000) ? true : (mCounter%(1000/mPeriod) == 0)){
        std::cout << '\r' << mCounter << ' ' << mName.toStdString() << std::flush ;
    }
    if (mCounter >= mLoopNumber) {
        std::cout << "Koniec wątku " <<  mName.toStdString() << std::endl;
        int sts;
        struct sched_param param;
        sts = sched_getparam(0, &param);
        CHECK(sts,"sched_getparam");
        std::cout << "Koniec wątku " << mName.toStdString() << " o priorytecie " << param.sched_priority << std::endl;

        mQTimer->stop();
        mPosixTimer->stop();
        emit finito();
    }
//    mProfiler.write(QString("koniec"));
}

void Program::atThreadStart()
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

