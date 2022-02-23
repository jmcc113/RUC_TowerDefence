#include "bullet.h"
#include "utility.h"
#include "enemy.h"
#include "widget.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <qmath.h>
#include <QSound>

const QSize Bullet::ms_fixedSize(8, 8);

Bullet::Bullet(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
               Widget *game,int lvl)
    : m_target(target)
    , m_startPos(startPos)
    , m_targetPos(targetPoint)
    , m_currentPos(startPos)
    , m_targetStartPos(targetPoint)
    , m_game(game)
    , m_damage(damage)
    , m_lvl(lvl)
{
}

void Bullet::draw(QPainter *painter) const
{
    if (m_game->m_enemyList.indexOf(m_target) != -1)
    {
        QVector2D normalized(m_targetPos - m_currentPos);
        normalized.normalize();
        qreal rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x()));
        painter->save();
        painter->translate(m_currentPos);
        painter->rotate(rotationSprite+90);
    }
    else
    {
        QVector2D normalized(m_targetEndPos - m_currentPos);
        normalized.normalize();
        qreal rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x()));
        painter->save();
        painter->translate(m_currentPos);
        painter->rotate(rotationSprite+90);
    }
    static const QPoint offsetPoint(-50/2, -50/2);
    painter->drawPixmap(offsetPoint, m_sprite[m_lvl-1]);
    painter->restore();
}

void Bullet::setCurrentPos(QPointF pos)
{
    m_currentPos = pos;
}

void Bullet::getEndPos(QPointF pos)
{
    m_targetEndPos=pos;
}

QPointF Bullet::currentPos() const
{
    return m_currentPos;
}

Arrow::Arrow(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
             Widget *game, int lvl,const QPixmap &sprite1,const QPixmap &sprite2,const QPixmap &sprite3)
    :Bullet(startPos,targetPoint,damage,target,game,lvl)
{
    m_sprite[0]=sprite1;
    m_sprite[1]=sprite2;
    m_sprite[2]=sprite3;
}

void Arrow::move()
{
    QPointF sts=(m_targetPos-m_currentPos);
    QPointF step=(m_targetStartPos-m_startPos);
    QVector2D v=QVector2D(sts);
    QVector2D v0=QVector2D(step);
    m_targetPos=m_target->pos();
    if (m_game->m_enemyList.indexOf(m_target) == -1)
    {
        QPointF ste=(m_targetEndPos-m_currentPos);
        QVector2D vs=QVector2D(ste);
        float speed=v0.length()/vs.length();
        m_currentPos = m_currentPos + ste * static_cast<qreal>(speed)*0.1;
        if(collisionWithCircle(m_currentPos, 15, m_targetEndPos, 15))
            hitTarget();
    }
    else
    {
        float speed=v0.length()/v.length();
        m_currentPos = m_currentPos + sts * static_cast<qreal>(speed)*0.1;
        if(collisionWithCircle(m_currentPos, 15, m_targetPos, 15))
            hitTarget();
    }
}

void Arrow::hitTarget()
{
    // 这样处理的原因是:
    // 可能多个炮弹击中敌人,而其中一个将其消灭,导致敌人delete
    // 后续炮弹再攻击到的敌人就是无效内存区域
    // 因此先判断下敌人是否还有效
    if (m_game->m_enemyList.indexOf(m_target) != -1)
    {
        m_target->getDamage(m_damage);
        stun(m_target);
    }
    m_game->removedArrow(this);
}

void Arrow::stun(Enemy *enemy)
{
    if (m_game->m_enemyList.indexOf(m_target) != -1){
        if(m_game->m_skill==3&&generateRandomInteger(1,100)<=17)
        {
            enemy->m_stuned=true;
            enemy->m_stuntime=1000;
            enemy->m_walkingSpeed=0;
            enemy->t_stun->start(100);
            QSound::play(":/sound/Stun.wav");
        }
    }
}

Magic::Magic(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
             Widget *game,int lvl,const QPixmap &sprite1,const QPixmap &sprite2,const QPixmap &sprite3)
    :Bullet(startPos,targetPoint,damage,target,game,lvl)
{
    m_sprite[0]=sprite1;
    m_sprite[1]=sprite2;
    m_sprite[2]=sprite3;
}

void Magic::move()
{
    QPointF sts=(m_targetPos-m_currentPos);
    QPointF step=(m_targetStartPos-m_startPos);
    QVector2D v=QVector2D(sts);
    QVector2D v0=QVector2D(step);
    m_targetPos=m_target->pos();
    if (m_game->m_enemyList.indexOf(m_target) == -1)
    {
        QPointF ste=(m_targetEndPos-m_currentPos);
        QVector2D vs=QVector2D(ste);
        float speed=v0.length()/vs.length();
        m_currentPos = m_currentPos + ste * static_cast<qreal>(speed)*0.1;
        if(collisionWithCircle(m_currentPos, 15, m_targetEndPos, 15))
            hitTarget();
    }
    else
    {
        float speed=v0.length()/v.length();
        m_currentPos = m_currentPos + sts * static_cast<qreal>(speed)*0.1;
        if(collisionWithCircle(m_currentPos, 15, m_targetPos, 15))
            hitTarget();
    }
}

