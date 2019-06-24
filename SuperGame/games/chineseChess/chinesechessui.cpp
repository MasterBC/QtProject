#include "chinesechessui.h"
#include "ui_chinesechessui.h"
#include "chinesechesslogic.h"
#include "base/UI/uimeterprocessbar.h"

#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <QObjectList>
#include <QPropertyAnimation>
#include <QDialog>

ChineseChessUI::ChineseChessUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::ChineseChessUI)
{
    ui->setupUi(this);
    setStretch(false);
    setShowFull(false);

    m_isSelect = false;
    m_curPieces = nullptr;
    m_toPoint = QPoint(0,0);

    m_logic = new ChineseChessLogic();
    m_selectUI = new QLabel(this);
    m_animation = new QPropertyAnimation(m_selectUI, "geometry");

    init(true);
}

ChineseChessUI::~ChineseChessUI()
{
    m_animation->deleteLater();
    m_selectUI->deleteLater();
    delete m_logic;
    delete ui;
}

void ChineseChessUI::enterScene(int scene, const std::string &data)
{
    resetChessBoard();
    //    UiMeterProcessBar* bar = new UiMeterProcessBar(ui->CheckerBoard);
    //    bar->setMaxValue(100);
    //    bar->setMinValue(0);
    //    bar->setDuration(20);
    //    bar->show();
}

void ChineseChessUI::changeState(int state, const std::string &data)
{

}

bool ChineseChessUI::gameHandle(int code, const std::string &data)
{

}

bool ChineseChessUI::updateInfo(int code, const std::string &data)
{
    
}

bool ChineseChessUI::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == event->MouseButtonDblClick ) return true;

    QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(event);
    if(mouseEvent->type() == mouseEvent->MouseButtonPress ){
        QLabel* label = qobject_cast<QLabel*>(watched);
        if(label)
        {
            if(m_isRed)
            {//只能选红方的棋子
                if( -1 != label->indent())
                    return GameMap::eventFilter(watched, event);

            }else
            {//只能选黑方的棋子
                if(1 != label->indent())
                    return GameMap::eventFilter(watched, event);
            }



            qDebug()<<"选中";
            m_curPieces = label;
            QRect rect = label->geometry();
            m_selectUI->setGeometry(rect.x()+10, rect.y()+10, rect.width(), rect.height());
            m_selectUI->show();

            m_animation->stop();
            m_animation->setStartValue(QRect(rect.x()+15, rect.y()+15, rect.width()-10, rect.height()-10));
            m_animation->setEndValue(QRect(rect.x()+10, rect.y()+10, rect.width(), rect.height()));
            m_animation->setEasingCurve(QEasingCurve::Linear);
            m_animation->setDuration(400);//无限循环
            m_animation->setLoopCount(-1);
            m_animation->start();
            m_isSelect = true;

        }

    }
    return GameMap::eventFilter(watched, event);
}

void ChineseChessUI::mousePressEvent(QMouseEvent *e)
{
    if(m_isSelect)
    {
        m_toPoint = e->pos();
    }

    return GameMap::mousePressEvent(e);
}

void ChineseChessUI::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_isSelect && nullptr != m_curPieces)
    {
        QRect curRect = m_curPieces->geometry();
        if(!curRect.contains(m_toPoint))
        {
            QRect endRect = adjustRect(m_toPoint);
            if(0 != endRect.x())
            {
                QPoint start =  toChessBoardPos(curRect);
                QPoint end =  toChessBoardPos(endRect);
                if(m_logic->CanMove(start.x(),start.y(),end.x(),end.y()))
                {
                    m_isRed = !m_isRed;
                    m_isSelect = false;
                    m_selectUI->hide();

                       unsigned char code = m_logic->JiangJun(end.x(),end.y());

                    if(0x03 != code && killPiece(endRect))
                    {
                        m_curPieces->setGeometry( endRect );
                        m_curPieces = nullptr;
                    }

                    if(0x01 == code)
                    {
                        QDialog dlg(this);
                        dlg.setMinimumSize(200,180);
                        dlg.setAutoFillBackground(true);
                        dlg.setStyleSheet("border-image: url(:/img/chineseChess/bkg2.png)");
                        dlg.exec();
                    }else if(0x02 == code)
                    {
                        QDialog dlg(this);
                        dlg.setMinimumSize(200,180);
                        dlg.setAutoFillBackground(true);
                        dlg.setStyleSheet("border-image: url(:/img/chineseChess/bkg1.png)");
                        dlg.exec();
                    }

                }
            }
        }
    }
    return GameMap::mouseReleaseEvent(e);
}

QRect ChineseChessUI::adjustRect(const QPoint &pt)
{
    // 根据起始点进行适配
    QRect leftTop = ui->label_JunR1->geometry();

    // 固定大小
    int height = 80;
    int width = 80;

    bool isHave = false;
    QRect rectPoint;

    for(int col = 0; col < 9; col++)
    {
        for(int row = 0; row < 10; row++)
        {
            rectPoint.setRect(leftTop.x()+col*width, leftTop.y()+row*height, 80, 60 );
            if(rectPoint.contains(pt))
            {
                isHave = true;
                break;
            }
        }
        if(isHave)break;
    }
    return isHave?rectPoint:QRect(0,0,0,0);
}

QPoint ChineseChessUI::toChessBoardPos(const QRect &rect)
{
    QRect leftTop = ui->label_JunR1->geometry();
    int startX = leftTop.x();
    int startY = leftTop.y();

    int toX = rect.x();
    int toY = rect.y();

    int x = (toY - startY)/80;
    int y = (toX - startX)/80;

    return QPoint(x, y);
}

