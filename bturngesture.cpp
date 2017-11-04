
#include "bturngesture.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGestureEvent>
#include <QWidget>
#include <QVariant>
#include <QDebug>

Qt::GestureType BTurnGesture::Type;

BTurnGesture::BTurnGesture()
    : QGesture(),
      m_completionStatus(0)
{
}

int BTurnGesture::completionStatus() const
{
    return m_completionStatus;
}

void BTurnGesture::setCompletionStatus(int value)
{
    m_completionStatus = value;
}

QPointF BTurnGesture::position() const
{
    return m_position;
}

void BTurnGesture::setPosition(const QPointF &value)
{
    m_position = value;
}

void BTurnGestureRecognizer::registerRecognizer()
{
//     QGestureRecognizer::registerRecognizer(new BTurnGestureRecognizer(false));
    BTurnGesture::Type = QGestureRecognizer::registerRecognizer(new BTurnGestureRecognizer(true));
}

BTurnGestureRecognizer::BTurnGestureRecognizer(bool feedback)
    : m_feedback(feedback)
{
}

QGesture *BTurnGestureRecognizer::create(QObject *)
{
    if (m_feedback)
        return new BTurnGesture;
    return new QTapAndHoldGesture;
}

QGestureRecognizer::Result BTurnGestureRecognizer::recognize(QGesture *state, QObject *watched,
                                                                  QEvent *event)
{
//     QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(event);
    QMouseEvent *ev = static_cast<QMouseEvent *>(event);
    switch(event->type()) {
        case QEvent::MouseButtonPress: {
//         case QEvent::GraphicsSceneMousePress:
//             qDebug() << "QEvent::GraphicsSceneMousePress";
            if (m_feedback)
                state->setProperty("completionStatus", 0);
            state->setProperty("position", ev->pos());
            Q_ASSERT(state->property("timerid").toInt() == 0);
            state->setProperty("timerid", state->startTimer(100));
    //-------
            state->setProperty("turnState", BTurnGestureRecognizer::Start);
    //-------
            
            
            QWidget* tWidget = static_cast<QWidget*>(watched);
            
            QPoint tPoint = ev->pos();
            tPoint = tWidget->mapToGlobal(tPoint);
//             qDebug() << tPoint;
            
            
//------------
            state->setHotSpot(tPoint);
            event->accept();
//             qDebug() << "QEvent::MouseButtonPress";
            return QGestureRecognizer::MayBeGesture;
//             return m_feedback ?
//                     QGestureRecognizer::TriggerGesture | QGestureRecognizer::ConsumeEventHint :
//                     QGestureRecognizer::MayBeGesture | QGestureRecognizer::ConsumeEventHint;
//     case QEvent::GraphicsSceneMouseMove:
        }
    case QEvent::MouseMove:
        if (int timerid = state->property("timerid").toInt()) {
            QPoint delta = ev->pos() - state->property("position").toPointF().toPoint();
//             qDebug() << delta << "MouseMove";
//-------
            BTurnGestureRecognizer::TurnState turnState = static_cast<BTurnGestureRecognizer::TurnState>(state->property("turnState").toInt());
            state->setProperty("position", ev->pos());
//-------
            switch (turnState) {
                case BTurnGestureRecognizer::Start:
//                     qDebug() << "start";
                    if (delta.x() >= 0 && delta.y() <= 0)
                    {
                        state->setProperty("turnState", BTurnGestureRecognizer::RightUp);
                        return QGestureRecognizer::MayBeGesture;
                    }
                    reset(state);
                    return QGestureRecognizer::CancelGesture;
                case BTurnGestureRecognizer::RightUp:
//                     qDebug() << "rightup";
                    if (delta.x() >= 0 && delta.y() <= 0)
                    {
                        return QGestureRecognizer::MayBeGesture;
                    }
                    else if (delta.x() >= 0 && delta.y() >= 0)
                    {
                        state->setProperty("turnState", BTurnGestureRecognizer::RightDown);
                        return QGestureRecognizer::MayBeGesture;
                    }
                    reset(state);
                    return QGestureRecognizer::CancelGesture;
                case BTurnGestureRecognizer::RightDown:
//                     qDebug() << "rightdown";
                    if (delta.x() >= 0 && delta.y() >= 0)
                    {
                        state->setProperty("turnState", BTurnGestureRecognizer::Done);
                        return QGestureRecognizer::MayBeGesture;
                    }
                    reset(state);
                    return QGestureRecognizer::CancelGesture;
                case BTurnGestureRecognizer::Done:
//                     qDebug() << "done";
                    if (delta.x() >= 0 && delta.y() >= 0)
                    {
                        return QGestureRecognizer::MayBeGesture;
                    }
                    reset(state);
                    return QGestureRecognizer::CancelGesture;
            }
            
            
//             QPoint delta = ev->screenPos() - state->property("position").toPointF().toPoint();
//             enum { TapRadius = 40 };
//             if (delta.manhattanLength() > TapRadius) {
//                 state->killTimer(timerid);
// #if 1
//                 reset(state);
// #endif
//                 return QGestureRecognizer::CancelGesture;
//             }
        }
        return QGestureRecognizer::Ignore;
//     case QEvent::GraphicsSceneMouseRelease:
    case QEvent::MouseButtonRelease:
        if (int timerid = state->property("timerid").toInt()) {
            BTurnGestureRecognizer::TurnState turnState = static_cast<BTurnGestureRecognizer::TurnState>(state->property("turnState").toInt());
            reset(state);
            if (turnState == BTurnGestureRecognizer::Done)
            {
                return QGestureRecognizer::TriggerGesture;
            }
            return QGestureRecognizer::CancelGesture;
        }
        return QGestureRecognizer::Ignore;
    case QEvent::Timer: {
//         qDebug() << "CancelGesture";
        return QGestureRecognizer::Ignore;
//         Q_ASSERT(state == watched);
//         Q_ASSERT(state->property("timerid").toInt() != 0);
//         int iteration = state->property("iteration").toInt();
//         if (m_feedback)
//             state->setProperty("completionStatus", iteration+1);
//         if (iteration++ == 9) {
//             state->killTimer(state->property("timerid").toInt());
//             return QGestureRecognizer::FinishGesture;
//         }
//         state->setProperty("iteration", iteration);
//         return m_feedback ?
//                 QGestureRecognizer::TriggerGesture|QGestureRecognizer::ConsumeEventHint :
//                 QGestureRecognizer::Ignore|QGestureRecognizer::ConsumeEventHint;
        }
    default:
        break;
    }
    return QGestureRecognizer::Ignore;
}

void BTurnGestureRecognizer::reset(QGesture *q)
{
//     qDebug() << "nope, not this gesture";
    q->setProperty("completionStatus", 0);
    q->setProperty("position", QPointF());
    if (int timerid = q->property("timerid").toInt())
        q->killTimer(timerid);
    q->setProperty("timerid", 0);
    q->setProperty("iteration", 0);
}

#include "bturngesture.moc"
