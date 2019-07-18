////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): uibase.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-04-26
/// 功能(@brief) ：
/// 目的(@target)：
///     1.去原始边框
///     2.可拖拽\可拉伸       已设置鼠标跟踪
///     3.右键菜单 关闭\退出   退出时会触发exitSig信号
///     4.添加背景音乐
/// 备注(@attention): 基础界面类
////////////////////////////////////////////////////

#ifndef UIBASE_H
#define UIBASE_H

#include <QWidget>
class QMediaPlayer;
class QMediaPlaylist;
class UIbase : public QWidget
{
    Q_OBJECT
public:
    // 边框方向
    enum Direction {
        UP, DOWN, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE
    };

    explicit UIbase(QWidget *parent = nullptr);
    virtual ~UIbase();
    virtual void setBackMusic(const QString& strPath,int volume = 5);
    virtual void setBGMVolume(int volume);
    virtual void addBGM(const QString& strPath);
    virtual void playBGM();
    virtual void pauseBGM();
    virtual void stopBGM();
    virtual void setStretch(bool isCan);//是否支持拉伸
    virtual void setShowFull(bool isCan);//是否支持全屏



signals:
    void closeSig();
public slots:

protected:
    // 拖拽界面
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
    // 鼠标事件——控制显示
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    void judgeRegionSetCursor(const QPoint&currentPoint); //根据拖拽拉伸

    const int       m_padding = 10;      // c++11 可以直接赋值
    bool            m_isLeftPressDown;  // 判断左键是否按下
    bool            m_isStretch;        // 是否支持拉伸
    bool            m_isShowFull;       // 是否支持全屏
    Direction       m_direction;        // 拉伸方向
    QPoint          m_dragPosition;     // 窗口移动拖动时需要记住的点
    QMediaPlayer*   m_mediaPlayer;      // 音乐播放
    QMediaPlaylist* m_playlist;         // 音乐列表
    // QSoundEffect*   m_effect; //只能播放wav格式
};

#endif // UIBASE_H
