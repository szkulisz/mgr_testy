#ifndef POSIXTIMER_H
#define POSIXTIMER_H

#include <QObject>

class PosixTimer : public QObject
{
    Q_OBJECT
public:
    explicit PosixTimer(QObject *parent = 0);

signals:

public slots:
};

#endif // POSIXTIMER_H
