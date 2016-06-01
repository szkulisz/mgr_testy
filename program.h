#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include "profiler.h"

class PosixTimer;
class QTimer;
//class TimingThread;
class QMutex;

class Program : public QObject
{
    Q_OBJECT
public:
//    explicit Program(QObject *parent = 0);
    explicit Program(int loopNumber = 200, bool notificate = false, int period = 100,
                     bool save = false, int whichTimer = 0, bool highPrio = false,
                     QString name = 0, QObject *parent = 0);
    ~Program();

signals:
    void done();

public slots:
    void update();
    void atThreadStart();

private:
    Profiler mProfiler;
    int mLoopNumber;
    bool mNotificate;
    PosixTimer *mPosixTimer;
    QTimer *mQTimer;
//    TimingThread *mTimingThread = NULL;
    QString mName;
    int mCounter = 0;
    int mPeriod;
    bool mHighPrio;
    static QMutex mMutex;

};

#endif // PROGRAM_H
