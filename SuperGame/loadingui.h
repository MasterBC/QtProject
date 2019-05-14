#ifndef LOADINGUI_H
#define LOADINGUI_H

#include "base/UI/uibase.h"
namespace Ui {
class LoadingUI;
}

class LoadingUI : public UIbase
{
    Q_OBJECT

public:
    explicit LoadingUI(QWidget *parent = 0);
    ~LoadingUI();

private:
    Ui::LoadingUI *ui;
};

#endif // LOADINGUI_H
