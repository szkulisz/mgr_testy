#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include "profiler.h"

class Program : public QObject
{
    Q_OBJECT
public:
//    explicit Program(QObject *parent = 0);
    explicit Program(int loopNumber = 200, int notification = 20, int period = 100,
                     bool save = false, QObject *parent = 0);

signals:

public slots:
    void update();

private:
    Profiler mProfiler;
    int mLoopNumber;
    int mNotificationNumber;
};

#endif // PROGRAM_H
