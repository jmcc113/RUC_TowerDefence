#ifndef STYLE_H
#define STYLE_H

#include<QObject>
#include<QProxyStyle>


class QTMyIconStyle : public QProxyStyle
{
    Q_OBJECT
public:
    QTMyIconStyle (){}
    virtual ~QTMyIconStyle (){}
    virtual int pixelMetric(PixelMetric metric, const QStyleOption * option, const QWidget * widget) const;
};

#endif // STYLE_H
