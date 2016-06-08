#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QThread>

class Logger : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;

    QThread mThread;
    QFile mLogFile;
    QTextStream *mLogStream;
public:
//    explicit Logger(QObject *parent = 0);
    Logger();
    ~Logger();

signals:

public slots:
};

#endif // LOGGER_H
