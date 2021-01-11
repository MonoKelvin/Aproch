#include "stdafx.h"
#include "AMainWindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    AMainWindow mainWindow;
    a.installNativeEventFilter(&mainWindow);

    mainWindow.show();

    return a.exec();
}
