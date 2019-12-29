#include "Editor/AprochMainWindow.h"

#include <QApplication>

#include "Core/AFlowView.h"
#include "Core/AFlowScene.h"
#include "Core/ADataModelRegistry.h"
#include "Test/MyTestDataModel_1.h"

using namespace Aproch;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto rm = new ADataModelRegistry;
    rm->registerModel<MyDataModel>();

    AFlowScene scene((QSharedPointer<ADataModelRegistry>(rm)));
    AFlowView view(&scene);

//    AprochMainWindow w;
//    w.show();

    view.show();
    return a.exec();
}
