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
    Core/AConnectionPainter.cpp \
    Core/AConnectionGraphicsObject.cpp \
    Core/AConnection.cpp \
    Core/ADataModelRegistry.cpp \
    Core/AFlowScene.cpp \
    Core/AFlowView.cpp \
    Core/ANCInteraction.cpp \
    Core/ANode.cpp \
    Core/ANodeDataModel.cpp \
    Core/ANodeGraphicsObject.cpp \
    Core/ANodePainter.cpp \
    Core/AStyle.cpp \
    Test/MyTestDataModel_1.cpp \
    main.cpp \
    Editor/AprochMainWindow.cpp

HEADERS += \
    Core/AConnectionPainter.h \
    Core/AConnectionGraphicsObject.h \
    Core/Aproch.h \
    Core/AConnection.h \
    Core/ADataModelRegistry.h \
    Core/AFlowScene.h \
    Core/AFlowView.h \
    Core/ANCInteraction.h \
    Core/ANode.h \
    Core/ANodeDataModel.h \
    Core/ANodeGraphicsObject.h \
    Core/ANodePainter.h \
    Core/APort.h \
    Core/AStyle.h \
    Core/ISerializable.h \
    Core/IStyle.h \
    Core/Utilities.h \
    Editor/AprochMainWindow.h \
    Test/MyTestDataModel_1.h

FORMS += \
    Editor/UI/aprochmainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource/resource.qrc
