#ifndef BACCARATUI_H
#define BACCARATUI_H

#include "base/UI/gamemap.h"

namespace Ui {
class BaccaratUI;
}
class QDialog;
class QHBoxLayout;
class Poker;
class QTimer;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;
class baccaratGameLogic;
class BaccaratUI : public GameMap
{
    Q_OBJECT

public:
    explicit BaccaratUI(QWidget *parent = 0);
    ~BaccaratUI();


    void changeState(int state,const std::string& data);
    void enterScene(int scene,const std::string& data);
    bool gameHandle(int code,const std::string& data);
protected slots:
    void onTimeout();
private:
    // 中奖区域动画
    void awardAnimation(const std::string& strAward);
    // 发牌动画
    void dealCardAnimation(const std::string& strPlayerCard,const std::string& strBankerCard);

    Ui::BaccaratUI *ui;
    int m_freeTime;
    int m_betTime;
    int m_openTime;
    int m_lastTime;                 // 剩余时长
    int m_curState;                 // 当前状态

    float m_PlayerGold;
    
    // 六张扑克牌

    bool m_animatIsFinish;
    QTimer* m_timer;
    baccaratGameLogic*  m_logic;
    QParallelAnimationGroup* m_animationGroup;
    QSequentialAnimationGroup* m_cardGroup;

    
};

#endif // BACCARATUI_H
