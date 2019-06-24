#-------------------------------------------------
#
# Project created by QtCreator 2019-04-22T21:10:11
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11  resources_big
TARGET = SuperGame
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += PROTOBUF_USE_DLLS
SOURCES += \
        main.cpp \
    network/protobuffer/include/baccarat.pb.cc \
    network/protobuffer/include/comand.pb.cc \
    network/protobuffer/include/cowcow.pb.cc \
    network/protobuffer/include/fishLord.pb.cc \
    network/protobuffer/include/gamecomm.pb.cc \
    network/protobuffer/include/landLords.pb.cc \
    network/protobuffer/include/login.pb.cc \
    network/protobuffer/include/mahjong.pb.cc \
    network/handlemsg.cpp \
    loginui.cpp \
    gameframeui.cpp \
    homeui.cpp \
    loadingui.cpp \
    superman.cpp \
    games/baccarat/baccaratui.cpp \
    base/UI/gamebutton.cpp \
    base/UI/uibase.cpp \
    base/UI/gamemap.cpp \
    base/UI/chronograph.cpp \
    games/landlord/landlordui.cpp \
    base/UI/poker.cpp \
    base/UI/uilabel.cpp \
    games/mahjong/mahjongui.cpp \
    games/cowcow/cowcowui.cpp \
    games/landlord/landlordlogic.cpp \
    games/baccarat/baccaratlogic.cpp \
    base/UI/uichips.cpp \
    base/UI/uibetarea.cpp \
    base/UI/uitextscroll.cpp \
    games/chineseChess/chinesechessui.cpp \
    games/chineseChess/chinesechesslogic.cpp \
    base/UI/uimeterprocessbar.cpp

HEADERS += \
    login.pb.h \
    network/protobuffer/include/baccarat.pb.h \
    network/protobuffer/include/comand.pb.h \
    network/protobuffer/include/cowcow.pb.h \
    network/protobuffer/include/fishLord.pb.h \
    network/protobuffer/include/gamecomm.pb.h \
    network/protobuffer/include/landLords.pb.h \
    network/protobuffer/include/login.pb.h \
    network/protobuffer/include/mahjong.pb.h \
    base/global.h \
    base/Magic_Singleton.h \
    network/handlemsg.h \
    loginui.h \
    gameframeui.h \
    homeui.h \
    loadingui.h \
    superman.h \
    games/baccarat/baccaratui.h \
    base/UI/gamebutton.h \
    base/UI/uibase.h \
    base/UI/gamemap.h \
    base/UI/chronograph.h \
    games/landlord/landlordui.h \
    base/UI/poker.h \
    base/common.h \
    base/UI/uilabel.h \
    games/mahjong/mahjongui.h \
    games/cowcow/cowcowui.h \
    games/landlord/landlordlogic.h \
    games/baccarat/baccaratlogic.h \
    base/UI/uichips.h \
    base/UI/uibetarea.h \
    base/UI/uitextscroll.h \
    games/chineseChess/chinesechessui.h \
    games/chineseChess/chinesechesslogic.h \
    base/UI/uimeterprocessbar.h


FORMS += \
        mainwindow.ui \
    loginui.ui \
    gameframeui.ui \
    homeui.ui \
    loadingui.ui \
    games/baccarat/baccaratui.ui \
    games/landlord/landlordui.ui \
    games/mahjong/mahjong.ui \
    games/cowcow/cowcowui.ui \
    games/chineseChess/chinesechessui.ui


win32: LIBS += -L$$PWD/network/protobuffer/lib/ -lprotobuf

INCLUDEPATH += $$PWD/network/protobuffer/include
DEPENDPATH += $$PWD/network/protobuffer/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/network/protobuffer/lib/protobuf.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/network/protobuffer/lib/libprotobuf.a

DISTFILES +=

RESOURCES += \
    imgage.qrc
