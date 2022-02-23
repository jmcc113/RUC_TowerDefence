#include "towerposition.h"
#include <QPainter>
#include <QMenu>

const QSize Towerposition::ms_fixedSize(70, 70);

Towerposition::Towerposition(QPointF pos, const QPixmap &sprite)
    : m_pos(pos)
    , m_sprite(sprite)
{
}

void Towerposition::draw(QPainter *painter) const
{
    painter->drawPixmap(static_cast<int>(m_pos.x()),static_cast<int>(m_pos.y()),70,70, m_sprite);
}

bool Towerposition::containPoint(const QPointF &pos) const
{
    bool isXInHere = m_pos.x() < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width());
    bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
    return isXInHere && isYInHere;
}

const QPointF Towerposition::centerPos() const
{
    QPoint offsetPoint(ms_fixedSize.width() / 2, ms_fixedSize.height() / 2);
    return m_pos + offsetPoint;
}

const QPointF Towerposition::Pos()const
{
    return m_pos;
}
