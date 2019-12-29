#include "AprochMainWindow.h"
#include "ui_aprochmainwindow.h"

AprochMainWindow::AprochMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AprochMainWindow)
{
    ui->setupUi(this);

//    auto layout = new QHBoxLayout(this);

//    AFlowScene *scene = new AFlowScene(registerDataModels());
//    AFlowView *view = new AFlowView(scene);

//    layout->addWidget(view);
}

AprochMainWindow::~AprochMainWindow()
{
    delete ui;
}
