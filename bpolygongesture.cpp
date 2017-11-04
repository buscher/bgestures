
#include "bpolygongesture.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGestureEvent>
#include <QWidget>
#include <QVariant>
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <QRegion>
#include <QImage>

Qt::GestureType BPolygonGesture::Type;
int BPolygonGestureRecognizer::NextID = 1;

static const char polygonRegonizerID[] = "polygonRegonizerID";


BPolygonGesture::BPolygonGesture()
    : QGesture(),
      m_completionStatus(0)
{
}

int BPolygonGesture::completionStatus() const
{
    return m_completionStatus;
}

void BPolygonGesture::setCompletionStatus(int value)
{
    m_completionStatus = value;
}

QPointF BPolygonGesture::position() const
{
    return m_position;
}

void BPolygonGesture::setPosition(const QPointF &value)
{
    m_position = value;
}

void BPolygonGestureRecognizer::registerRecognizer()
{
//     QGestureRecognizer::registerRecognizer(new BPolygonGestureRecognizer(false));
    BPolygonGesture::Type = QGestureRecognizer::registerRecognizer(new BPolygonGestureRecognizer(0.15));
}

QPainterPath BPolygonGestureRecognizer::structure()
{
    QPainterPath path;
    path.moveTo(0,0);
    path.lineTo(1.0, 0);
    return path;
}

bool BPolygonGestureRecognizer::pointCheck(const QVector< QPoint >& list)
{
    return list.size() == 2 && list.first().x() < list.last().x();
}

BPolygonGestureRecognizer::BPolygonGestureRecognizer(qreal threshold, int minWidth, int minHeight)
    : m_threshold(threshold),
      m_minWidth(minWidth),
      m_minHeight(minHeight)
{
}

QGesture *BPolygonGestureRecognizer::create(QObject *)
{
    return new BPolygonGesture;
}

