#include "puzzlepart.h"
#include <cmath>
#include <QMessageBox>
#include <QPainter>
#include <QRect>

PuzzlePart::PuzzlePart(QWidget *parent) : QWidget(parent)
{
    partId = 0;
}

void PuzzlePart::drawPart(QPainter &p,QImage *img)
{
    p.setBrush(Qt::white);
    p.drawRect(col*rect.width(),row*rect.height(),rect.width(),rect.height());
    if(partId != 0){
        p.drawImage(col*rect.width(),row*rect.height(),*img,rect.x(),rect.y(),rect.width(),rect.height());
    }else{

    }
}

bool PuzzlePart::isBlank()
{
    return partId == 0;
}

PuzzlePart &PuzzlePart::operator =(const PuzzlePart &pp)
{
    partId = pp.partId;
    rect = pp.rect;
    return *this;
}
