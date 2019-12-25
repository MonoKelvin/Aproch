#ifndef APROCHMAINWINDOW_H
#define APROCHMAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class AprochMainWindow; }
QT_END_NAMESPACE

class AprochMainWindow : public QWidget
{
    Q_OBJECT

public:
    AprochMainWindow(QWidget *parent = nullptr);
    ~AprochMainWindow();

private:
    Ui::AprochMainWindow *ui;
};
#endif // APROCHMAINWINDOW_H
