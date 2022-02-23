#include "tower.h"
#include <QPainter>
#include "bullet.h"
#include "enemy.h"
#include "utility.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
#include "widget.h"
#include <QSound>

const QSize Tower::ms_fixedSize(60, 60);

Tower::Tower(QPointF pos,Widget *game)
    : m_protime(0)
    , m_promoted(false)
    , m_game(game)
    , m_pos(pos)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    t_p = new QTimer(this);
    connect(t_p,SIGNAL(timeout()),this,SLOT(promote()));
}

Tower::~Tower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = nullptr;
}

bool Tower::containPoint(const QPointF &pos) const
{
    bool isXInHere = m_pos.x() < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width());
    bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
    return isXInHere && isYInHere;
}

void Tower::draw(QPainter *painter) const
{
    painter->save();
    if(!m_game->check2){
        painter->setPen(Qt::white);
    // 绘制攻击范围
        painter->drawEllipse(m_pos+QPointF(30,30), m_attackRange, m_attackRange);
    }
    // 绘制偏转坐标,由中心+偏移=左上
    //static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    // 绘制炮塔并选择炮塔
    painter->translate(m_pos);
    //painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
    painter->drawPixmap(m_pos.x(), m_pos.y()-20, m_sprite[m_lvl-1]);
    if(m_promoted)
        painter->drawPixmap(m_pos.x()+5,m_pos.y()-80,60,60,QPixmap(":/image/innerfire.png"));
}

void Tower::checkEnemyInRange()
{
    if (m_chooseEnemy)
    {
        if (!collisionWithCircle(m_pos+QPointF(30,30), m_attackRange, m_chooseEnemy->pos(), 10))
            lostSightOfEnemy();
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        QList<Enemy *> enemyList = m_game->m_enemyList;
        foreach (Enemy *enemy, enemyList)
        {
            if (collisionWithCircle(m_pos, m_attackRange, enemy->pos(), 1)&&enemy->m_active)
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}

void Tower::targetKilled()
{
    if (m_chooseEnemy)
        m_chooseEnemy = nullptr;

    //m_fireRateTimer->stop();
}

void Tower::attackEnemy()
{
    m_fireRateTimer->start(m_fireRate);
}

void Tower::chooseEnemyForAttack(Enemy *enemy)
{
    m_chooseEnemy = enemy;
    //attackEnemy();
    m_chooseEnemy->getAttacked(this);
}

void Tower::lostSightOfEnemy()
{
//    if(m_chooseEnemy!=NULL)

    if (m_chooseEnemy){
        m_chooseEnemy->gotLostSight(this);
        m_chooseEnemy = nullptr;
    }

    //m_fireRateTimer->stop();
}

int Tower::getlvl() const
{
    return m_lvl;
}

const QPointF Tower::Pos()const
{
    return m_pos;
}

void Tower::promote()
{
    if(m_protime!=0)
        m_protime-=100;
    else {
        t_p->stop();
        m_promoted=false;
        m_fireRate=m_fireRate*3/2;
        m_fireRateTimer->start(m_fireRate);
    }
}

void Tower::sppromote()
{
    if(!m_promoted&&m_game->m_skill==2&&generateRandomInteger(1,100)<=25)
    {
        m_promoted=true;
        m_protime=3000;
        m_fireRate=m_fireRate*2/3;
        t_p->start(100);
        m_fireRateTimer->start(m_fireRate);
        QSound::play(":/sound/InnerFire.wav");
    }
}

ArrowTower::ArrowTower(QPointF pos,Widget *game,int lvl)
     :Tower(pos,game)
{
    m_lvl=lvl;
    m_sprite[0]=QPixmap(":/image/arrowtower1.png");
    m_sprite[1]=QPixmap(":/image/arrowtower2.png");
    m_sprite[2]=QPixmap(":/image/arrowtower3.png");
    m_attackRange=325;
    m_attacking=false;
    m_damage=15;
    m_price=50;
    m_total=50;
    m_fireRate=750;
    m_chooseEnemy=nullptr;
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    m_fireRateTimer->start(m_fireRate);
}

ArrowTower::~ArrowTower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = nullptr;
}

void ArrowTower::upgrade()
{
    if(m_game->m_playerGold>=m_total){
        QSound::play(":/sound/upgrade.wav");
        m_game->m_playerGold-=m_total;
        m_lvl++;
        m_total=m_lvl*m_price;
        if(m_lvl==2){
            m_attackRange=350;
            m_damage=25;
        }
        if(m_lvl==3){
            m_attackRange=375;
            m_damage=40;
        }
    }
    else
        QSound::play(":/sound/moregold.wav");
}

void ArrowTower::shootWeapon()
{
    if(m_chooseEnemy==nullptr)
        return;
    sppromote();
    Arrow *arrow = new Arrow(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,m_lvl);
    arrow->move();
    m_game->addArrow(arrow);
    if(m_lvl==1)
        QSound::play(":/sound/arrow1.wav");
    else if(m_lvl==2)
        QSound::play(":/sound/arrow2.wav");
    else if(m_lvl==3)
        QSound::play(":/sound/arrow3.wav");
}

MagicTower::MagicTower(QPointF pos,Widget *game,int lvl)
    :Tower(pos,game)
{
   m_lvl=lvl;
   m_sprite[0]=QPixmap(":/image/magictower1.png");
   m_sprite[1]=QPixmap(":/image/magictower2.png");
   m_sprite[2]=QPixmap(":/image/magictower3.png");
   m_attackRange=250;
   m_attacking=false;
   m_damage=6;
   m_price=70;
   m_total=70;
   m_fireRate=200;
   m_chooseEnemy=nullptr;
   m_fireRateTimer = new QTimer(this);
   connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
   m_fireRateTimer->start(m_fireRate);
}

MagicTower::~MagicTower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = nullptr;
}

