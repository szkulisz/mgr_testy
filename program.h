#ifndef SUPER_H
#define SUPER_H

#include <QObject>
#include "worker.h"
#include <QThread>

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(int loopNumber = 200, int period = 100, int timer = 0, bool highPrio = false,
                     bool save = false, bool load = false, bool rtKernel = false, QObject *parent = 0);
    ~Program();

signals:

public slots:
    void finish();

private:
    Worker *mWorker;
    QThread mTimerThread;
    pid_t mChildPid;
    bool mLoad, mSave;
};

#endif // SUPER_H
