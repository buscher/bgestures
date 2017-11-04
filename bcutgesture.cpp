
#include "bcutgesture.h"

#include <QDebug>

Qt::GestureType BCutGesture::Type;

void BCutGestureRecognizer::registerRecognizer()
{
    BCutGesture::Type = QGestureRecognizer::registerRecognizer(new BCutGestureRecognizer(0.20, -1, 20));
}

BCutGestureRecognizer::BCutGestureRecognizer(qreal thres, int minWidth, int minHeight)
    : BPolygonGestureRecognizer(thres, minWidth, minHeight),
      m_generatedPathCount(0)
{
}

bool BCutGestureRecognizer::pointCheck(const QVector< QPoint >& list)
{
    bool ok = true;

    ok &= list.at(0).x() > list.at(1).x();
    ok &= list.at(1).y() < list.at(3).y();
    ok &= list.at(2).x() > list.at(3).x();

    return ok && list.size() == m_generatedPathCount;
}

QPainterPath BCutGestureRecognizer::structure()
{
    QPainterPath path;
    path.moveTo(1,0);
    path.lineTo(0,0);
    path.lineTo(1,1);
    path.lineTo(0,1);
    m_generatedPathCount = path.elementCount();
    return path;
}

#include "bcutgesture.moc"

