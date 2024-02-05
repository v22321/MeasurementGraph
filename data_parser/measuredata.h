#ifndef MEASUREDATA_H
#define MEASUREDATA_H

#include <QPointF>

class MeasureData
{
public:
    MeasureData(const qreal _timeStamp, const qreal _value);

    void printAll() const;
    QPointF getPoint() const;

private:
    qreal m_timeStamp;
    qreal m_value;
};

#endif // MEASUREDATA_H
