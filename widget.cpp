#include "widget.h"
#include <QPainter>
#include <cmath>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mm = ss = 0;
    isStart = true;
    totalRadius = 600;
    width = 3;
    height = 3;
    stepCount = 0;
    radius = totalRadius/width;
    win = false;

    map = new PuzzlePart*[height];

    for(int i = 0;i<height;i++){
        map[i] = new PuzzlePart[width];
        for(int j = 0;j<width;j++){
            map[i][j].rect = QRect(radius*j,radius*i,radius,radius);
            map[i][j].partId = i*width+j;
            map[i][j].row = i;
            map[i][j].col = j;
        }
    }

    /*这个函数用来打乱顺序*/
    orderBroken();
    update();

    this->setFixedSize(1000,600);

    QString fileName = tr(":/resImage/image/2.jpg");

    img = new QImage;
    img->load(fileName);
    *img = img->scaled(600,600,
                       Qt::IgnoreAspectRatio);

    btn  = new QPushButton(this);
    btn->setText(tr("选择图片"));
    btn->setFont(QFont(tr("宋体"),12));
    btn->setGeometry(totalRadius+80,20,120,40);
    connect(btn,SIGNAL(clicked(bool)),/*关联函数*/
            this,SLOT(onBtnClicked()));

    startBtn  = new QPushButton(this);
    startBtn->setText(tr("开始"));
    startBtn->setFont(QFont(tr("宋体"),12));
    startBtn->setGeometry(totalRadius+230,20,120,40);

    diffLevel = new QSpinBox(this);
    diffLevel->setGeometry(totalRadius+160,80,120,30);
    diffLevel->setMinimum(3);
    diffLevel->setMaximum(10);
    diffLevel->setFont(QFont(tr("宋体"),12));
    diffLevel->setAlignment(Qt::AlignHCenter);

    connect(diffLevel,SIGNAL(valueChanged(int)),
            this,SLOT(onSpinBoxValueChanged(int)));

    curStepCount = new QLineEdit(this);
    curStepCount->setGeometry(totalRadius+160,130,120,40);
    curStepCount->setText(tr("0步"));
    curStepCount->setEnabled(false);
    curStepCount->setAlignment(Qt::AlignHCenter);
    curStepCount->setFont(QFont(tr("宋体"),12));


    QLabel *curStepCountText = new QLabel(this);
    curStepCountText->setGeometry(totalRadius+50,145,100,30);
    curStepCountText->setText(tr("步数:"));
    curStepCountText->setAlignment(Qt::AlignHCenter);
    curStepCountText->setFont(QFont(tr("宋体"),12));


    QLabel *diffLevelText = new QLabel(this);
    diffLevelText->setGeometry(totalRadius+50,85,100,40);
    diffLevelText->setText(tr("难度:"));
    diffLevelText->setAlignment(Qt::AlignHCenter);
    diffLevelText->setFont(QFont(tr("宋体"),12));

    QLabel *timeText = new QLabel(this);
    timeText->setGeometry(totalRadius+50,200,100,30);
    timeText->setText(tr("时间:"));
    timeText->setAlignment(Qt::AlignHCenter);
    timeText->setFont(QFont(tr("宋体"),12));

    timeLineEdit = new QLineEdit(this);
    timeLineEdit->setGeometry(totalRadius+160,190,120,40);
    timeLineEdit->setText(tr("00:00"));
    timeLineEdit->setEnabled(false);
    timeLineEdit->setAlignment(Qt::AlignHCenter);
    timeLineEdit->setFont(QFont(tr("宋体"),12));


    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),
            this,SLOT(onTimeOutSlot()));

    connect(startBtn,SIGNAL(clicked(bool)),
            this,SLOT(onStartBtnClicked()));
    this->setFocus();
}

Widget::~Widget()
{
    //release resource of space.
    for(int i = 0;i<height;i++){
        delete[] map[i];
    }
    delete[] map;

    delete img;
    delete btn;
    delete diffLevel;
    delete curStepCount;
    delete timeLineEdit;
    delete timer;
    delete startBtn;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setBrush(Qt::white);

    p.setPen(QPen(Qt::white,5, Qt::SolidLine));
    p.setFont(QFont("宋体", radius*0.6));

    for(int i = 0;i<height;i++){
        for(int j = 0;j<width;j++){
           map[i][j].drawPart(p,img);
        }
    }

    p.drawImage(totalRadius+50,totalRadius-350,
                img->scaled(300,300,Qt::IgnoreAspectRatio),
                0,0,300,300);

    if(checkGameOver()&&!win){
        win = true;
        QMessageBox::warning(this,tr("Congratulations!"),tr("You Win!"),QMessageBox::Retry);
    }
}

