#ifndef HOMEUI_H
#define HOMEUI_H

#include "base/UI/uibase.h"
#include <login.pb.h>
#include <QMap>
namespace Ui {
class HomeUI;
}

class QGridLayout;
class QSequentialAnimationGroup;
class QParallelAnimationGroup;
class HomeUI : public UIbase
{
    Q_OBJECT

public:
    explicit HomeUI(QWidget *parent = 0);
    ~HomeUI();
    static int S_GameID;  // 当前的游戏ID
    static int S_GameKindID;
    static int S_GameLevel;


    void updateGameList(go::GameList& list);
    void clearList();
    void clearAnimations();
signals:
    void enterSig(int);
private slots:
    void onEnterGame(int gameID);
    void onExit();
    void on_pushButton_clicked();

private:
    Ui::HomeUI *ui;
    bool m_animatIsFinish;
    QGridLayout* m_layoutGames;
    QSequentialAnimationGroup *m_AnimationGoup;
    //QParallelAnimationGroup *m_AnimationGoup;
};

#endif // HOMEUI_H