void MagicTower::upgrade()
{
    if(m_game->m_playerGold>=m_total){
        QSound::play(":/sound/upgrade.wav");
        m_game->m_playerGold-=m_total;
        m_lvl++;
        m_total=m_lvl*m_price;
        if(m_lvl==2){
            m_attackRange=275;
            m_damage=12;
        }
        if(m_lvl==3){
            m_attackRange=300;
            m_damage=18;
        }
    }
    else
        QSound::play(":/sound/moregold.wav");
}

void MagicTower::shootWeapon()
{
    if(m_chooseEnemy==nullptr)
        return;
    sppromote();
    Magic *magic = new Magic(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,m_lvl);
    magic->move();
    m_game->addMagic(magic);
    if(m_lvl==1)
        QSound::play(":/sound/magic1.wav");
    else if(m_lvl==2)
        QSound::play(":/sound/magic2.wav");
    else if(m_lvl==3)
        QSound::play(":/sound/magic3.wav");
}

StoneTower::StoneTower(QPointF pos,Widget *game,int lvl)
    :Tower(pos,game)
{
    m_lvl=lvl;
    m_sprite[0]=QPixmap(":/image/stonetower1.png");
    m_sprite[1]=QPixmap(":/image/stonetower2.png");
    m_attackRange=400;
    m_attacking=false;
    m_damage=65;
    m_price=80;
    m_total=80;
    m_fireRate=1200;
    m_chooseEnemy=nullptr;
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    m_fireRateTimer->start(m_fireRate);
}

StoneTower::~StoneTower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = nullptr;
}

void StoneTower::upgrade()
{
    if(m_game->m_playerGold>=m_total){
        QSound::play(":/sound/upgrade.wav");
        m_game->m_playerGold-=m_total;
        m_lvl++;
        m_total=m_lvl*m_price;
        m_attackRange=450;
        m_damage=120;
    }
    else
        QSound::play(":/sound/moregold.wav");
}

void StoneTower::shootWeapon()
{
    if(m_chooseEnemy==nullptr)
        return;
    sppromote();
    Stone *stone = new Stone(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,m_lvl);
    stone->move();
    m_game->addStone(stone);
    if(m_lvl==1)
        QSound::play(":/sound/stone1.wav");
    else if(m_lvl==2)
        QSound::play(":/sound/stone2.wav");
}

DeathTower::DeathTower(QPointF pos,Widget *game,int lvl)
    :Tower(pos,game)
{
   m_lvl=lvl;
   m_sprite[0]=QPixmap(":/image/deathtower1.png");
   m_sprite[1]=QPixmap(":/image/deathtower2.png");
   m_sprite[2]=QPixmap(":/image/deathtower3.png");
   m_attackRange=450;
   m_attacking=false;
   m_damage=30;
   m_price=150;
   m_total=150;
   m_fireRate=750;
   m_chooseEnemy=nullptr;
   m_fireRateTimer = new QTimer(this);
   connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
   m_fireRateTimer->start(m_fireRate);
}

DeathTower::~DeathTower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = nullptr;
}

void DeathTower::upgrade()
{   if(m_game->m_playerGold>=m_total){
        QSound::play(":/sound/upgrade.wav");
        m_game->m_playerGold-=m_total;
        m_lvl++;
        m_total=m_lvl*m_price;
        if(m_lvl==2){
            m_attackRange=425;
            m_damage=80;
        }
        if(m_lvl==3){
            m_attackRange=450;
            m_damage=130;
        }
    }
    else
        QSound::play(":/sound/moregold.wav");
}

void DeathTower::shootWeapon()
{
    if(m_chooseEnemy==nullptr)
        return;
    sppromote();
    Death *death = new Death(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,m_lvl);
    death->move();
    m_game->addDeath(death);
    if(m_lvl==1)
        QSound::play(":/sound/death1.wav");
    else if(m_lvl==2)
        QSound::play(":/sound/death2.wav");
    else if(m_lvl==3)
        QSound::play(":/sound/death3.wav");
}

void DeathTower::draw(QPainter *painter) const
{
    painter->save();
    if(!m_game->check2){
        painter->setPen(Qt::white);
    // 绘制攻击范围
        painter->drawEllipse(m_pos+QPointF(30,30), m_attackRange, m_attackRange);
    }
    // 绘制偏转坐标,由中心+偏移=左上
    //static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    // 绘制炮塔并选择炮塔
    painter->translate(m_pos);
    //painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
    painter->drawPixmap(m_pos.x()-12, m_pos.y()-6, m_sprite[m_lvl-1]);
    if(m_promoted)
        painter->drawPixmap(m_pos.x()+5,m_pos.y()-80,60,60,QPixmap(":/image/innerfire.png"));
}
