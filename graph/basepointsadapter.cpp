#include "basepointsadapter.h"

BasePointsAdapter::BasePointsAdapter() {}

QVector<QPointF> BasePointsAdapter::convertToPoints(const QVector<MeasureData> &_measureData)
{
    QVector<QPointF> resultPoints {};
    const size_t measureDataSize { _measureData.size() };
    for (size_t index = 0; index < measureDataSize; ++index)
    {
        QPointF currPoint { _measureData[index].getPoint() };
        resultPoints.append(currPoint);
    }
    return resultPoints;
}
