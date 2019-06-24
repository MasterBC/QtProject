#include "uichips.h"

UIChips::UIChips(QWidget *parent) : QLabel(parent)
{
    // 允许拖拽
    setAcceptDrops(false);
}
