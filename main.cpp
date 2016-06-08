#include <QCoreApplication>
#include "worker.h"
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "program.h"
#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("load_test");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("QTimer and POSIX timer performance tester");
    parser.addHelpOption();
    parser.addVersionOption();
    // -i -> interval
    QCommandLineOption interval("i", QCoreApplication::translate("main","timer interval"),
                                QCoreApplication::translate("main","interval=MSEC"));
    interval.setDefaultValue("1");
    parser.addOption(interval);
    // -n -> umber of loops
    QCommandLineOption length("n", QCoreApplication::translate("main","number of loops"),
                              QCoreApplication::translate("main","loops"));
    length.setDefaultValue("10000");
    parser.addOption(length);
    // -t -> timer type
    QCommandLineOption timer("t", QCoreApplication::translate("main","timer type: 0=QT, 1=POSIX"),
                              QCoreApplication::translate("main","timer"));
    timer.setDefaultValue("0");
    parser.addOption(timer);
    // -s -> if save to file
    QCommandLineOption save("s", QCoreApplication::translate("main","save to file option"));
    parser.addOption(save);
    // -l -> if load is present
    QCommandLineOption load("l", QCoreApplication::translate("main","indicate if load is present in system"));
    parser.addOption(load);
    // -p -> timer in high priority thread
    QCommandLineOption priority("p", QCoreApplication::translate("main","timer in high priority thread"));
    parser.addOption(priority);
    parser.process(a);


    Program program(parser.value(length).toInt(), parser.value(interval).toInt(), parser.value(timer).toInt(),
                parser.isSet(priority), parser.isSet(save), parser.isSet(load));

    return a.exec();
}

