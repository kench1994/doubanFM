#ifndef MOUSEEVENT
#define MOUSEEVENT
#include <QOBJECT>
#include <QMouseEvent>

class mouse{

QPoint m_WindowPos;
QPoint m_MousePos;
bool m_MousePressed;

void mouseMoveEvent(QMouseEvent *);
void mousePressEvent(QMouseEvent *lpEvent);
void mouseReleaseEvent(QMouseEvent *lpEvent);

};


#endif // MOUSEEVENT


