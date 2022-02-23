#include "style.h"

int  QTMyIconStyle::pixelMetric(PixelMetric metric, const QStyleOption * option, const QWidget * widget) const
{
    int s = QProxyStyle::pixelMetric(metric, option, widget);
    if (metric == QStyle::PM_SmallIconSize) {
        s = 64;
    }
    return s;
}
