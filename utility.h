#ifndef UTILITY_H
#define UTILITY_H

#include <QtMath>
#include <QPointF>
#include <QTime>

// 判断接触
inline bool collisionWithCircle(QPointF point1, int radius1, QPointF point2, int radius2)
{
    const double xdif = point1.x() - point2.x();
    const double ydif = point1.y() - point2.y();
    const double distance = qSqrt(xdif * xdif + ydif * ydif);

    if (distance <= radius1 + radius2)
        return true;

    return false;
}

// 产生随机数 min:随机数的最小值，max:随机数的最大值
inline int generateRandomInteger(int min, int max)
{
    Q_ASSERT(min < max);
    // 加入随机种子。种子是当前时间距离0点0分0秒的秒数。
    // 每次启动程序，只添加一次种子，以做到数字真正随机。
    static bool seedStatus;
    if (!seedStatus)
    {
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        seedStatus = true;
    }
    int nRandom = qrand() % (max - min);
    nRandom = min + nRandom;

    return nRandom;
}


#endif // UTILITY_H
