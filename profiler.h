#ifndef PROFILER_H
#define PROFILER_H

#include <time.h>
#include <QFile>

#include <QString>

class QTextStream;

class Profiler
{
public:
    Profiler();
    Profiler(int period, bool save);
    ~Profiler();

    void startProfiling();
    void updateProfiling();
    int getDifferenceInSeconds();
    int getDifferenceInMiliseconds();
    int getDifferenceInMicroseconds();
    qlonglong getDifferenceInNanoseconds();
    void logToFile();

    void setPeriod(int period);
    void setSave(bool save);
    void setFileName(const QString &fileName);

private:
    bool mSave;
    int mPeriod;
    timespec mTimePrevious, mTimeActual, mTimerDifference;
    QFile mLogFile;
    QString mFileName;
    QTextStream *mLogStream;

    timespec countDifference(timespec start, timespec stop);

};



#endif // PROFILER_H