void Magic::hitTarget()
{
    // 这样处理的原因是:
    // 可能多个炮弹击中敌人,而其中一个将其消灭,导致敌人delete
    // 后续炮弹再攻击到的敌人就是无效内存区域
    // 因此先判断下敌人是否还有效
    if (m_game->m_enemyList.indexOf(m_target) != -1)
    {
        m_target->getDamage(m_damage);
        slowdown(m_target);
    }
    m_game->removedMagic(this);
}

void Magic::slowdown(Enemy *enemy)
{
    if (m_game->m_enemyList.indexOf(m_target) != -1)
    {
        enemy->m_slowtime=2000;
        if(enemy->m_slowlvl<m_lvl)
        {
            if(m_lvl==1)
                enemy->m_walkingSpeed=enemy->m_originspeed*0.7;
            else if(m_lvl==2)
                enemy->m_walkingSpeed=enemy->m_originspeed*0.65;
            else if(m_lvl==3)
                enemy->m_walkingSpeed=enemy->m_originspeed*0.6;
            enemy->m_slowlvl=m_lvl;
            enemy->m_slowed=true;
            enemy->t_slow->start(100);
        }
    }
}

Stone::Stone(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
             Widget *game, int lvl,const QPixmap &sprite1,const QPixmap &sprite2)
    :Bullet(startPos,targetPoint,damage,target,game,lvl)
{
    m_range=140;
    m_ratk=35*m_lvl;
    m_sprite[0]=sprite1;
    m_sprite[1]=sprite2;
    m_sprite[2]=sprite2;
}

void Stone::move()
{
    QPointF sts=(m_targetPos-m_currentPos);
    QPointF step=(m_targetStartPos-m_startPos);
    QVector2D v=QVector2D(sts);
    QVector2D v0=QVector2D(step);
    m_targetPos=m_target->pos();
    if (m_game->m_enemyList.indexOf(m_target) == -1)
    {
        QPointF ste=(m_targetEndPos-m_currentPos);
        QVector2D vs=QVector2D(ste);
        float speed=v0.length()/vs.length();
        m_currentPos = m_currentPos + ste * static_cast<qreal>(speed)*0.1;
        if(collisionWithCircle(m_currentPos, 15, m_targetEndPos, 15))
            hitTarget();
    }
    else
    {
        float speed=v0.length()/v.length();
        m_currentPos = m_currentPos + sts * static_cast<qreal>(speed)*0.1;
        if(collisionWithCircle(m_currentPos, 15, m_targetPos, 15))
            hitTarget();
    }
}

void Stone::hitTarget()
{
    // 这样处理的原因是:
    // 可能多个炮弹击中敌人,而其中一个将其消灭,导致敌人delete
    // 后续炮弹再攻击到的敌人就是无效内存区域
    // 因此先判断下敌人是否还有效
    if (m_game->m_enemyList.indexOf(m_target) != -1)
        m_target->getDamage(m_damage);
    rangeatk();
    m_game->removedStone(this);
}

void Stone::rangeatk()
{
    foreach(Enemy *enemy,m_game->m_enemyList)
    {
        if (collisionWithCircle(m_currentPos,m_range, enemy->pos(), 10)&&enemy!=m_target)
            enemy->getDamage(m_ratk);
    }
}

Death::Death(QPointF startPos, QPointF targetPoint, int damage, Enemy *target,
             Widget *game, int lvl,const QPixmap &sprite1,const QPixmap &sprite2,const QPixmap &sprite3)
    :Bullet(startPos,targetPoint,damage,target,game,lvl)
{
    m_per=0.04*m_lvl;
    m_sprite[0]=sprite1;
    m_sprite[1]=sprite2;
    m_sprite[2]=sprite3;
}

void Death::move()
{
    QPointF sts=(m_targetPos-m_currentPos);
    QPointF step=(m_targetStartPos-m_startPos);
    QVector2D v=QVector2D(sts);
    QVector2D v0=QVector2D(step);
    m_targetPos=m_target->pos();
    if (m_game->m_enemyList.indexOf(m_target) == -1)
    {
        QPointF ste=(m_targetEndPos-m_currentPos);
        QVector2D vs=QVector2D(ste);
        float speed=v0.length()/vs.length();
        m_currentPos = m_currentPos + ste * static_cast<qreal>(speed)*0.1;
        if(collisionWithCircle(m_currentPos, 15, m_targetEndPos, 15))
            hitTarget();
    }
    else
    {
        float speed=v0.length()/v.length();
        m_currentPos = m_currentPos + sts * static_cast<qreal>(speed)*0.1;
        if(collisionWithCircle(m_currentPos, 15, m_targetPos, 15))
            hitTarget();
    }
}

void Death::hitTarget()
{
    // 这样处理的原因是:
    // 可能多个炮弹击中敌人,而其中一个将其消灭,导致敌人delete
    // 后续炮弹再攻击到的敌人就是无效内存区域
    // 因此先判断下敌人是否还有效
    if (m_game->m_enemyList.indexOf(m_target) != -1)
    {
        m_target->getDamage(m_damage);
        peratk(m_target);
    }
    m_game->removedDeath(this);
}

void Death::peratk(Enemy *enemy){
    enemy->m_currentHp-=static_cast<int>(enemy->m_currentHp*m_per);
}
