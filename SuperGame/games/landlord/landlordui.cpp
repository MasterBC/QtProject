#include "landlordui.h"
#include "ui_landlordui.h"

LandlordUI::LandlordUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::LandlordUI)
{
    ui->setupUi(this);
}

LandlordUI::~LandlordUI()
{
    delete ui;
}

void LandlordUI::changeState(int state, const std::string &data)
{

}

void LandlordUI::enterScene(int scene, const std::string &data)
{

}

bool LandlordUI::gameHandle(int code, const std::string &data)
{

}
