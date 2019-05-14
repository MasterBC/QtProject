#include "mahjongui.h"
#include "ui_mahjong.h"

MahjongUI::MahjongUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::Mahjong)
{
    ui->setupUi(this);
}

MahjongUI::~MahjongUI()
{
    delete ui;
}

void MahjongUI::changeState(int state, const std::string &data)
{

}

void MahjongUI::enterScene(int scene, const std::string &data)
{

}

bool MahjongUI::gameHandle(int code, const std::string &data)
{

}
