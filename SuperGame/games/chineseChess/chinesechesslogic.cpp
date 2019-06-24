#include "chinesechesslogic.h"
#include <QDebug>
//十行九列
unsigned char arryChessBoard[10][9]={
    'c','m','x','s','j','s','x','m','c',
    0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,'p',  0,  0,  0,  0,  0,'p',  0,
    'b',  0,'b',  0,'b',  0,'b',  0,'b',
    0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,
    'B',  0,'B',  0,'B',  0,'B',  0,'B',
    0,'P',  0,  0,  0,  0,  0,'P',  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,
    'C','M','X','S','J','S','X','M','C',
};
ChineseChessLogic::ChineseChessLogic()
{
    initChessBoard();
}

void ChineseChessLogic::initChessBoard()
{
    memcpy_s(m_arryChessBoard,sizeof(m_arryChessBoard),arryChessBoard,sizeof(arryChessBoard));
    m_setInfo.clear();
}

bool ChineseChessLogic::CanMove(int x, int y, int toX, int toY)
{

    // x代表着列 y代表行
    if(10<=x || 9<=y || (x == toX && y == toY))return false;


    //颜色相同则不符合规则(含0x00) 为了表达清晰,两个判断分开写了
    if(judgmetColor(x,y) == judgmetColor(toX,toY))
    {
        qDebug()<<"踩到自己人了";
        return false;
    }


    //获取棋子类型
    bool isOk = false;
    unsigned char piece = m_arryChessBoard[x][y];
    qDebug()<<"当前棋子:"<<piece <<" ->"<<m_arryChessBoard[toX][toY];
    switch (piece) {
    case 'C':
    case 'c':
        //qDebug()<<"车的走法";
        isOk = verifyChe(x,y,toX,toY);
        break;
    case 'M':
    case 'm':
        //qDebug()<<"马的走法";
        isOk = verifyMa(x,y,toX,toY);
        break;
    case 'X':
    case 'x':
        //qDebug()<<"象的走法";
        isOk = verifyXiang(x,y,toX,toY);
        break;
    case 'S':
    case 's':
        //qDebug()<<"士的走法";
        isOk = verifyShi(x,y,toX,toY);
        break;
    case 'J':
    case 'j':
        //qDebug()<<"将军的走法";
        isOk = verifyJiang(x,y,toX,toY);
        break;
    case 'P':
    case 'p':
        //qDebug()<<"大炮的走法";
        isOk = verifyPao(x,y,toX,toY);
        break;
    case 'B':
    case 'b':
        //qDebug()<<"兵的走法";
        isOk = verifyBing(x,y,toX,toY);
        break;
    default:
        break;
    }

    if(isOk)
    {
        qDebug()<<"成功";
        int index = m_setInfo.size();
        setInfo* info = new setInfo();
        info->x = x;
        info->y = y;
        info->toX = toX;
        info->toY = toY;
        info->prePiece = m_arryChessBoard[x][y];
        info->nowPiece = m_arryChessBoard[toX][toY];
        m_setInfo.insert(index+1,info);
        m_arryChessBoard[toX][toY] = m_arryChessBoard[x][y];
        m_arryChessBoard[x][y] = 0;
    }else
    {
        qDebug()<<"失败";
    }


    return isOk;
}

void ChineseChessLogic::Rollback()
{
    int nIdex = m_setInfo.size();
    if( 0 < nIdex )
    {
        setInfo* v = m_setInfo[nIdex-1];
        m_arryChessBoard[v->x][v->y] = v->prePiece;
        m_arryChessBoard[v->toX][v->toY] = v->nowPiece;
        m_setInfo.remove(nIdex-1);
        delete v;
    }


}


