TEMPLATE = app
CONFIG += console c99
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    CSVParser.h \
    HashTable.h \
    JSON.h \
    MeteoData.h \
    Util.h
