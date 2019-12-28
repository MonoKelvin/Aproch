#include "AprochMainWindow.h"
#include "ui_aprochmainwindow.h"

AprochMainWindow::AprochMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AprochMainWindow)
{
    ui->setupUi(this);

//    auto layout = new QHBoxLayout(this);

//    AprochFlowScene *scene = new AprochFlowScene(registerDataModels());
//    AprochFlowView *view = new AprochFlowView(scene);

//    layout->addWidget(view);
}

AprochMainWindow::~AprochMainWindow()
{
    delete ui;
}
