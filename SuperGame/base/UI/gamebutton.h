#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <QPushButton>

class GameButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)


public:
    explicit GameButton(const QString& name,QWidget *parent = nullptr);

signals:

public slots:

private:
    inline int alpha() const{ return m_nAlpha; }
    void setAlpha(const int alpha);


private:
    int m_nAlpha;
};

#endif // GAMEBUTTON_H
