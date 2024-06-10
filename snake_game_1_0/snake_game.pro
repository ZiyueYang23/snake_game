QT       += core gui
QT += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    food.cpp \
    game.cpp \
    game_override.cpp \
    leaderboard.cpp \
    main.cpp \
    mainwindow.cpp \
    map.cpp \
    obstacle.cpp \
    snake.cpp \
    start_menu.cpp


HEADERS += \
    food.h \
    game.h \
    head.h \
    leaderboard.h \
    mainwindow.h \
    map.h \
    obstacle.h \
    snake.h \
    start_menu.h


FORMS += \
    mainwindow.ui \
    start_menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourse.qrc

DISTFILES += \
    leaderboard.txt

QMAKE_LFLAGS += -no-pie
