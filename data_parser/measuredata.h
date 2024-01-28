#ifndef MEASUREDATA_H
#define MEASUREDATA_H

#include <QString>
#include <QPointF>

class MeasureData
{
public:
    MeasureData(const double _timeStamp, const double _value);

    void printAll() const;
    QPointF getPoint() const;

private:
    double m_timeStamp;
    double m_value;
};

#endif // MEASUREDATA_H
