QT       += core sql gui widgets
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Auto_salon
TEMPLATE = app
requires(qtConfig(tableview))
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    adminwindow.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    registracija.cpp

HEADERS += \
    about.h \
    adminwindow.h \
    login.h \
    mainwindow.h \
    registracija.h

FORMS += \
    about.ui \
    adminwindow.ui \
    login.ui \
    mainwindow.ui \
    registracija.ui
QT += core

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pictures.qrc
