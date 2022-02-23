#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>
#include <QSound>
#include "enemy.h"
#include "waypoint.h"
#include "tower.h"
#include "utility.h"
#include "widget.h"
#include "audioplayer.h"


static const int Health_Bar_Width = 50;

const QSize Enemy::ms_fixedSize(52, 52);

Enemy::Enemy(WayPoint *startWayPoint, Widget *game, QList<QPixmap> sprite,int hp,int dam,int go,qreal sp)
    : QObject(nullptr)
    , m_walkingSpeed(sp)
    , m_originspeed(sp)
    , m_slowlvl(0)
    , m_currentHp(hp)
    , m_slowtime(0)
    , m_stuntime(0)
    , m_slowed(false)
    , m_stuned(false)
    , m_active(false)
    , m_maxHp(hp)
    , m_damage(dam)
    , m_gold(go)
    , m_rotationSprite(0.0)
    , m_pos(startWayPoint->pos())
    , m_passedWayPoint(startWayPoint)
    , m_destinationWayPoint(startWayPoint->nextWayPoint())
    , m_game(game)
{
    m_sprite=sprite;
    t_slow=new QTimer(this);
    connect(t_slow,SIGNAL(timeout()),this,SLOT(slowtime()));
    t_stun=new QTimer(this);
    connect(t_stun,SIGNAL(timeout()),this,SLOT(stuntime()));
}

Enemy::~Enemy()
{
    m_attackedTowersList.clear();
    m_destinationWayPoint = nullptr;
    m_game = nullptr;
}

void Enemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;
    if(m_stuned)
        painter->drawPixmap(m_pos.x()-25,m_pos.y()-120,60,60,QPixmap(":/image/stun.png"));

    painter->save();

    QPointF healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 3-50);
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRectF healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 8));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRectF healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 8));
    painter->drawRect(healthBarRect);

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2-15, -ms_fixedSize.height() / 2-15);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // 绘制敌人
    painter->drawPixmap(offsetPoint, m_nowsprite);


    painter->restore();
}

void Enemy::move()
{
    if (!m_active)
        return;

    if (collisionWithCircle(m_pos, 10, m_destinationWayPoint->pos(), 10))
    {
        // 敌人抵达了一个航点
        if (m_destinationWayPoint->nextWayPoint())
        {
            // 还有下一个航点
            m_pos = m_destinationWayPoint->pos();
            m_passedWayPoint=m_destinationWayPoint;
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        else
        {
            // 表示进入基地
            m_game->getHpDamage(m_damage);
            foreach (Tower *attacker, m_attackedTowersList)
                attacker->targetKilled();
            m_game->removedEnemy(this);

            return;
        }
    }

    // 还在前往航点的路上
    // 目标航点的坐标
    QPointF targetPoint = m_destinationWayPoint->pos();
    qreal movementSpeed = m_walkingSpeed;
    QVector2D normalized(targetPoint - m_pos);
    normalized.normalize();
    QPointF step=(m_destinationWayPoint->pos()-m_passedWayPoint->pos())/200;
    if(!m_game->check){
        if(this->m_destinationWayPoint==m_game->m_waypointList[3])
            m_pos = m_pos + step * movementSpeed;
        else if(this->m_destinationWayPoint==m_game->m_waypointList[4]||this->m_destinationWayPoint==m_game->m_waypointList[2]||this->m_destinationWayPoint==m_game->m_waypointList[0])
            m_pos = m_pos + step * movementSpeed/2;
        else if(this->m_destinationWayPoint==m_game->m_waypointList[5]||this->m_destinationWayPoint==m_game->m_waypointList[1])
            m_pos = m_pos + step * movementSpeed/4*3;
        if(this->m_destinationWayPoint==m_game->m_waypointList[5]||this->m_destinationWayPoint==m_game->m_waypointList[3]||this->m_destinationWayPoint==m_game->m_waypointList[1])
            m_nowsprite=m_sprite[0];
        else if(this->m_destinationWayPoint==m_game->m_waypointList[4]||this->m_destinationWayPoint==m_game->m_waypointList[0])
            m_nowsprite=m_sprite[1];
        else if(this->m_destinationWayPoint==m_game->m_waypointList[2])
            m_nowsprite=m_sprite[2];
    }
    else {
        if(this->m_destinationWayPoint==m_game->m_waypointList[3])
            m_pos = m_pos + step * movementSpeed/2;
        else if(this->m_destinationWayPoint==m_game->m_waypointList[2])
            m_pos = m_pos + step * movementSpeed*4/7;
        else if(this->m_destinationWayPoint==m_game->m_waypointList[4])
            m_pos = m_pos + step * movementSpeed*5/12;
        else if(this->m_destinationWayPoint==m_game->m_waypointList[5])
            m_pos = m_pos + step * movementSpeed/3;
        else if(this->m_destinationWayPoint==m_game->m_waypointList[1])
            m_pos = m_pos + step * movementSpeed*2/3;
        else if(this->m_destinationWayPoint==m_game->m_waypointList[0])
            m_pos = m_pos + step * movementSpeed*3/2;
        if(this->m_destinationWayPoint==m_game->m_waypointList[5]||this->m_destinationWayPoint==m_game->m_waypointList[1])
            m_nowsprite=m_sprite[0];
        else if(this->m_destinationWayPoint==m_game->m_waypointList[4]||this->m_destinationWayPoint==m_game->m_waypointList[0])
            m_nowsprite=m_sprite[1];
        else if(this->m_destinationWayPoint==m_game->m_waypointList[2])
            m_nowsprite=m_sprite[2];
        else if (this->m_destinationWayPoint==m_game->m_waypointList[3])
            m_nowsprite=m_sprite[3];
    }
    // 确定敌人选择方向
}

void Enemy::getDamage(int damage)
{
    //m_game->audioPlayer()->playSound(LaserShootSound);
    m_currentHp -= damage;
    if(m_game->m_skill==1&&generateRandomInteger(1,100)<=7){
        m_game->awardGold(5);
        QSound::play(":/sound/ReceiveGold.wav");
    }

    // 阵亡,需要移除
    if (m_currentHp <= 0)
    {
        //m_game->audioPlayer()->playSound(EnemyDestorySound);
        m_game->awardGold(m_gold);
        getRemoved();
    }
}

void Enemy::getRemoved()
{
    if (m_attackedTowersList.empty())
    {
        m_game->removedEnemy(this);
        return;
    }


    foreach (Tower *attacker, m_attackedTowersList)
        attacker->targetKilled();
    // 通知game,此敌人已经阵亡
    m_game->removedEnemy(this);
}

void Enemy::getAttacked(Tower *attacker)
{
    m_attackedTowersList.push_back(attacker);
}

// 表明敌人已经逃离了攻击范围
void Enemy::gotLostSight(Tower *attacker)
{
    m_attackedTowersList.removeOne(attacker);
}

QPointF Enemy::pos() const
{
    return m_pos;
}

void Enemy::doActivate()
{
    m_active = true;
}

void Enemy::slowtime()
{
    if(m_slowtime!=0)
        m_slowtime-=100;
    else {
        t_slow->stop();
        m_slowed=false;
        m_walkingSpeed=m_originspeed;
        m_slowlvl=0;
    }
}
void Enemy::stuntime()
{
    if(m_stuntime!=0)
        m_stuntime-=100;
    else {
        t_stun->stop();
        m_stuned=false;
        m_walkingSpeed=m_originspeed;
    }
}
