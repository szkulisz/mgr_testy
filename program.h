#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include "profiler.h"

class PosixTimer;
class QTimer;
class TimingThread;

class Program : public QObject
{
    Q_OBJECT
public:
//    explicit Program(QObject *parent = 0);
    explicit Program(int loopNumber = 200, int notification = 20, int period = 100,
                     bool save = false, int whichTimer = 0,
                     int whichThread = 0, QObject *parent = 0);
    ~Program();

signals:

public slots:
    void update();

private:
    Profiler mProfiler;
    int mLoopNumber;
    int mNotificationNumber;
    PosixTimer *mPosixTimer;
    QTimer *mQTimer;
    TimingThread *mTimingThread = NULL;
};

#endif // PROGRAM_H
