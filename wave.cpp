#include "wave.h"

Wave::Wave(int n,int spawn,qreal sp,int h,int d,int g,int wg,QString na,QPixmap q1,QPixmap q2,QPixmap q3,QPixmap q4)
    : name(na)
    , number(n)
    , spawntime(spawn)
    , damage(d)
    , gold(g)
    , wgold(wg)
    , hp(h)
    , speed(sp)
{
    m_sprite.append(q1);
    m_sprite.append(q2);
    m_sprite.append(q3);
    m_sprite.append(q4);
    if(speed<=2)
        speed_="慢";
    else if(speed>2&&speed<=4)
        speed_="中等";
    else if(speed>4)
        speed_="快";
}

void Wave::appenddes(QString s)
{
    des=s;
}
