#ifndef BACCARATLOGIC_H
#define BACCARATLOGIC_H

#include <string>
//下注区域
#define AREA_XIAN        0 //闲家
#define AREA_PING        1 //平家
#define AREA_ZHUANG      2 //庄家
#define AREA_XIAN_TIAN   3 //闲天王
#define AREA_ZHUANG_TIAN 4 //庄天王
#define AREA_TONG_DUI    5 //同点平
#define AREA_XIAN_DUI    6 //闲对子
#define AREA_ZHUANG_DUI  7 //庄对子


#define LOGIC_MASK_COLOR 0xF0 //花色掩码
#define  LOGIC_MASK_VALUE 0x0F //数值掩码
class baccaratLogic
{
public:
    baccaratLogic();
    // 获取扑克的数值
    unsigned char GetCardValue(unsigned char cbCardData);
    // 获取扑克的花色
    unsigned char GetCardColor(unsigned char cbCardData);
    // 获取扑克的文字
    std::string GetCardText(unsigned char cbCardData);

    // 获取区域
    std::string GetAreaText(unsigned char cbCardData);
    unsigned char GetArea(const std::string& area);

    // 比牌大小 引用获取两个数值大小
    void CompareCard(const char* cbFirstData, const char* cbSecondData,int& first, int& second);

};

#endif // BACCARATGAMELOGIC_H
