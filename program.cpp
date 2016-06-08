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
            char *argv[] = {(char*)"/usr/bin/stress",
                            (char*)"--cpu", (char*)"1",(char*)"--io", (char*)"1",
                            (char*)"--vm", (char*)"1",(char*)"--vm-bytes", (char*)"128M",
                            (char*)"--hdd", (char*)"1",(char*)"--hdd-bytes", (char*)"128M",
                            0};
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


    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = 99;//(sched_get_priority_max(SCHED_FIFO));
    sts = sched_setscheduler(0, SCHED_FIFO, &param);
    CHECK(sts,"sched_setscheduler");
    std::cout << param.sched_priority<< " elo z program" << QThread::currentThreadId() << "prio " << param.sched_priority << std::endl;

    p1 = new Worker(loop,true,period,false,timer,highPrio,load,name);
    connect(p1,&Worker::done,this,&Program::finish);
    p1->moveToThread(&t1);
    t1.setObjectName(name);
    connect(&t1,&QThread::started,p1,&Worker::atThreadStart);
    connect(&t1,&QThread::finished,p1,&QObject::deleteLater);

//    if (mSave) {
//        mFileName = "logs/" + QString::number(period) + name + ".txt";
//        mLogFile.setFileName(mFileName);
//        mLogFile.open(QFile::WriteOnly | QFile::Text);
//        mLogStream = new QTextStream(&mLogFile);
//        mLogStream = new QTextStream(&mArray);
//        mBuffer.open(QBuffer::ReadWrite);
//        connect(p1,&Worker::timeout,this,&Program::onTimeoutLog);
//        connect(&mTimer,SIGNAL(timeout()),this,SLOT(onTimeoutLog2()));
//        mTimer.start(250);

//    }
//    connect(&mLogger,&Logger::finished, )
    mLogger.start();
    t1.start();
}

Program::~Program()
{
//    std::cout << mBuffer.size() << std::endl;
//    mLogFile.write(mBuffer.buffer());
    mLogFile.close();
    delete mLogStream;
    if (mLoad) {
        if (system("pkill -f stress") ==0 )
            perror("pkill");
    }
}

void Program::finish()
{
    t1.quit();
    t1.wait();
    mLogger.quit();
    mLogger.wait();

    QCoreApplication::quit();
}

void Program::onTimeoutLog(long long difference)
{
    if (mSave) {
//        *mLogStream << difference << "\n";
//        mStream << difference;
//        mBuffer.write(mStream.str().c_str());

    }
}

void Program::onTimeoutLog2()
{
//    if (mSave) {
//        if (mBufFlagUpReady) {
//            for (int i=0;i<1000;++i)
//                *mLogStream <</* mBuf[i] << */"\n";
//        }
//        if (mBufFlagDownReady) {
//            for (int i=0;i<1000;++i)
//                *mLogStream << mBuf[i+1000] << "\n";
//        }
//    }

}

