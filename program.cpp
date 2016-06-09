#include "program.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QThread>
#include <pthread.h>
#include "worker.h"
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <sstream>

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

Program::Program(int loop, int period, int timer, bool highPrio, bool save, bool load, bool rtKernel, QObject *parent)
    : QObject(parent),
      mLoad(load),
      mSave(save)
{
//    mSave = 1;
//    loop = 250;
//    period = 1;
    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = 50;//(sched_get_priority_max(SCHED_FIFO));
    sts = sched_setscheduler(0, SCHED_FIFO, &param);
    CHECK(sts,"sched_setscheduler");
    std::cout << "proces ma ID: " << QThread::currentThreadId() << " i priorytet: " << param.sched_priority << std::endl;

    if (mLoad) {
        mChildPid = fork();
        if (mChildPid == 0) {
            char *argv[] = {(char*)"/usr/bin/stress",
                            (char*)"--cpu", (char*)"1",(char*)"--io", (char*)"1",
                            (char*)"--vm", (char*)"1",(char*)"--vm-bytes", (char*)"128M",
                            (char*)"--hdd", (char*)"1",(char*)"--hdd-bytes", (char*)"128M",
                            0};
            execv("/usr/bin/stress",argv);
        }
    }

    QString name = QString::number(period);
    if (rtKernel) {
        name += "_rt_";
    } else {
        name += "_norm_";
    }
    if (load) {
        name += "with_";
    } else {
        name += "without_";
    }
    if (highPrio) {
        name += "hi_";
    } else {
        name += "low_";
    }
    switch (timer) {
    case 0:
        name += "qt";
        break;
    case 1:
        name += "pos";
    default:
        break;
    }

    mWorker = new Worker(loop,true,period,save,timer,highPrio,name);
    connect(mWorker,&Worker::done,this,&Program::finish);
    mWorker->moveToThread(&mTimerThread);
    mTimerThread.setObjectName("TimerThread");
    connect(&mTimerThread,&QThread::started,mWorker,&Worker::atThreadStart);
    connect(&mTimerThread,&QThread::finished,mWorker,&QObject::deleteLater);
    mTimerThread.start();
}

Program::~Program()
{
    if (mLoad) {
        if (system("pkill -f stress") ==0 )
            perror("pkill");
    }
}

void Program::finish()
{
    mTimerThread.quit();
    mTimerThread.wait();
    QCoreApplication::quit();
}

