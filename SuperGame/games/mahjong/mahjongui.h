#ifndef MAHJONGUI_H
#define MAHJONGUI_H

#include "base/UI/gamemap.h"

namespace Ui {
class Mahjong;
}

class MahjongUI : public GameMap
{
    Q_OBJECT

public:
    explicit MahjongUI(QWidget *parent = 0);
    ~MahjongUI();

    void changeState(int state,const std::string& data);
    void enterScene(int scene,const std::string& data);
    bool gameHandle(int code,const std::string& data);

private:
    Ui::Mahjong *ui;
};

#endif // MAHJONG_H
