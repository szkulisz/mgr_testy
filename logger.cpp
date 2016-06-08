#include "logger.h"
#include "globals.h"
#include <QCoreApplication>
#include <iostream>
#include <iterator>
#include <time.h>

void Logger::run()
{
        int sts;
        struct sched_param param;
        sts = sched_getparam(0, &param);
//        CHECK(sts,"sched_getparam");
        param.sched_priority = sched_get_priority_min(SCHED_OTHER);
        sts = sched_setscheduler(0, SCHED_OTHER, &param);
//        CHECK(sts,"sched_setscheduler");

    std::cout << param.sched_priority<< " elo z log" << QThread::currentThreadId() << "prio " << param.sched_priority << std::endl;
//    std::cout << "elo z " << mLogFile.isOpen() << std::endl;

    while(1){
        if (gBufferFull.testAndSetAcquire(1,0)) {
            if (gWhichBufferIsActive.load() == 0) {
                std::cout <<gDiffBuffer2.end()->first << ", " << std::endl;
                for( auto&& value : gDiffBuffer2)
                    *mLogStream << value.first << ", " << value.second << "\n";
                gDiffBuffer2.clear();
//                mLogStream->flush();
            }
            if (gWhichBufferIsActive.load() == 1) {
                std::cout <<gDiffBuffer1.end()->first << ", " << std::endl;
                for( auto&& value : gDiffBuffer1)
                    *mLogStream << value.first << ", " << value.second << "\n";
                gDiffBuffer1.clear();
//                mLogStream->flush();
            }
            std::cout << "zapisalem" << std::endl;
        }
        QThread::msleep(50);
        if (gContinueLogging.load() == 0)
            break;
    }
}

//Logger::Logger(QObject *parent) : QObject(parent)
Logger::Logger()
{
    mLogFile.setFileName("logs8.txt");
    mLogFile.open(QFile::WriteOnly | QFile::Text);
    mLogStream = new QTextStream( &mLogFile );
}

Logger::~Logger()
{
    std::cout << "zamykam  " << QThread::currentThreadId() << std::endl;
    if (mLogFile.isOpen()) {
                mLogFile.close();
    }
    delete mLogStream;
}

