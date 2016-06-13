#ifndef SUPER_H
#define SUPER_H

#include <QObject>
#include "worker.h"
#include <QTimer>
#include "posixtimer.h"

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(int loopNumber = 200, int period = 100, int timer = 0, bool highPrio = false,
                     bool save = false, bool load = false, bool rtKernel = false, QObject *parent = 0);
    ~Program();

signals:
    void done();

public slots:
    void finish();
    void onTimeout();

private:
    pid_t mChildPid;
    QString mName;
    Profiler mProfiler;
    PosixTimer *mPosixTimer;
    QTimer *mQTimer;
    bool mLoad, mSave, mNotificate, mHighPrio;
    int mLoopNumber = 0;
    int mCounter = 0;
    int mPeriod = 0;
    int mOverrunCounter = 0;
    int mMaxOverrun  = 0;
    int mTest = 0;
    int mMaxCounter = 0;
    long long mMaxNano = 0;
};

#endif // SUPER_H
