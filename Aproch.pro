QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/AprochConnectionGraphicsObject.cpp \
    Core/AprochConnection.cpp \
    Core/AprochFlowScene.cpp \
    Core/AprochFlowView.cpp \
    Core/AprochNode.cpp \
    Core/AprochNodeDataModel.cpp \
    Core/AprochNodeGraphicsObject.cpp \
    Core/AprochNodePainter.cpp \
    Core/AprochStyle.cpp \
    main.cpp \
    Editor/AprochMainWindow.cpp

HEADERS += \
    Core/AprochConnectionGraphicsObject.h \
    Core/Aproch.h \
    Core/AprochConnection.h \
    Core/AprochFlowScene.h \
    Core/AprochFlowView.h \
    Core/AprochNode.h \
    Core/AprochNodeDataModel.h \
    Core/AprochNodeGraphicsObject.h \
    Core/AprochNodePainter.h \
    Core/AprochPort.h \
    Core/AprochStyle.h \
    Core/ISerializable.h \
    Core/IStyle.h \
    Core/Utilities.h \
    Editor/AprochMainWindow.h

FORMS += \
    Editor/UI/aprochmainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource/resource.qrc
