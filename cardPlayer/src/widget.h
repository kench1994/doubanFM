#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPoint>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    //mouse event
    QPoint qp_WindowPos;
    QPoint qp_MousePos;
    bool b_MousePressed;
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *lpEvent);
    void mouseReleaseEvent(QMouseEvent *lpEvent);
};

#endif // WIDGET_H
