QT += widgets
QT += core gui
LIBS += -L/home/physor/qwt-6.1.0/lib -lqwt
LIBS += -LC:\build-qwt-Desktop_Qt_5_2_1_MinGW_32bit-Debug/lib/ -lqwt
INCLUDEPATH += C:\qwt-6.1\src
DEPENDPATH += C:\qwt-6.1\src
INCLUDEPATH += /home/physor/qwt-6.1.0/src
DEPENDPATH += /home/physor/qwt-6.1.0/src

TARGET       = OptMultiObj

HEADERS = \
    mainwindow.h \
    plot.h \
    optimisationengine.h \
    individual.h \
    abstractproblem.h \
    problem_a.h \
    problem_b.h \
    problem_script.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    plot.cpp \
    optimisationengine.cpp \
    individual.cpp \
    abstractproblem.cpp \
    problem_a.cpp \
    problem_b.cpp \
    problem_script.cpp

