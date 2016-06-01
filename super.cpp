#include "super.h"
#include <QCoreApplication>

#include "program.h"

Super::Super(int loop, int notification, int period, bool save, QObject *parent)
    : QObject(parent)
{
//    save = 1;
//    loop = 250000;
//    period = 1;
//    notification = 1;
    p1 = new Program(loop,1,period,save,0,"qt_norm_",0);
    p2 = new Program(loop,2,period,save,1,"pos_norm_",0);
    p3 = new Program(loop,3,period,save,0,"qt_hi_",0);
    p4 = new Program(loop,4,period,save,1,"pos_hi_",0);
    connect(p1,SIGNAL(finito()),this,SLOT(finish()));
    connect(p2,SIGNAL(finito()),this,SLOT(finish()));
    connect(p3,SIGNAL(finito()),this,SLOT(finish()));
    connect(p4,SIGNAL(finito()),this,SLOT(finish()));

    p1->moveToThread(&t1);
    p2->moveToThread(&t2);
    p3->moveToThread(&t3);
    p4->moveToThread(&t4);
    t1.setObjectName("QT_norm");
    t2.setObjectName("pos_norm");
    t3.setObjectName("QT_hi");
    t4.setObjectName("pos_hi");
    t1.start();
    t2.start();
//    t3.start();
//    t4.start();
    t3.start(QThread::TimeCriticalPriority);
    t4.start(QThread::TimeCriticalPriority);
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

