#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QMenu>

class QPainter;

class Towerposition
{
public:
    Towerposition(QPointF pos, const QPixmap &sprite = QPixmap(":/image/basic.png"));
    void draw(QPainter *painter) const;
    bool containPoint(const QPointF &pos) const;
    const QPointF Pos() const;
    const QPointF centerPos() const;

private:
    QPointF		m_pos;
    QPixmap		m_sprite;

    static const QSize ms_fixedSize;
};

#endif // TOWERPOSITION_H
