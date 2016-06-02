#include <QCoreApplication>
#include "worker.h"
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "program.h"

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
    interval.setDefaultValue("100");
    parser.addOption(interval);
    // -n -> umber of loops
    QCommandLineOption length("n", QCoreApplication::translate("main","number of loops"),
                              QCoreApplication::translate("main","loops"));
    length.setDefaultValue("10");
    parser.addOption(length);
    // -s -> if save to file
    QCommandLineOption save("s", QCoreApplication::translate("main","save to file option"));
    parser.addOption(save);
    // -l -> if load is present
    QCommandLineOption load("l", QCoreApplication::translate("main","indicate if load is present in system"));
    parser.addOption(load);
    parser.process(a);


    Program super(parser.value(length).toInt(), parser.value(interval).toInt(),
                parser.isSet(save), parser.isSet(load));

    return a.exec();
}

