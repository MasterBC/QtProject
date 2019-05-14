#include "gamebutton.h"

// Qt
#include <QRgb>
GameButton::GameButton(const QString &name, QWidget *parent) : QPushButton(parent)
{
    this->setText(name);
}

void GameButton::setAlpha(const int alpha)
{
    //    m_nAlpha = alpha;
    //    QString strQSS = QString("background-color: rgba(%1);").arg(alpha.name(QColor::HexRgb));

    m_nAlpha = alpha;
    QString strQSS = QString("color:#000000; background-color: rgba(34,139,34, %1);").arg(m_nAlpha);
    this->setStyleSheet(strQSS);

}
