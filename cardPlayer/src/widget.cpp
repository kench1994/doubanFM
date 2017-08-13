#include "widget.h"
#include "ui_widget.h"
#include <QMouseEvent>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255,0));
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/bg--.png")));
    this->setPalette(palette);
    setWindowFlags(Qt::FramelessWindowHint);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::mouseMoveEvent(QMouseEvent *lpEvent){
    if (b_MousePressed){
        this->move(qp_WindowPos + (lpEvent->globalPos() - qp_MousePos));
    }
}


void Widget::mousePressEvent(QMouseEvent *lpEvent){
    if (lpEvent->button() == Qt::LeftButton)    {
        qp_WindowPos = this->pos();
        qp_MousePos = lpEvent->globalPos();
        this->b_MousePressed = true;
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *lpEvent){
    if (lpEvent->button() == Qt::LeftButton)    {
        this->b_MousePressed = false;
    }
}
