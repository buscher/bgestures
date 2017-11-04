
#ifndef BPOLYGONGESTURE_H
#define BPOLYGONGESTURE_H

#include <QGesture>
#include <QGestureRecognizer>

#include <QMap>
#include <QVector>
#include <QPoint>
#include <QPolygon>
#include <QPainterPath>

class BPolygonGesture : public QGesture
{
protected:
  Q_OBJECT
  Q_PROPERTY(int completionStatus READ completionStatus WRITE setCompletionStatus)
  Q_PROPERTY(QPointF position READ position WRITE setPosition)
public:
  static Qt::GestureType Type;

  BPolygonGesture();

  int completionStatus() const;
  void setCompletionStatus(int value);
  QPointF position() const;
  void setPosition(const QPointF &value);
private:
  int m_completionStatus;
  QPointF m_position;
  friend class BPolygonGestureRecognizer;
};


class BPolygonGestureRecognizer : public QGestureRecognizer
{
public:
    static void registerRecognizer();

protected:
    virtual QPainterPath structure();
    virtual bool pointCheck(const QVector<QPoint>& list);
    
protected:
    BPolygonGestureRecognizer(qreal threshold, int minWidth = -1, int minHeight = -1);

    QGesture *create(QObject *);
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event);
    void reset(QGesture *q);

    QMap<int, QVector<QPoint>* > m_pointListMap;

    static int NextID;
    qreal m_threshold;
    
    int m_minWidth;
    int m_minHeight;
};


#endif //BPOLYGONGESTURE_H
