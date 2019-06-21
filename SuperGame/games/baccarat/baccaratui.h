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
class QLabel;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;
class QStandardItemModel;

class baccaratLogic;
class BaccaratUI : public GameMap
{
    Q_OBJECT

public:
    explicit BaccaratUI(QWidget *parent = 0);
    ~BaccaratUI();

    void enterScene(int scene,const std::string& data); //进入场景
    void changeState(int state,const std::string& data);//状态改变
    bool gameHandle(int code,const std::string& data);//游戏逻辑处理
    bool updateInfo(int code, const std::string& data);//更新游戏信息
    protected:
    bool eventFilter(QObject *watched, QEvent *event);

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

    qint64 m_PlayerGold;
    
    // 六张扑克牌

    bool m_animatIsFinish;
    baccaratLogic*  m_logic;

    QTimer* m_timer;
    QLabel* m_select;
    QStandardItemModel* m_modelPlayers;
    QParallelAnimationGroup* m_animationGroup;
    QSequentialAnimationGroup* m_cardGroup;

    
};

#endif // BACCARATUI_H
