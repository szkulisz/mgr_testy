#include <QCoreApplication>
#include "program.h"
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "super.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("load_test");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Program testujacy dzialanie timerow");
    parser.addHelpOption();
    parser.addVersionOption();
    // -i -> interwał
    QCommandLineOption interval("i", QCoreApplication::translate("main","Interwał timera"));
    interval.setDefaultValue("100");
    parser.addOption(interval);
    // -l -> ilość pętli
    QCommandLineOption length("l", QCoreApplication::translate("main","Ilość pętli do wykonania przez program"));
    length.setDefaultValue("10");
    parser.addOption(length);
    // -s -> czy zapisywać do pliku
    QCommandLineOption save("s", QCoreApplication::translate("main","Logowanie do pliku"));
    parser.addOption(save);
    parser.process(a);


    Super super(parser.value(length).toInt(), parser.value(interval).toInt(), parser.isSet(save));

    return a.exec();
}

