TEMPLATE = app
QT += widgets network
DEPENDPATH += .
INCLUDEPATH += .


SOURCES += \
    Client.cpp \
    fenserveur.cpp \
    ../../RSA/RSAFunctions.cpp \
    main.cpp

HEADERS += \
    Client.h \
    fenserveur.h \
    ../../RSA/RSAFunctions.h
