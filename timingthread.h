#ifndef TIMINGTHREAD_H
#define TIMINGTHREAD_H

#include <QThread>
#include "profiler.h"

class PosixTimer;
class QTimer;

class TimingThread : public QThread
{
    Q_OBJECT
public:
    TimingThread(int loopNumber = 200, int notification = 20, int period = 100,
                 bool save = false, int whichTimer = 0, QObject *parent = 0);
    ~TimingThread();

    void run() Q_DECL_OVERRIDE;

public slots:
    void update();

private:
    Profiler mProfiler;
    int mLoopNumber;
    int mNotificationNumber;
    int mPeriod;
    int mWhichTimer;
    PosixTimer *mPosixTimer;
    QTimer *mQTimer;
};

#endif // TIMINGTHREAD_H
