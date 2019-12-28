#include "Editor/AprochMainWindow.h"

#include <QApplication>

#include "Core/AprochFlowView.h"
#include "Core/AprochFlowScene.h"
#include "Core/AprochDataModelRegistry.h"
#include "Test/MyTestDataModel_1.h"

using namespace Aproch;

static std::shared_ptr<AprochDataModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<AprochDataModelRegistry>();

    ret->registerModel<MyDataModel>();

    return ret;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AprochFlowScene scene(registerDataModels());
    AprochFlowView view(&scene);

//    AprochMainWindow w;
//    w.show();

    view.show();
    return a.exec();
}
