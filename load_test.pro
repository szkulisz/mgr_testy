QT += core
QT -= gui

TARGET = load_test2
    target.files = load_test2
    target.path = /home/szymon/load_tests2
INSTALLS+=target

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    profiler.cpp \
    posixtimer.cpp \
    worker.cpp \
    program.cpp

HEADERS += \
    profiler.h \
    posixtimer.h \
    worker.h \
    program.h

QMAKE_LIBS += -lrt
LIBS += -pthread