unsigned char ChineseChessLogic::JiangJun(int x, int y)
{
    if(0 == m_arryChessBoard[x][y]) return 0x00;

    int toX = 0;
    int toY = 0;
    bool isGet = false;
    unsigned char piece = 0;
    unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方.
    // 移动范围 0 1 2行  3 4 5列
    for(toX=0; toX<3; toX++)
    {
        for(toY=3; toY<6; toY++)
        {
            if('j' == m_arryChessBoard[toX][toY])
            {
                isGet = true;
                break;
            }
        }
        if(isGet)break;
    }

    qDebug()<<"黑方将军坐标:"<<toX<<" "<<toY;

    //校验当前的棋子是否可以到达将军的位置
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<9; j++)
        {
            if('A'<m_arryChessBoard[i][j] && m_arryChessBoard[i][j]<'S')
            {
                piece = m_arryChessBoard[i][j];
                if(CanMove(i,j, toX, toY))
                {//还原坐标
                    m_arryChessBoard[toX][toY] = 'j';
                    m_arryChessBoard[i][j] = piece;
                    if(0x02 == whichSide)
                    {
                        Rollback();
                        return 0x03;
                    }
                    else
                    {
                        return 0x02;
                    }
                }
            }
        }
    }

    isGet = false;
    // 移动范围 7 8 9行  3 4 5列
    for(toX=7; toX<10; toX++)
    {
        for(toY=3; toY<6; toY++)
        {
            if('J' == m_arryChessBoard[toX][toY])
            {
                isGet = true;
                break;
            }
        }
        if(isGet)break;
    }
    qDebug()<<"红方将军坐标:"<<toX<<" "<<toY;
    //校验当前的棋子是否可以到达将军的位置
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<9; j++)
        {

            if('a'<m_arryChessBoard[i][j] && m_arryChessBoard[i][j]<'s')
            {
                piece = m_arryChessBoard[i][j];
                if(CanMove(i,j, toX, toY))
                {//还原坐标
                    m_arryChessBoard[toX][toY] = 'J';
                    m_arryChessBoard[i][j] = piece;
                    if(0x01 == whichSide)
                    {
                        Rollback();
                        return 0x03;
                    }
                    else
                    {
                        return 0x01;
                    }
                }
            }
        }
    }
    return 0x00;
}


bool ChineseChessLogic::verifyChe(int x, int y, int toX, int toY)
{
    if ('C' == m_arryChessBoard[x][y] || 'c' == m_arryChessBoard[x][y])
    {//确认 棋子
        //车的走法  必须同一行  或者 同一列
        if(x == toX)
        {//若同一行 则校验 直到toY列中是否包含其他棋子
            int start = y;
            int end = toY;
            if(toY < y){
                start = toY;
                end = y;
            }
            for(int i=start+1; i<end; i++){
                if(0 < m_arryChessBoard[x][i])
                {
                    qDebug()<<"遇到障碍 "<<m_arryChessBoard[x][i];
                    return false;
                }
            }
        }else if(y == toY)
        {//若同一列 则校验 直到toX行中是否包含其他棋子
            int start = x;
            int end = toX;
            if(toX < x){
                start = toX;
                end = x;
            }
            for(int i=start+1; i<end; i++){
                if(0 < m_arryChessBoard[i][y])
                {
                    qDebug()<<"遇到障碍 "<<m_arryChessBoard[i][y];
                    return false;
                }
            }
        }else{
            return false;
        }
        return true;
    }
    qDebug()<<"不是车 "<<m_arryChessBoard[x][y];
    return false;
}

bool ChineseChessLogic::verifyMa(int x, int y, int toX, int toY)
{
    if (('M' == m_arryChessBoard[x][y] ||'m' == m_arryChessBoard[x][y] )&& x != toX && y != toY)
    {
        //马的走法  必须不在同一行  且 不在同一列
        if(toX - x == 2 )
        {//竖着走 校验马脚
            if(0 < m_arryChessBoard[x+1][y])
            {
                qDebug()<<"挡住马脚了";
                return false;
            }
            return y == toY-1 || y == toY+1;
        }
        else if(toX - x == -2)
        {
            if(0 < m_arryChessBoard[x-1][y])
            {
                qDebug()<<"挡住马脚了";
                return false;
            }
            return y == toY-1 || y == toY+1;
        }else if(toY - y == 2)
        {//横着走
            if(0 < m_arryChessBoard[x][y+1])
            {
                qDebug()<<"挡住马脚了";
                return false;
            }
            return x == toX-1 || x == toX+1;
        }else if(toY - y == -2)
        {
            if(0 < m_arryChessBoard[x][y-1])
            {
                qDebug()<<"挡住马脚了";
                return false;
            }
            return x == toX-1 || x == toX+1;
        }

        qDebug()<<"正在校验";
    }
    return false;
}

bool ChineseChessLogic::verifyXiang(int x, int y, int toX, int toY)
{
    if (('X' == m_arryChessBoard[x][y] || 'x' == m_arryChessBoard[x][y]) && x != toX && y != toY)
    {//象的走法  不能过河 走田字

        //不能挡住象脚
        if(0 < m_arryChessBoard[(x+toX)/2][(y+toY)/2])
        {
            qDebug()<<"挡住像脚了";
            return false;
        }

        unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方的象.
        if(0x01 == whichSide)
        {//红
            if(toX<5)
            {
                qDebug()<<"不能过河";
                return false;
            }


            return (x == toX+2 || x == toX-2) &&  (y == toY+2 || y == toY-2);

        }else if(0x02 == whichSide)
        {//黑
            if(4<toX)
            {
                qDebug()<<"不能过河";
                return false;
            }
            return (x == toX+2 || x == toX-2) &&  (y == toY+2 || y == toY-2);
        }
    }
    return false;
}

