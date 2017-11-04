
#ifndef BCUTGESTURE_H
#define BCUTGESTURE_H

#include "bpolygongesture.h"

class BCutGesture : public BPolygonGesture
{
public:
    static Qt::GestureType Type;

private:
    friend class BCutGestureRecognizer;
};

class BCutGestureRecognizer : public BPolygonGestureRecognizer
{
public:
    static void registerRecognizer();

protected:
    BCutGestureRecognizer(qreal thres, int minWidth, int minHeight);
    
protected:
    virtual QPainterPath structure();
    virtual bool pointCheck(const QVector<QPoint>& list);
    
private:
    int m_generatedPathCount;
};

#endif // BCUTGESTURE_H