QGestureRecognizer::Result BPolygonGestureRecognizer::recognize(QGesture *state, QObject *watched,
                                                                  QEvent *event)
{
//     QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(event);
    QMouseEvent *ev = static_cast<QMouseEvent *>(event);
    switch(event->type()) {
        case QEvent::MouseButtonPress: {
//         case QEvent::GraphicsSceneMousePress:
//             qDebug() << "QEvent::GraphicsSceneMousePress";
            state->setProperty("completionStatus", 0);
            state->setProperty("position", ev->pos());
            Q_ASSERT(state->property("timerid").toInt() == 0);
            state->setProperty("timerid", state->startTimer(100));
//-------
            int newID = BPolygonGestureRecognizer::NextID++;
            QVector<QPoint> *newList = new QVector<QPoint>();
            state->setProperty(polygonRegonizerID, newID);
            m_pointListMap.insert(newID, newList);
//-------
            QWidget* tWidget = static_cast<QWidget*>(watched);
            QPoint tPoint = ev->pos();
            tPoint = tWidget->mapToGlobal(tPoint);
//             qDebug() << tPoint;
            state->setHotSpot(tPoint);

//------------

            event->accept();
            return QGestureRecognizer::MayBeGesture;
        }
//   case QEvent::GraphicsSceneMouseMove:
    case QEvent::MouseMove:
        if (int timerid = state->property("timerid").toInt()) {
            int id = state->property(polygonRegonizerID).toInt();
            QMap<int, QVector<QPoint>* >::iterator ite = m_pointListMap.find(id);
            if (ite != m_pointListMap.end())
            {
                QVector<QPoint> *list = ite.value();
                list->append(ev->pos());
//             QPoint delta = ev->pos() - state->property("position").toPointF().toPoint();
//             qDebug() << delta << "MouseMove";
//-------
                state->setProperty("position", ev->pos());
            
            
//-------
                return QGestureRecognizer::MayBeGesture;
            }
        }
        return QGestureRecognizer::Ignore;
//     case QEvent::GraphicsSceneMouseRelease:
    case QEvent::MouseButtonRelease:
        if (int timerid = state->property("timerid").toInt()) {
            int id = state->property(polygonRegonizerID).toInt();
            QMap<int, QVector<QPoint>* >::iterator ite = m_pointListMap.find(id);
            if (ite != m_pointListMap.end())
            {
                QVector<QPoint> *list = ite.value();
                list->append(ev->pos());
//-------
                //do the actual COMPARE
                qDebug() << "size:" << list->size();
                QPoint firstPoint = list->first();
                int minX = firstPoint.x(), minY = firstPoint.y();
                int maxX = minX, maxY = minY;
                foreach(const QPoint& point, *list)
                {
                    if (point.x() < minX)
                        minX = point.x();
                    else if (point.x() > maxX)
                        maxX = point.x();
                    
                    if (point.y() < minY)
                        minY = point.y();
                    else if (point.y() > maxY)
                        maxY = point.y();
                    
                    qDebug() << point;
                    
                }
                qDebug() << minX << minY << maxX << maxY;
                
                
                int width = maxX - minX + 1;
                int height= maxY - minY + 1;
                qDebug() << width << height;

                if (m_minWidth != -1 && width < m_minWidth)
                    return QGestureRecognizer::CancelGesture;
                if (m_minHeight != -1 && height < m_minHeight)
                    return QGestureRecognizer::CancelGesture;
                    
                
                
                QVector<QPoint> newList;
                foreach(const QPoint& point, *list)
                {
                    newList.append(QPoint(point.x()-minX + width, point.y()-minY + height));
                }
//-------
                const qreal magicValue = m_threshold;
//-------
                QPainterPath path = structure();
                QBitmap bm(width*3, height*3);
                bm.fill(Qt::white);
                QPainter paint(&bm);
                

                
                paint.translate(width, height);
                paint.scale(width, height);
//                 paint.setPen(QPen(Qt::black, 1.0/height * 15));
                paint.setPen(QPen(Qt::black, 1.0/height * width*magicValue, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                
                paint.drawPath(path);
                paint.end();
                
                bm.save("/home/buscher/test-output.bmp");
                
                QRegion region(bm);
                
                QPixmap copy(QPixmap::fromImage(bm.toImage()));
                QPainter paint2(&copy);
                paint2.setPen(Qt::red);
                
                
                bool isGesture = true;
                foreach(const QPoint& point, newList)
                {
                    if (!region.contains(point))
                    {
                        qDebug() << "NOT CONTAINING:" << point;
                        isGesture = false;
                        paint2.drawPoint(point);
                    }
                }
                paint2.end();
                copy.save("/home/buscher/test-output-miss.bmp");
                
#if 1
                {
                    QPixmap copylineonly(QPixmap::fromImage(bm.toImage()));
                    QPainter paint2(&copylineonly);
                    paint2.setPen(Qt::red);
                    
                    
                    bool isGesture = true;
                    foreach(const QPoint& point, newList)
                    {
                        paint2.drawPoint(point);
                    }
                    paint2.end();
                    copylineonly.save("/home/buscher/test-output-line.bmp");
                }
                
#endif 
                
                
                
//                 return QGestureRecognizer::CancelGesture;

                
                if (isGesture) {
                    
                    qDebug() << "COUNT" << path.elementCount();
                    QVector<QPoint> checkList;
#if 0
                    int minElement = 0;
                    for (int i = 0; i < path.elementCount(); ++i)
                    {
//                         qDebug() << i;
                        QPainterPath::Element ele = path.elementAt(i);
                        QPoint goal(ele.x*width, ele.y*height + height*2);
//                         int mlength = goal.manhattanLength();
                        
                        int minLength = width+height;
                        
                        for (int i = minElement; i < newList.size(); ++i)
                        {
                            QPoint point = newList.at(i);
                            QPoint tmp = point - goal;
                            if (tmp.manhattanLength() < minLength)
                            {
                                minLength = tmp.manhattanLength();
                                minElement = i;
                            } else {
                                break;
                            }
                        }
                        if (minElement < newList.size())
                        {
                            qDebug() << minElement;
                            checkList.append(newList.at(minElement));
                            minElement++;
                        }
                    }
#endif

#if 1

                    if (path.elementCount() == 2)
                    {
                        checkList.append(list->first());
                        checkList.append(list->last());
                    }
                    else
                    {
                        int minElement = 0;
                        int mid = static_cast<int>(path.elementCount()/2.0+0.5);
                        qDebug() << mid;
                        for (int i = 0; i < mid; ++i)
                        {
    //                         qDebug() << i;
                            QPainterPath::Element ele = path.elementAt(i);
                            QPoint goal(ele.x*width + width, ele.y*height + height);
                            qDebug() << "new goal" << goal;
    //                         int mlength = goal.manhattanLength();
                            
                            int minLength = width+height;
                            int maxItem = newList.size() / 3.0 * 2.0;
                            
                            for (int j = minElement; j < maxItem; ++j)
                            {
                                QPoint point = newList.at(j);
                                QPoint tmp = point - goal;
                                if (tmp.manhattanLength() < minLength)
                                {
                                    minLength = tmp.manhattanLength();
                                    minElement = j;
                                }
                            }
                            if (minElement < newList.size())
                            {
                                qDebug() << "ADD:" << minElement << i;
                                checkList.append(newList.at(minElement));
                                minElement++;
                            }
                        }
                        
                        
                        qDebug() << "PART2";
                        
                        for (int i = path.elementCount()-1; i >= mid; --i)
                        {
    //                         qDebug() << i;
                            QPainterPath::Element ele = path.elementAt(i);
                            QPoint goal(ele.x*width + width, ele.y*height + height);
                            qDebug() << "new goal" << goal;
    //                         int mlength = goal.manhattanLength();
                            
                            int minLength = width+height;
                            int minItem = newList.size() / 3.0;
                            
                            for (int j = newList.size()-1; j >= minItem ; --j)
                            {
                                QPoint point = newList.at(j);
                                QPoint tmp = point - goal;
                                if (tmp.manhattanLength() < minLength)
                                {
                                    minLength = tmp.manhattanLength();
                                    minElement = j;
                                }
                            }
                            if (minElement >= 0)
                            {
                                QPoint def(-1,-1);
                                while (checkList.size() <= i)
                                    checkList.append(def);
                                
                                qDebug() << "ADD:" << minElement << "AT:" << i;
                                checkList[i] = newList.at(minElement);
                                minElement--;
                            }
                        }
                    }
                    
                    
#endif
                    
                    
                    
                    isGesture &= pointCheck(checkList);
                    
                    
                    QPixmap copy2(copy.copy());
                    QPainter paint3(&copy2);
                    paint3.setPen(Qt::green);
                    foreach(const QPoint& point, checkList)
                    {
                        qDebug() << point;
                        paint3.drawPoint(point);
                    }
                    paint3.end();
                    copy2.save("/home/buscher/test-output-miss-checks.bmp");
                }
                
                delete list;
                reset(state);
                
                m_pointListMap.remove(id);
                
                return isGesture ? QGestureRecognizer::TriggerGesture :
                                   QGestureRecognizer::CancelGesture;
            }
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

void BPolygonGestureRecognizer::reset(QGesture *q)
{
//     qDebug() << "nope, not this gesture";
    q->setProperty("completionStatus", 0);
    q->setProperty("position", QPointF());
    if (int timerid = q->property("timerid").toInt())
        q->killTimer(timerid);
    q->setProperty("timerid", 0);
    q->setProperty("iteration", 0);
    q->setProperty(polygonRegonizerID, -1);
}

#include "bpolygongesture.moc"
