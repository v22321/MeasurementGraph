#ifndef MEASUREDATA_H
#define MEASUREDATA_H

#include <QPointF>

class MeasureData
{
public:
    MeasureData(const long double _timeStamp, const long double _value);

    void printAll() const;
    QPointF getPoint() const;

private:
    long double m_timeStamp;
    long double m_value;
};

#endif // MEASUREDATA_H