void Widget::onBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("选择图片"),"",tr("Images(*.png *.bmp *.jpg *.gif )"));
    if(!fileName.isEmpty()){
        QImage *imgTemp;
        imgTemp = new QImage;
        if(imgTemp->load(fileName)){
            *imgTemp = imgTemp->scaled(width*radius,height*radius,
                           Qt::IgnoreAspectRatio);
            if(img)
                delete img;
            img = imgTemp;
            onSpinBoxValueChanged(width);
            update();
        }else{
            delete imgTemp;
            return;
        }

    }

}

void Widget::onSpinBoxValueChanged(int value)
{
    stepCount = 0;
    curStepCount->setText(tr("%1步").arg(stepCount));
    startBtn->setText(tr("开始"));
    if(isStart == false){
        timer->stop();
        timeLineEdit->setText(tr("00:00"));
        isStart = true;
    }
    //释放资源
    for(int i = 0;i<height;i++){
        delete[] map[i];
    }
    delete[] map;
    mm = ss = 0;

    width = height = value;
    radius = totalRadius/width;

    map = new PuzzlePart*[height];

    for(int i = 0;i<height;i++){
        map[i] = new PuzzlePart[width];
        for(int j = 0;j<width;j++){
            map[i][j].rect = QRect(radius*j,radius*i,radius,radius);
            map[i][j].partId = i*width+j;
            map[i][j].row = i;
            map[i][j].col = j;
        }
    }
    orderBroken();
    update();
}

void Widget::onTimeOutSlot()
{
    ss++;
    if(ss >= 60){
        ss = 0;
        mm++;
    }
    QString str;
    str = str.sprintf("%02d:%02d",mm,ss);
    //QMessageBox::warning(this,tr("Error!"),str,QMessageBox::Retry);
    timeLineEdit->setText(str);
}

void Widget::onStartBtnClicked()
{
    if(isStart){
        timer->start(1000);
        startBtn->setText(tr("暂停"));
    }
    else{
        timer->stop();
        startBtn->setText(tr("继续"));
    }
    isStart = isStart != true;

}

void Widget::orderBroken()
{
    //循环width*height次，随机交换两个块使得其无序
    int x1,y1,x2,y2;
    for(int i = 0;i<width*height;i++){
        x1 = rand()%width;
        x2 = rand()%width;
        y1 = rand()%height;
        y2 = rand()%height;

        PuzzlePart temp;
        temp = map[y1][x1];
        map[y1][x1] = map[y2][x2];
        map[y2][x2] = temp;
    }
}

void Widget::click(QPoint pt)
{
    if(radius == 0)
        return;
    int y = pt.y()/radius;
    int x = pt.x()/radius;
    if(!checkRange(y,x))
        return;
    //左 右 下 上
    int cells[4][2] = {0,-1,0,1,1,0,-1,0};

    for(int i = 0;i<4;i++){
        int ry = y+cells[i][0];
        int rx = x+cells[i][1];
        if(checkRange(ry,rx) && map[ry][rx].partId == 0){
            PuzzlePart temp;
            temp = map[y][x];
            map[y][x] = map[ry][rx];
            map[ry][rx] = temp;
        }
    }
    stepCount++;
    curStepCount->setText(tr("%1步").arg(stepCount));
    update();
}

void Widget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() != Qt::LeftButton||isStart == true) { // 排除鼠标右键点击
        return;
    }
    click(ev->pos());
}

bool Widget::checkRange(int y, int x)
{
    return y>=0&&y<height&&x>=0&&x<width;
}

bool Widget::checkGameOver()
{
    int counter = 0;
    int last = map[0][0].partId;

    for(int i = 0;i<height;i++){
        for(int j = 0;j<width;j++){
            if(map[i][j].partId >= last)
                counter++;
            last = map[i][j].partId;
            if(counter == width*height-1)
                return true;
        }
    }
    return false;
}
