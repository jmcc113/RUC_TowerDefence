#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class Widget;

class Bullet : QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF m_currentPos READ currentPos WRITE setCurrentPos)

public:
    Bullet(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,Widget *game,int lvl=1);
    void draw(QPainter *painter) const;
    void setCurrentPos(QPointF pos);
    void getEndPos(QPointF pos);
    QPointF currentPos() const;
    virtual void move() = 0;

    friend class Widget;

protected:
    virtual void hitTarget() = 0;
    Enemy *	m_target;
    QPointF	m_startPos;
    QPointF	m_targetPos;
    QPointF	m_currentPos;
    QPointF m_targetStartPos;
    QPointF m_targetEndPos;
    QPixmap	m_sprite[3];
    Widget *m_game;
    int	m_damage;
    int m_lvl;

    static const QSize ms_fixedSize;
};

class Arrow : public Bullet
{
public:
    Arrow(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
          Widget *game, int lvl=1, const QPixmap &sprite1 = QPixmap(":/image/arrow1.png"),
          const QPixmap &sprite2 = QPixmap(":/image/arrow2.png"),const QPixmap &sprite3 = QPixmap(":/image/arrow3.png"));
    void move();

protected:
    void hitTarget();
    void stun(Enemy *enemy);
};

class Magic : public Bullet
{
public:
    Magic(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
          Widget *game,int lvl=1, const QPixmap &sprite1 = QPixmap(":/image/magic1.png"),
          const QPixmap &sprite2 = QPixmap(":/image/magic2.png"),const QPixmap &sprite3 = QPixmap(":/image/magic3.png"));
    void move();

private:
    void hitTarget();
    void slowdown(Enemy *enemy);
};
class Stone : public Bullet
{
public:
    Stone(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
          Widget *game,int lvl=1, const QPixmap &sprite1 = QPixmap(":/image/stone1.png"),
          const QPixmap &sprite2 = QPixmap(":/image/stone2.png"));
    void move();
    void rangeatk();
private:
    qreal m_range;
    int m_ratk;
protected:
    void hitTarget();
};
class Death : public Bullet
{
public:
    Death(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
          Widget *game,int lvl=1, const QPixmap &sprite1 = QPixmap(":/image/death1.png"),
          const QPixmap &sprite2 = QPixmap(":/image/death2.png"),const QPixmap &sprite3 = QPixmap(":/image/death3.png"));
    void move();
    void peratk(Enemy *enemy);
private:
    qreal m_per;
protected:
    void hitTarget();
};

#endif // BULLET_H
