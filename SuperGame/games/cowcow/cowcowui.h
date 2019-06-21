#ifndef COWCOWUI_H
#define COWCOWUI_H

#include "base/UI/gamemap.h"

namespace Ui {
class CowcowUI;
}

class CowcowUI : public GameMap
{
    Q_OBJECT

public:
    explicit CowcowUI(QWidget *parent = 0);
    ~CowcowUI();
    void enterScene(int scene,const std::string& data);
    void changeState(int state,const std::string& data);
    bool gameHandle(int code,const std::string& data);
    bool updateInfo(int code, const std::string& data);
private:
    Ui::CowcowUI *ui;
};

#endif // COWCOWUI_H
