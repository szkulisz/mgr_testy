#ifndef SUPER_H
#define SUPER_H

#include <QObject>
#include "worker.h"
#include <QBuffer>
#include <QByteArray>
#include <QThread>
#include <QTimer>
#include <sstream>
#include "logger.h"

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
    void onTimeoutLog2();

private:
    Worker *p1, *p2, *p3, *p4;
    QThread t1, t2, t3, t4, logger;
    pid_t mChildPid;
    bool mLoad, mSave;
    QFile mLogFile;
    QString mFileName;
    QTextStream *mLogStream = nullptr;
    QBuffer mBuffer;
    QByteArray mArray;
    std::ostringstream mStream;
    QTimer mTimer;
    Logger mLogger;
};

#endif // SUPER_H
