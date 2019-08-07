TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_CFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_CFLAGS += --coverage
LIBS += -lgcov
#LIBS += --coverage
SOURCES += test_hello.c
