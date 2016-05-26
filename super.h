#ifndef SUPER_H
#define SUPER_H

#include <QObject>
#include "program.h"
#include <QThread>
class Super : public QObject
{
    Q_OBJECT
public:
    explicit Super(int loopNumber = 200, int notification = 20, int period = 100,
                     bool save = false, QObject *parent = 0);

signals:

public slots:
    void finish();

private:
    Program *p1, *p2, *p3, *p4;
    QThread t1, t2, t3, t4;
};

#endif // SUPER_H
