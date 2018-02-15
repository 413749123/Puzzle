#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "puzzlepart.h"
#include <QMouseEvent>
#include <QVector>
#include <QTimer>

class QPushButton;
class QSpinBox;
class QLineEdit;
//待实现功能：
//退回上一步功能
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    virtual void paintEvent(QPaintEvent *);//绘制界面
public slots://槽函数
    void onBtnClicked();//当鼠标点击时
    void onSpinBoxValueChanged(int);//当spinbox里面的值改变时
    void onTimeOutSlot();//计时器，
    void onStartBtnClicked();//开始按钮的响应函数

private:
    int width;//宽
    int height;//高，格子数
    int radius;//正方形格子的边长
    int totalRadius;//固定大小为600*600，然后根据width算出radius
    int stepCount;//步数计数器
    int mm,ss;//时间的分和秒
    bool isStart;//判断是否是开始按钮，以在开始/暂停功能间转换
    //QImage *image;


    QImage *img;
    QPushButton *btn;
    QSpinBox *diffLevel;
    QLineEdit *curStepCount;
    QLineEdit *timeLineEdit;
    QTimer *timer;
    QPushButton *startBtn;

    bool win;

    PuzzlePart **map;

    void orderBroken();//将格子打乱顺序

    virtual void click(QPoint pt);//处理鼠标点击

    virtual void mouseReleaseEvent(QMouseEvent *ev); // 鼠标点击象棋并释放鼠标时候触发

    bool checkRange(int,int);//检查是否越界

    bool checkGameOver();//检查游戏是否结束
};

#endif // WIDGET_H