bool ChineseChessLogic::verifyShi(int x, int y, int toX, int toY)
{
    if (('S' == m_arryChessBoard[x][y] || 's' == m_arryChessBoard[x][y]) && x != toX && y != toY)
    {//仕的走法  必须不在同一行且不在同一列 只能是斜着走
        unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方的卫仕.
        if(0x01 == whichSide)
        {//红  移动范围 7 8 9行  3 4 5列
            if(7<=toX && toX<10 && 3<=toY && toY<6)
            {
                return (x == toX+1 || x == toX-1) &&  (y == toY+1 || y == toY-1);
            }

        }else if(0x02 == whichSide)
        {//黑  移动范围 0 1 2行  3 4 5列
            if(0<=toX && toX<3 && 3<=toY && toY<6)
            {
                return (x == toX+1 || x == toX-1) &&  (y == toY+1 || y == toY-1);
            }
        }
    }
    return false;
}

bool ChineseChessLogic::verifyJiang(int x, int y, int toX, int toY)
{
    if ('J' == m_arryChessBoard[x][y] || 'j' == m_arryChessBoard[x][y])
    {
        unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方的将军.
        if(0x01 == whichSide)
        {//红  移动范围 7 8 9行  3 4 5列
            if(7<=toX && toX<10 && 3<=toY && toY<6)
            {
                return (x == toX && (y == toY+1||y==toY-1)) || (y == toY &&(x == toX+1||x==toX-1));
            }

        }else if(0x02 == whichSide)
        {//黑  移动范围 0 1 2行  3 4 5列
            if(0<=toX && toX<3 && 3<=toY && toY<6)
            {
                return (x == toX && (y == toY+1||y==toY-1)) || (y == toY &&(x == toX+1||x==toX-1));
            }
        }
    }
    return false;
}

bool ChineseChessLogic::verifyPao(int x, int y, int toX, int toY)
{
    if ('P' == m_arryChessBoard[x][y] || 'p' == m_arryChessBoard[x][y])
    {
        //跑的走法  必须同一行  或者 同一列
        if(x == toX)
        {//若同一行 则校验 直到toY列中是否包含其他棋子
            int start = y;
            int end = toY;
            if(toY < y){
                start = toY;
                end = y;
            }
            if(0 == m_arryChessBoard[toX][toY])
            {//如果目标地 是空,校验是否有障碍物
                for(int i=start+1; i<end; i++){
                    if(0 < m_arryChessBoard[x][i])
                    {
                        qDebug()<<"遇到障碍 "<<m_arryChessBoard[x][i];
                        return false;
                    }
                }
            }else
            {//如果是敌方棋子,校验是否前面有一个障碍
                int nCount = 0;
                for(int i=start+1; i<end; i++){
                    if(0 < m_arryChessBoard[x][i]) nCount++;
                }
                if (1 != nCount)  return false;
            }

        }else if(y == toY)
        {//若同一列 则校验 直到toX行中是否包含其他棋子
            int start = x;
            int end = toX;
            if(toX < x){
                start = toX;
                end = x;
            }

            if(0 == m_arryChessBoard[toX][toY])
            {//如果目标地 是空,校验是否有障碍物
                for(int i=start+1; i<end; i++){
                    if(0 < m_arryChessBoard[i][y])
                    {
                        qDebug()<<"遇到障碍 "<<m_arryChessBoard[i][y];
                        return false;
                    }
                }
            }else
            {//如果是敌方棋子,校验是否前面有一个障碍
                int nCount = 0;
                for(int i=start+1; i<end; i++){
                    if(0 < m_arryChessBoard[i][y]) nCount++;
                }
                if (1 != nCount)  return false;
            }
        }else{
            return false;
        }
        return true;
    }
    return false;
}

bool ChineseChessLogic::verifyBing(int x, int y, int toX, int toY)
{
    if ('B' == m_arryChessBoard[x][y] || 'b' == m_arryChessBoard[x][y])
    {
        // 兵 不能往回走,且只能一步步走. 过河之后 才能左右走.
        unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方的来兵.
        if(0x01 == whichSide)
        {//红方面军 任何时候向前一步都是可以 ->过河之后 同一行时,只能左右走
            return (x == toX+1 && y == toY) || (x < 5 && (toX == x && (y == toY+1 || y == toY-1)));
        }else if(0x02 == whichSide)
        {//黑方面军
            return (x == toX-1 && y == toY) || (4 < x && (toX == x && (y == toY+1 || y == toY-1)));
        }
    }
    return false;
}


unsigned char ChineseChessLogic::judgmetColor(int x, int y)
{
    if(0x00 == m_arryChessBoard[x][y]){//蓝
        return 0x00;
    }
    if(0x41 < m_arryChessBoard[x][y] && m_arryChessBoard[x][y] < 0x5A){//红
        return 0x01;
    }else{//黑
        return 0x02;
    }
}

