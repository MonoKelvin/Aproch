#include "Editor/AprochMainWindow.h"

#include <QApplication>

#include "Core/AFlowView.h"
#include "Core/AFlowScene.h"
#include "Core/ADataModelRegistry.h"
#include "Test/MyTestDataModel_1.h"

using namespace Aproch;

static std::shared_ptr<ADataModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<ADataModelRegistry>();

    ret->registerModel<MyDataModel>();

    return ret;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AFlowScene scene(registerDataModels());
    AFlowView view(&scene);

//    AprochMainWindow w;
//    w.show();

    view.show();
    return a.exec();
}
