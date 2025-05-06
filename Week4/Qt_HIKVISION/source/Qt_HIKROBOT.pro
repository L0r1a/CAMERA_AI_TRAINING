QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

message (Before: $$QMAKE_CFLAGS_UTF8_SOURCE)
QMAKE_CFLAGS_UTF8_SOURCE = /source-charset:windows-1252
message (After: $$QMAKE_CFLAGS_UTF8_SOURCE)

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -L$$PWD/'../../../../../Program Files (x86)/MVS/Development/Libraries/win64/' -lMvCameraControl

INCLUDEPATH += $$PWD/'../../../../../Program Files (x86)/MVS/Development/Includes'
DEPENDPATH += $$PWD/'../../../../../Program Files (x86)/MVS/Development/Includes'

win32:CONFIG(release, debug|release): LIBS += -LC:/opencv/build/x64/vc16/lib/ -lopencv_world4100
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/opencv/build/x64/vc16/lib/ -lopencv_world4100d
else:unix: LIBS += -LC:/opencv/build/x64/vc16/lib/ -lopencv_world4100

INCLUDEPATH += C:/opencv/build/include
DEPENDPATH += C:/opencv/build/include
