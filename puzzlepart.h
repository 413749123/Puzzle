#ifndef PUZZLEPART_H
#define PUZZLEPART_H

#include <QWidget>

class PuzzlePart : public QWidget
{
    Q_OBJECT
public:
    explicit PuzzlePart(QWidget *parent = 0);

    void drawPart(QPainter &p,QImage *img);//画一块，传入painter和图

    QRect rect;//矩形区域，表示绘制img上rect大的图像

    unsigned int partId;//每一块的id

    bool isBlank();//没用到的函数

    PuzzlePart& operator =(const PuzzlePart&);//方便交换两个格子

    int row,col;//当前格子所在的行列值

signals:

public slots:
};

#endif // PUZZLEPART_H
