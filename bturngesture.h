
#ifndef BTURNGESTURE_H
#define BTURNGESTURE_H

#include <QGesture>
#include <QGestureRecognizer>

class BTurnGesture : public QGesture
{
  Q_OBJECT
  Q_PROPERTY(int completionStatus READ completionStatus WRITE setCompletionStatus)
  Q_PROPERTY(QPointF position READ position WRITE setPosition)
public:
  static Qt::GestureType Type;

  BTurnGesture();

  int completionStatus() const;
  void setCompletionStatus(int value);
  QPointF position() const;
  void setPosition(const QPointF &value);
private:
  int m_completionStatus;
  QPointF m_position;
  friend class BTurnGestureRecognizer;
};


class BTurnGestureRecognizer : public QGestureRecognizer
{
public:
    static void registerRecognizer();

    enum TurnState {
            Start = 1,
            RightUp,
            RightDown,
            Done //unused?
    };

private:
    BTurnGestureRecognizer(bool feedback);

    QGesture *create(QObject *);
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event);
    void reset(QGesture *q);

    bool m_feedback;

    const static int Timeout = 5000;
};


#endif //BTURNGESTURE_H
