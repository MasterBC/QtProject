#include "cowcowui.h"
#include "ui_cowcowui.h"

CowcowUI::CowcowUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::CowcowUI)
{
    ui->setupUi(this);
}

CowcowUI::~CowcowUI()
{
    delete ui;
}

void CowcowUI::changeState(int state, const std::string &data)
{

}

void CowcowUI::enterScene(int scene, const std::string &data)
{

}

bool CowcowUI::gameHandle(int code, const std::string &data)
{

}
