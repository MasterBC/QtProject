#ifndef UIMETERPROCESSBAR_H
#define UIMETERPROCESSBAR_H

#include <QWidget>
class QLabel;
class UiMeterProcessBar : public QWidget
{
    Q_OBJECT
public:
    explicit UiMeterProcessBar(QWidget *parent = nullptr);
    ~UiMeterProcessBar();
    void setMaxValue(int value);
    void setMinValue(int value);
    void setDuration(int timeout);//设置时长-秒
    //void setSize(int width, int height);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void slotUpdateTimer();

private:
    QPoint beginDrag;
    bool bPressFlag;

    int maxValue;
    int currentValue;
    QLabel *startValueLabel;
    QLabel *endValueLabel;
    QLabel *dispayValueLabel;

    QTimer *updateTimer;
};

#endif // UIMETERPROCESSBAR_H
