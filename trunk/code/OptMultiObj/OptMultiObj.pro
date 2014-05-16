QT += widgets
QT += core gui
LIBS += -LC:\build-qwt-Desktop_Qt_5_2_1_MinGW_32bit-Debug/lib/ -lqwt

DEFINES += QMLJSDEBUGGER

INCLUDEPATH += C:\qwt-6.1\src
DEPENDPATH += C:\qwt-6.1\src

TARGET       = OptMultiObj

HEADERS = \
    mainwindow.h \
    plot.h \
    optimisationengine.h \
    individual.h \
    abstractproblem.h \
    problem_a.h \
    problem_b.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    plot.cpp \
    optimisationengine.cpp \
    individual.cpp \
    abstractproblem.cpp \
    problem_a.cpp \
    problem_b.cpp

