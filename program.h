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
                     bool save = false, bool load = false, QObject *parent = 0);
    ~Program();

signals:

public slots:
    void finish();
    void onTimeoutLog(long long);

private:
    Worker *p1, *p2, *p3, *p4;
    QThread t1, t2, t3, t4, logger;
    pid_t mChildPid;
    bool mLoad, mSave;
    QFile mLogFile;
    QString mFileName;
    QTextStream *mLogStream = nullptr;
};

#endif // SUPER_H
