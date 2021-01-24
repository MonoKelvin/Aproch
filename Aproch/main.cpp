#include "stdafx.h"
#include "AAprochWidget.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    aproch::AAprochWidget mainWidget;
    a.installNativeEventFilter(&mainWidget);

    mainWidget.show();
    mainWidget.resize(1080, 680);
    mainWidget.move(QApplication::desktop()->screen()->rect().center() - mainWidget.rect().center());

    return a.exec();
}
