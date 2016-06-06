#include "program.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QThread>
#include <pthread.h>
#include "worker.h"
#include <unistd.h>
#include <signal.h>
#include <iostream>

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

Program::Program(int loop, int period, int timer, bool highPrio, bool save, bool load, QObject *parent)
    : QObject(parent),
      mLoad(load),
      mSave(save)
{
//    save = 1;
//    loop = 250;
//    period = 1;
    if (mLoad) {
        mChildPid = fork();
        if (mChildPid == 0) {
            char *argv[] = {(char*)"/usr/bin/stress", (char*)"--cpu", (char*)"1",
                           (char*)"--io", (char*)"1", (char*)"--vm", (char*)"1",
                           (char*)"--vm-bytes", (char*)"128M", 0};
            execv("/usr/bin/stress",argv);
        }
    }

    QString name;
    if (load) {
        name = "_with_";
    } else {
        name = "_without_";
    }
    if (highPrio) {
        name += "hi_";
    } else {
        name += "norm_";
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


//    int sts;
//    struct sched_param param;
//    sts = sched_getparam(0, &param);
//    CHECK(sts,"sched_getparam");
//    param.sched_priority = (sched_get_priority_max(SCHED_FIFO) - sched_get_priority_min(SCHED_FIFO)) / 2;
//    sts = sched_setscheduler(0, SCHED_FIFO, &param);
//    CHECK(sts,"sched_setscheduler");

    p1 = new Worker(loop,true,period,0,timer,highPrio,load,name);
    connect(p1,&Worker::done,this,&Program::finish);
    p1->moveToThread(&t1);
    t1.setObjectName(name);
    connect(&t1,&QThread::started,p1,&Worker::atThreadStart);
    connect(&t1,&QThread::finished,p1,&QObject::deleteLater);

    if (mSave) {
        mFileName = "logs/" + QString::number(period) + name + ".txt";
        mLogFile.setFileName(mFileName);
        mLogFile.open(QFile::WriteOnly | QFile::Text);
        mLogStream = new QTextStream(&mLogFile);
        connect(p1,&Worker::timeout,this,&Program::onTimeoutLog);
    }

    t1.start();
}

Program::~Program()
{
    delete mLogStream;
}

void Program::finish()
{
    t1.quit();
    t1.wait();
    if (mLoad) {
        if (system("pkill -f stress") ==0 )
            perror("pkill");
    }
    QCoreApplication::quit();
}

void Program::onTimeoutLog(std::deque<long long> difference)
{
    if (mSave) {
        for (auto &diff :difference){
            *mLogStream << diff << "\n";
        }
//        *mLogStream << difference << "\n";
    }
}

