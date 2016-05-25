#ifndef PROFILER_H
#define PROFILER_H

#include <time.h>
#include <QFile>

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
    long  getDifferenceInNanoseconds();
    void logToFile();

    void setPeriod(int period);
    void setSave(bool save);

private:
    timespec mTimePrevious, mTimeActual, mTimerDifference;
    int mPeriod;
    QTextStream *mLogStream;
    QFile mLogFile;
    bool mSave;


    timespec countDifference(timespec start, timespec stop);

};



#endif // PROFILER_H
