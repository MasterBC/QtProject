#ifndef LOGINUI_H
#define LOGINUI_H

#include "base/UI/uibase.h"
#include "base/global.h"
namespace Ui {
class LoginUI;
}
class HandleMsg;
class LoginUI : public UIbase
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = 0);
    ~LoginUI();

private slots:
    void on_pushButtonEnter_clicked();
    void on_recvData(QByteArray data);

private:
    Ui::LoginUI     *ui;
};

#endif // LOGINUI_H
