#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>

class QPainter;
class Enemy;
class Widget;
class QTimer;

class Tower:public QObject
{
    Q_OBJECT
public:
    Tower(QPointF pos,Widget *game);
    ~Tower();
    bool containPoint(const QPointF &pos) const;
    virtual void draw(QPainter *painter) const;
    virtual void upgrade() = 0;
    void checkEnemyInRange();
    void targetKilled();
    void attackEnemy();
    void chooseEnemyForAttack(Enemy *enemy);
    void lostSightOfEnemy();
    int getlvl() const;
    const QPointF Pos() const;
    bool m_attacking;
    int m_total;

protected slots:
    virtual void shootWeapon() = 0;
    void promote();
protected:
    void sppromote();

    int m_lvl;
    int	m_attackRange;	// 代表塔可以攻击到敌人的距离
    int m_damage;		// 代表攻击敌人时造成的伤害
    int	m_fireRate;		// 代表再次攻击敌人的时间间隔
    int m_price;
    int m_protime;
    bool m_promoted;
    Enemy *	m_chooseEnemy;
    Widget * m_game;
    QTimer * m_fireRateTimer;
    QTimer *t_p;
    QPointF m_pos;
    QPixmap m_sprite[3];

    static const QSize ms_fixedSize;
};

class ArrowTower:public Tower
{
    Q_OBJECT
public:
    ArrowTower(QPointF pos,Widget *game,int lvl=1);
    ~ArrowTower();
    void upgrade();
protected slots:
    void shootWeapon();
};
class MagicTower:public Tower
{
    Q_OBJECT
public:
    MagicTower(QPointF pos,Widget *game,int lvl=1);
    ~MagicTower();
    void upgrade();
protected slots:
    void shootWeapon();
};
class StoneTower:public Tower
{
    Q_OBJECT
public:
    StoneTower(QPointF pos,Widget *game,int lvl=1);
    ~StoneTower();
    void upgrade();
protected slots:
    void shootWeapon();
};
class DeathTower:public Tower
{
    Q_OBJECT
public:
    DeathTower(QPointF pos,Widget *game,int lvl=1);
    ~DeathTower();
    void upgrade();
    void draw(QPainter *painter) const;
protected slots:
    void shootWeapon();
};

#endif // TOWER_H
