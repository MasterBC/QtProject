#ifndef LANDLORDUI_H
#define LANDLORDUI_H
#include "base/UI/gamemap.h"


namespace Ui {
class LandlordUI;
}

class LandlordUI : public GameMap
{
    Q_OBJECT

public:
    explicit LandlordUI(QWidget *parent = 0);
    ~LandlordUI();

    void changeState(int state,const std::string& data);
    void enterScene(int scene,const std::string& data);
    bool gameHandle(int code,const std::string& data);

private:
    Ui::LandlordUI *ui;
};

#endif // LANDLORDUI_H
