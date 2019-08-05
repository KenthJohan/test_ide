TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += test.c

HEADERS += debug.h

INCLUDEPATH += "C:\msys64\mingw64\include"

LIBS += -L"C:\msys64\mingw64\lib"

LIBS += -limm32 -lz -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32
LIBS += -luv
