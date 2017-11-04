
#include "bgesturewidget.h"

#include <QEvent>
#include <QGesture>
#include <QGestureEvent>
#include <QDebug>
#include <QPainter>

#include "bturngesture.h"
#include "bpolygongesture.h"
#include "bturngesturev2.h"
#include "bcutgesture.h"

BGestureWidget::BGestureWidget()
    : QWidget()
{
//     grabGesture(BTurnGesture::Type);
//     grabGesture(BPolygonGesture::Type);
//     grabGesture(BTurnGestureV2::Type);
    grabGesture(BCutGesture::Type);
    m_background = 0;
    m_mousePressed = false;
}

bool BGestureWidget::event(QEvent* e)
{
    if (e->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(e));
    return QWidget::event(e);
}

void BGestureWidget::resizeEvent(QResizeEvent* event)
{
    delete m_background;
    m_background = new QPixmap(event->size());
    m_background->fill(Qt::white);
    
    QWidget::resizeEvent(event);
}

void BGestureWidget::paintEvent(QPaintEvent* event)
{
    if (!m_background)
        return;
    QPainter paint(this);
    paint.drawPixmap(0,0, *m_background);
    paint.end();
}

void BGestureWidget::mousePressEvent(QMouseEvent* event)
{
    m_mousePressed = true;
    QWidget::mousePressEvent(event);
}

void BGestureWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_mousePressed = false;
    
    if (event->button() == Qt::RightButton && m_background) {
        m_background->fill(Qt::white);
    }
    
    

    QWidget::mouseReleaseEvent(event);
    update();
}

void BGestureWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_background)
    {
        QPainter paint(m_background);
        paint.setPen(Qt::black);
        paint.drawPoint(event->pos());
        paint.end();
    }
    QWidget::mouseMoveEvent(event);
}



bool BGestureWidget::gestureEvent(QGestureEvent* event)
{
    if (QGesture *turn = event->gesture(BTurnGesture::Type))
    {
        BTurnGesture* turnges = static_cast<BTurnGesture *>(turn);
        turnGesture(turnges);
//         qDebug() << "YAY its a turn!";
    }
    else if (QGesture *turn = event->gesture(BPolygonGesture::Type))
    {
        BPolygonGesture* polyges = static_cast<BPolygonGesture *>(turn);
        polygonGesture(polyges);
    }
    else if (QGesture *turn = event->gesture(BTurnGestureV2::Type))
    {
        BTurnGestureV2* turnges2 = static_cast<BTurnGestureV2 *>(turn);
        turnGestureV2(turnges2);
    }
    else if (QGesture *turn = event->gesture(BCutGesture::Type))
    {
        BCutGesture* cutges = static_cast<BCutGesture *>(turn);
        cutGesture(cutges);
    }
    
    
//      if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
//          swipeTriggered(static_cast<QSwipeGesture *>(swipe));
//      else if (QGesture *pan = event->gesture(Qt::PanGesture))
//          panTriggered(static_cast<QPanGesture *>(pan));
//      if (QGesture *pinch = event->gesture(Qt::PinchGesture))
//          pinchTriggered(static_cast<QPinchGesture *>(pinch));
     return true;
}

void BGestureWidget::turnGesture(BTurnGesture* gesture)
{
    qDebug() << "TURN!";
}

void BGestureWidget::polygonGesture(BPolygonGesture* gesture)
{
    qDebug() << "POLYGON!";
}

void BGestureWidget::turnGestureV2(BTurnGestureV2* gesture)
{
    qDebug() << "TURNV2!";
}

void BGestureWidget::cutGesture(BCutGesture* gesture)
{
    qDebug() << "CUT!";
}