bool ChineseChessUI::killPiece(const QRect &rect)
{
    QObjectList list = ui->CheckerBoard->children();
    QLabel *pieces = nullptr;
    foreach( QObject *obj, list)
    {
        pieces = qobject_cast<QLabel*>(obj);
        if(pieces && pieces->geometry().contains(rect)){
            pieces->hide();
            if(pieces == ui->label_Jiang || pieces == ui->label_Shuai)
            {
                QDialog dlg(this);
                dlg.setMinimumSize(200,180);
                dlg.setAutoFillBackground(true);
                dlg.setStyleSheet("border-image: url(:/img/chineseChess/yingjiemian.png)");
                dlg.exec();
                resetChessBoard();
                return false;
            }

        }
    }
    return true;
}

void ChineseChessUI::resetChessBoard()
{
    m_logic->initChessBoard();
    //重新布置棋子
    ui->label_Jiang->setGeometry(QRect(370, 730, 80, 60));
    ui->label_Jiang->show();        //1

    ui->label_ShiB2->setGeometry(QRect(450, 730, 80, 60));
    ui->label_ShiB2->show();        //2

    ui->label_ZuB3->setGeometry(QRect(370, 490, 80, 60));
    ui->label_ZuB3->show();        //3

    ui->label_XiangB2->setGeometry(QRect(530, 730, 80, 60));
    ui->label_XiangB2->show();      //4

    ui->label_ZuB2->setGeometry(QRect(210, 490, 80, 60));
    ui->label_ZuB2->show();         //5

    ui->label_ZuB1->setGeometry(QRect(50, 490, 80, 60));
    ui->label_ZuB1->show();         //6

    ui->label_CheB1->setGeometry(QRect(50, 730, 80, 60));
    ui->label_CheB1->show();        //7

    ui->label_MaB1->setGeometry(QRect(130, 730, 80, 60));
    ui->label_MaB1->show();         //8

    ui->label_XiangB1->setGeometry(QRect(210, 730, 80, 60));
    ui->label_XiangB1->show();      //9

    ui->label_ShiB1->setGeometry(QRect(290, 730, 80, 60));
    ui->label_ShiB1->show();        //10

    ui->label_ZuB4->setGeometry(QRect(530, 490, 80, 60));
    ui->label_ZuB4->show();         //11

    ui->label_PaoB2->setGeometry(QRect(610, 570, 80, 60));
    ui->label_PaoB2->show();        //12

    ui->label_CheB2->setGeometry(QRect(690, 730, 80, 60));
    ui->label_CheB2->show();        //13

    ui->label_PaoB1->setGeometry(QRect(130, 570, 80, 60));
    ui->label_PaoB1->show();        //14

    ui->label_MaB2->setGeometry(QRect(610, 730, 80, 60));
    ui->label_MaB2->show();         //15

    ui->label_ZuB5->setGeometry(QRect(690, 490, 80, 60));
    ui->label_ZuB5->show();         //32

    ui->label_BingR1->setGeometry(QRect(50, 250, 80, 60));
    ui->label_BingR1->show();         //16

    ui->label_BingR2->setGeometry(QRect(210, 250, 80, 60));
    ui->label_BingR2->show();         //17

    ui->label_BingR3->setGeometry(QRect(370, 250, 80, 60));
    ui->label_BingR3->show();         //18

    ui->label_BingR4->setGeometry(QRect(530, 250, 80, 60));
    ui->label_BingR4->show();         //19

    ui->label_PaoR2->setGeometry(QRect(610, 170, 80, 60));
    ui->label_PaoR2->show();         //20

    ui->label_BingR5->setGeometry(QRect(690, 250, 80, 60));
    ui->label_BingR5->show();         //21

    ui->label_PaoR1->setGeometry(QRect(130, 170, 80, 60));
    ui->label_PaoR1->show();         //22

    ui->label_JunR1->setGeometry(QRect(50, 10, 80, 60));
    ui->label_JunR1->show();         //23

    ui->label_JunR2->setGeometry(QRect(690, 10, 80, 60));
    ui->label_JunR2->show();         //24

    ui->label_MaR1->setGeometry(QRect(130, 10, 80, 60));
    ui->label_MaR1->show();         //25

    ui->label_XiangR1->setGeometry(QRect(210, 10, 80, 60));
    ui->label_XiangR1->show();         //26

    ui->label_MaR2->setGeometry(QRect(610, 10, 80, 60));
    ui->label_MaR2->show();         //27

    ui->label_XiangR2->setGeometry(QRect(530, 10, 80, 60));
    ui->label_XiangR2->show();         //28

    ui->label_ShiR1->setGeometry(QRect(290, 10, 80, 60));
    ui->label_ShiR1->show();         //29

    ui->label_ShiR2->setGeometry(QRect(450, 10, 80, 60));
    ui->label_ShiR2->show();         //30

    ui->label_Shuai->setGeometry(QRect(370, 10, 80, 60));
    ui->label_Shuai->show();         //31
}

void ChineseChessUI::init(bool isRedStart)
{
    m_isRed = isRedStart;
    m_selectUI->setStyleSheet("border-image: url(:/img/chineseChess/selected.png);");
    m_selectUI->hide();

    QObjectList list = ui->CheckerBoard->children();
    QLabel *pieces = nullptr;
    foreach( QObject *obj, list)
    {
        pieces = qobject_cast<QLabel*>(obj);
        if(pieces){
            pieces->installEventFilter(this);
        }
    }
}
