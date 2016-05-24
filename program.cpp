#include "program.h"
#include <QTimer>
#include <QCoreApplication>
#include <time.h>
#include <QFile>
#include <iostream>
#include <iomanip>
#include "profiler.h"

Program::Program(int loop, int notification, int period, bool save, QObject *parent)
    : QObject(parent),
      mLoopNumber(loop),
      mNotificationNumber(notification)
{

    mProfiler.setPeriod(period);
    mProfiler.setSave(save);

    mProfiler.startProfiling();


}

void Program::update() {
    static int counter;

    mProfiler.updateProfiling();
    //    std::cout << std::setiosflags(std::ios::right) << std::resetiosflags(std::ios::left) << std::setw(10);
    //    std::cout << mProfiler.getDifferenceInMicroseconds() << std::endl;
    mProfiler.logToFile();



    if (counter%mNotificationNumber == 0)
        std::cout << '\r' << counter << std::flush;
    if (counter >= mLoopNumber) {
        std::cout << std::endl;
        QCoreApplication::quit();
    }
    ++counter;


}

