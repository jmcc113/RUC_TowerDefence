#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QTimer>

class WayPoint;
class QPainter;
class Widget;
class Tower;


class Enemy : public QObject
{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, Widget *game, QList<QPixmap> sprite,int hp,int dam,int go,qreal sp);
    ~Enemy();

    void draw(QPainter *painter) const;
    void move();
    void getDamage(int damage);
    void getRemoved();
    void getAttacked(Tower *attacker);
    void gotLostSight(Tower *attacker);
    QPointF pos() const;
    qreal	m_walkingSpeed;
    qreal   m_originspeed;
    int m_slowlvl;
    int	m_currentHp;
    int m_slowtime;
    int m_stuntime;
    bool m_slowed;
    bool m_stuned;
    bool m_active;
    QTimer *t_slow;
    QTimer *t_stun;

public slots:
    void doActivate();
    void slowtime();
    void stuntime();

private:
    int				m_maxHp;
    int             m_damage;
    int             m_gold;
    qreal			m_rotationSprite;
    QPointF			m_pos;
    WayPoint *      m_passedWayPoint;
    WayPoint *		m_destinationWayPoint;
    Widget * 	m_game;
    QList<Tower *>	m_attackedTowersList;
    QPixmap m_nowsprite;
    QList<QPixmap>	m_sprite;
    static const QSize ms_fixedSize;
};

#endif // ENEMY_H
