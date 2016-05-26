QT += core
QT -= gui

TARGET = load_test
    target.files = load_test
    target.path = /home/szymon/load_tests
INSTALLS+=target

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    program.cpp \
    profiler.cpp \
    posixtimer.cpp

HEADERS += \
    program.h \
    profiler.h \
    posixtimer.h

QMAKE_LIBS += -lrt
