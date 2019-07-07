#ifndef CHINESECHESSLOGIC_H
#define CHINESECHESSLOGIC_H

struct setInfo
{
    int x;
    int y;
    unsigned char prePiece;
    int toX;
    int toY;
    unsigned char nowPiece;
};
#include <QMap>

class ChineseChessLogic
{
public:
    ChineseChessLogic();

    void initChessBoard();

    bool CanMove(int x, int y, int toX, int toY); // 0:表示成功 -1:表示失败 1:表示红方被将军 2:黑方被将军
    void Rollback();
    bool JiangJun(int x, int y);//0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)0x03表示不能走，处于被将军状态
    inline int GetCode()const{return m_state;}
protected:
    bool verifyChe(int x, int y, int toX, int toY);
    bool verifyMa(int x, int y, int toX, int toY);
    bool verifyXiang(int x, int y, int toX, int toY);
    bool verifyShi(int x, int y, int toX, int toY);
    bool verifyJiang(int x, int y, int toX, int toY);
    bool verifyPao(int x, int y, int toX, int toY);
    bool verifyBing(int x, int y, int toX, int toY);

    bool getPos(unsigned char piece, int& x, int& y);//查找棋子的位置
    unsigned char judgmetColor(int x, int y);//0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)
private:
    int m_state;//当前状态 0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)0x03表示不能走，处于被将军状态
    unsigned char m_arryChessBoard[10][9];
    QList<setInfo*> m_setInfo;
};

#endif // CHINESECHESSLOGIC_H
