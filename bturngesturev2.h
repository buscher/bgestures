
#ifndef BTURNGESTUREV2_H
#define BTURNGESTUREV2_H

#include "bpolygongesture.h"

class BTurnGestureV2 : public BPolygonGesture
{
public:
    static Qt::GestureType Type;

private:
    friend class BTurnGestureRegognizerV2;
};

class BTurnGestureRecognizerV2 : public BPolygonGestureRecognizer
{
public:
    static void registerRecognizer();

protected:
    BTurnGestureRecognizerV2(qreal);
    
protected:
    virtual QPainterPath structure();
    virtual bool pointCheck(const QVector<QPoint>& list);
    
private:
    int m_generatedPathCount;
};

#endif // BTURNGESTUREV2_H
