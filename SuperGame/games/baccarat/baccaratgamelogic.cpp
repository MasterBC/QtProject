#include "baccaratgamelogic.h"


#include <string.h>


//获取数值
unsigned char baccaratGameLogic::GetCardValue(unsigned char cbCardData)  {
    return cbCardData & LOGIC_MASK_VALUE;
}

//获取花色
unsigned char baccaratGameLogic::GetCardColor(unsigned char cbCardData)  {
    return cbCardData & LOGIC_MASK_COLOR;
}

// 文字
std::string baccaratGameLogic::GetCardText(unsigned char cbCardData) {
    unsigned char color = GetCardColor(cbCardData);
    unsigned char value = GetCardValue(cbCardData);
    std::string strTxt = "";
    switch (color)
    {
    case 0x00:
        strTxt = "♦";
        break;
    case 0x10:
        strTxt = "♣";
        break;
    case 0x20:
        strTxt = "♥";
        break;
    case 0x30:
        strTxt = "♠";
        break;
    }

    switch (value )
    {
    case 0x00:
        return "";
    case 0x01:
        strTxt += "1";
        break;
    case 0x02:
        strTxt += "2";
        break;
    case 0x03:
        strTxt += "3";
        break;
    case 0x04:
        strTxt += "4";
        break;
    case 0x05:
        strTxt += "5";
        break;
    case 0x06:
        strTxt += "6";
        break;
    case 0x07:
        strTxt += "7";
        break;
    case 0x08:
        strTxt += "8";
        break;
    case 0x09:
        strTxt += "9";
        break;
    case 0x0A:
        strTxt += "10";
        break;
    case 0x0B:
        strTxt += "J";
        break;
    case 0x0C:
        strTxt += "Q";
        break;
    case 0x0D:
        strTxt += "K";
        break;
    case 0x0E:
        strTxt += "小王";
        break;
    case 0x0F:
        strTxt += "大王";
        break;
    }
    return strTxt;
}

std::string baccaratGameLogic::GetAreaText(unsigned char cbCardData)
{

    switch (cbCardData) {
    case AREA_XIAN:
        return "闲家";
    case AREA_PING:
        return "平家";
    case AREA_ZHUANG:
        return "庄家";
    case AREA_XIAN_TIAN:
        return "闲天王";
    case AREA_ZHUANG_TIAN:
        return "庄天王";
    case AREA_TONG_DUI:
        return "同点平";
    case AREA_XIAN_DUI:
        return "闲对子";
    case AREA_ZHUANG_DUI:
        return "庄对子";
    default:
        break;
    }
    return "";
}

void baccaratGameLogic::CompareCard(const char *cbFirstData, const char *cbSecondData, int &first, int &second)
{

    first = 0, second = 0;
    size_t firstLen = strlen(cbFirstData);
    size_t secondLen = strlen(cbSecondData);
    for(size_t i = 0; i < firstLen; i++)
    {
        if(GetCardValue(cbFirstData[i]) < 10)
        {
            first += GetCardValue(cbFirstData[i]);
        }

    }

    for(size_t i = 0; i < secondLen; i++)
    {
        if(GetCardValue(cbSecondData[i]) < 10)
        {
            second += GetCardValue(cbSecondData[i]);
        }

    }
    first = first%10;
    second = second%10;

}





baccaratGameLogic::baccaratGameLogic()
{

}
