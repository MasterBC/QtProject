    #ifndef GAMEFRAMEUI_H
#define GAMEFRAMEUI_H

#include "base/UI/uibase.h"
#include <login.pb.h>


namespace Ui {
class GameFrameUI;
}

class QGridLayout;
class GameFrameUI : public UIbase
{
    Q_OBJECT

public:
    explicit GameFrameUI(QWidget *parent = 0);
    ~GameFrameUI();
    void updateUserInfo(const go::UserInfo& userInfo);
    void updateRoomInfo(const go::RoomInfo& roomInfo);
private:
    Ui::GameFrameUI *ui;

};

#endif // GAMEFRAMEUI_H
