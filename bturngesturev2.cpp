
#include "bturngesturev2.h"

#include <QDebug>

Qt::GestureType BTurnGestureV2::Type;

void BTurnGestureRecognizerV2::registerRecognizer()
{
    BTurnGestureV2::Type = QGestureRecognizer::registerRecognizer(new BTurnGestureRecognizerV2(0.20));
}

BTurnGestureRecognizerV2::BTurnGestureRecognizerV2(qreal thres)
    : BPolygonGestureRecognizer(thres),
      m_generatedPathCount(0)
{
}

bool BTurnGestureRecognizerV2::pointCheck(const QVector< QPoint >& list)
{
    qDebug() << "final check" << list.size();
    qDebug() << list.first().x()  << list.last().x();
    int mid = list.size()/2;
    return list.size() == m_generatedPathCount &&
           list.first().x() < list.last().x() &&
           list.at(mid).y() < list.first().y() &&
           list.at(mid).y() < list.last().y();
}

QPainterPath BTurnGestureRecognizerV2::structure()
{
    QPainterPath path;
    path.moveTo(0,1);
//     path.arcMoveTo(0.0, 0.0, 1.0, 1.0, -180);
    path.arcTo(0.0, 0.0, 1.0, 2.0, 180, -180);
//     piePath.arcTo(20.0, 30.0, 60.0, 40.0, 60.0, 240.0);
    
    m_generatedPathCount = path.elementCount();
    
    return path;
}

#include "bturngesturev2.moc"

