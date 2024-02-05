#include "basepointsadapter.h"

BasePointsAdapter::BasePointsAdapter() {}

QVector<QPointF> BasePointsAdapter::convertToPoints(const QVector<MeasureData> &_measureData)
{
    QVector<QPointF> resultPoints {};
    const qsizetype measureDataSize { _measureData.size() };
    for (qsizetype index = 0; index < measureDataSize; ++index)
    {
        resultPoints.emplace_back(_measureData[index].getPoint());
    }
    return resultPoints;
}
