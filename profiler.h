#ifndef PROFILER_H
#define PROFILER_H

#include <time.h>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QObject>

class QTextStream;


class Profiler : public QObject
{
    Q_OBJECT
public:
    explicit Profiler(QObject *parent = 0);
    explicit Profiler(int loops,int period, bool save, QObject *parent = 0);
    ~Profiler();

    void startProfiling();
    void updateProfiling();
    int getDifferenceInSeconds();
    int getDifferenceInMiliseconds();
    int getDifferenceInMicroseconds();
    long long getDifferenceInNanoseconds();
    void logToFile();

    void setPeriod(int period);
    void startLogging(int loops, bool save, const QString &fileName);



private:
    bool mSave;
    int mPeriod;
    timespec mTimePrevious, mTimeActual, mTimerDifference;
    int mLoops;
    long long *mLogTable;
    unsigned int mLogTableIdx=0;
    QFile mLogFile;
    QString mFileName;
    QTextStream *mLogStream = nullptr;

    timespec countDifference(timespec start, timespec stop);

};





#endif // PROFILER_H
