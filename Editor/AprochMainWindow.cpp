#include "AprochMainWindow.h"
#include "ui_aprochmainwindow.h"

#include "Core/AprochFlowView.h"
#include "Core/AprochFlowScene.h"
#include "Core/AprochDataModelRegistry.h"
#include "Test/MyTestDataModel_1.h"

#include <QHBoxLayout>

using namespace Aproch;

static std::shared_ptr<AprochDataModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<AprochDataModelRegistry>();

    ret->registerModel<MyDataModel>();

    return ret;
}

AprochMainWindow::AprochMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AprochMainWindow)
{
    ui->setupUi(this);

    auto layout = new QHBoxLayout(this);

    AprochFlowScene *scene = new AprochFlowScene(registerDataModels());
    AprochFlowView *view = new AprochFlowView(scene);

    layout->addWidget(view);
}

AprochMainWindow::~AprochMainWindow()
{
    delete ui;
}
