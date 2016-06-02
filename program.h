#ifndef SUPER_H
#define SUPER_H

#include <QObject>
#include "worker.h"
#include <QThread>
class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(int loopNumber = 200, int period = 100,
                     bool save = false, bool load = false, QObject *parent = 0);
    ~Program();

signals:

public slots:
    void finish();

private:
    Worker *p1, *p2, *p3, *p4;
    QThread t1, t2, t3, t4;
};

#endif // SUPER_H
