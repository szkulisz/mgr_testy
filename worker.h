#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include "profiler.h"

class PosixTimer;
class QTimer;
//class GPIO;
#include "GPIO/GPIO.h"
using namespace exploringBB;
class Worker : public QObject
{
    Q_OBJECT
public:
//    explicit Program(QObject *parent = 0);
    explicit Worker(int loopNumber = 200, bool notificate = false, int period = 100,
                     bool save = false, int whichTimer = 0, bool highPrio = false,
                     bool load = false, QString name = 0, QObject *parent = 0);
    ~Worker();

signals:
    void done();

public slots:
    void update();
    void atThreadStart();

private:
    bool mNotificate;
    bool mHighPrio;
    int mLoopNumber;
    int mCounter;
    int mPeriod;
    QString mName;
    Profiler mProfiler;
    PosixTimer *mPosixTimer;
    QTimer *mQTimer;
    GPIO *mPin;



};

#endif // PROGRAM_H
