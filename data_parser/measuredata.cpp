#include "measuredata.h"
#include <QDebug>

MeasureData::MeasureData(const long double _timeStamp, const long double _value)
    : m_timeStamp(_timeStamp), m_value(_value)
{}

void MeasureData::printAll() const
{
    qInfo() << "===";
    // qInfo() << "Time stamp: " << m_timeStamp << ", value: " << m_value;
    qInfo() << "===";
}

QPointF MeasureData::getPoint() const
{
    return QPointF(m_timeStamp, m_value);
}
