#ifndef UIBETAREA_H
#define UIBETAREA_H

#include <QLabel>
#include "base/common.h"
class UIBetArea : public QLabel
{
    Q_OBJECT
public:
    explicit UIBetArea(QWidget *parent = nullptr);
    void allowClick(bool isAllow);
    void setChipType(NSType::enChip chipType);
    bool generateChip(NSType::enChip chipType,const QRect& rect);
    void clearALL();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:

public slots:

private:
    NSType::enChip m_curChipType;
    bool m_isAllow;
};

#endif // UIBETAREA_H
