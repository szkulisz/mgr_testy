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
    void insertToFileName(const QString &fileName);
    void write(QString);

private:
    timespec mTimePrevious, mTimeActual, mTimerDifference;
    int mPeriod;
    QTextStream *mLogStream;
    QFile mLogFile;
    QString mFileName;
    bool mSave;


    timespec countDifference(timespec start, timespec stop);

};



#endif // PROFILER_H
