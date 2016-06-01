QT += core
QT -= gui

TARGET = load_test_th
    target.files = load_test_th
    target.path = /home/szymon/load_tests_th
INSTALLS+=target

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    program.cpp \
    profiler.cpp \
    posixtimer.cpp \
    super.cpp

HEADERS += \
    program.h \
    profiler.h \
    posixtimer.h \
    super.h

QMAKE_LIBS += -lrt
#QMAKE_LIBS += -pthreads
LIBS += -pthread

#QMAKE_CXXFLAGS += -pthreads
