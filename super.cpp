#include "super.h"
#include <QCoreApplication>
#include <pthread.h>

#include "program.h"
#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

Super::Super(int loop, int period, bool save, QObject *parent)
    : QObject(parent)
{
//    save = 1;
//    loop = 250;
//    period = 1;
    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = (sched_get_priority_max(SCHED_FIFO) - sched_get_priority_min(SCHED_FIFO)) / 2;
    sts = sched_setscheduler(0, SCHED_FIFO, &param);
    CHECK(sts,"sched_setscheduler");

    p1 = new Program(loop,true, period,save,0,false,"qt_norm_");
    p2 = new Program(loop,false,period,save,1,false,"pos_norm_");
    p3 = new Program(loop,false,period,save,0,true ,"qt_hi_");
    p4 = new Program(loop,false,period,save,1,true ,"pos_hi_");
    connect(p1,&Program::done,this,&Super::finish);
    connect(p2,&Program::done,this,&Super::finish);
    connect(p3,&Program::done,this,&Super::finish);
    connect(p4,&Program::done,this,&Super::finish);
    p1->moveToThread(&t1);
    p2->moveToThread(&t2);
    p3->moveToThread(&t3);
    p4->moveToThread(&t4);
    t1.setObjectName("QT_norm");
    t2.setObjectName("pos_norm");
    t3.setObjectName("QT_hi");
    t4.setObjectName("pos_hi");
    connect(&t1,&QThread::started,p1,&Program::atThreadStart);
    connect(&t2,&QThread::started,p2,&Program::atThreadStart);
    connect(&t3,&QThread::started,p3,&Program::atThreadStart);
    connect(&t4,&QThread::started,p4,&Program::atThreadStart);
    connect(&t1,&QThread::finished,p1,&QObject::deleteLater);
    connect(&t2,&QThread::finished,p2,&QObject::deleteLater);
    connect(&t3,&QThread::finished,p3,&QObject::deleteLater);
    connect(&t4,&QThread::finished,p4,&QObject::deleteLater);
    t1.start();
    t2.start();
    t3.start();
    t4.start();
}

Super::~Super()
{
    delete p1;
    delete p2;
    delete p3;
    delete p4;
}

void Super::finish()
{
    static int counter = 4;

    --counter;
    if (!counter) {
        t1.quit();
        t1.wait();
        t2.quit();
        t2.wait();
        t3.quit();
        t3.wait();
        t4.quit();
        t4.wait();
        QCoreApplication::quit();
    }

}

