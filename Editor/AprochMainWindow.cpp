#include "AprochMainWindow.h"
#include "ui_aprochmainwindow.h"

#include "Core/AprochFlowView.h"
#include "Core/AprochFlowScene.h"

#include <QHBoxLayout>

using Aproch::AprochFlowView;
using Aproch::AprochFlowScene;

AprochMainWindow::AprochMainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AprochMainWindow)
{
    ui->setupUi(this);

    auto layout = new QHBoxLayout(this);

    layout->addWidget(new AprochFlowView(new AprochFlowScene));

}

AprochMainWindow::~AprochMainWindow()
{
    delete ui;
}

