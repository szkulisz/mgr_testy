#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include "profiler.h"

class PosixTimer;
class QTimer;
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(int loopNumber = 200, bool notificate = false, int period = 100,
                     bool save = false, int whichTimer = 0, bool highPrio = false, QString name = 0, QObject *parent = 0);
    ~Worker();
    long long getDifferenceInNanoseconds();


signals:
    void done();
    void timeout(long long);

public slots:
    void onTimeout();
    void atThreadStart();

private:
    bool mNotificate;
    bool mHighPrio;
    int mLoopNumber;
    int mCounter;
    int mPeriod;
    int mWhereToWrite;
    QString mName;
    Profiler mProfiler;
    PosixTimer *mPosixTimer;
    QTimer *mQTimer;
    int mOverrunCounter = 0;
    int mMaxOverrun  = 0;
    int mTest = 0;
    int mMaxCounter = 0;
    long long mMaxNano = 0;




};

#endif // PROGRAM_H
