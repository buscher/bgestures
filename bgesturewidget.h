

#ifndef BGESTUREWIDGET_H
#define BGESTUREWIDGET_H

#include <QWidget>

class BTurnGesture;
class BPolygonGesture;
class BTurnGestureV2;
class BCutGesture;

class QGestureEvent;
class QEvent;
class QResizeEvent;
class QPaintEvent;
class QMouseEvent;

class BGestureWidget : public QWidget
{
public:
    BGestureWidget();
    
protected:
    bool event( QEvent * e);
    virtual bool gestureEvent(QGestureEvent *event);
    
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void paintEvent ( QPaintEvent * event );
    
    
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent(QMouseEvent* event);
    
    
    void turnGesture(BTurnGesture* gesture);
    void polygonGesture(BPolygonGesture* gesture);
    void turnGestureV2(BTurnGestureV2* gesture);
    void cutGesture(BCutGesture* gesture);
    
private:
    QPixmap* m_background;
    bool m_mousePressed;
};

#endif //BGESTUREWIDGET_H

