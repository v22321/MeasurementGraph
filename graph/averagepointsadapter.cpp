#include "averagepointsadapter.h"

AveragePointsAdapter::AveragePointsAdapter() {}

QVector<QPointF> AveragePointsAdapter::convertToPoints(const QVector<MeasureData> &_measureData)
{
    QVector<QPointF> resultPoints {};
    const qsizetype measureDataSize { _measureData.size() };
    /// If we have many points, we need to calculate the average
    const auto& screenWidth { getScreenWidth() };
    const qsizetype stepSize { measureDataSize > screenWidth ? measureDataSize / screenWidth : 1 };

    qreal averageValue { 0.0 };
    uint32_t stepNumber { 1 };
    for (qsizetype index = 0; index < measureDataSize; ++index)
    {
        QPointF currPoint { _measureData[index].getPoint() };
        /// If we have many points, we need to calculate the average
        if (stepSize > 1)
        {
            averageValue += currPoint.y();

            if (index < stepSize * stepNumber - 1)
                continue;

            currPoint.setY(averageValue / stepSize);
            currPoint.setX(_measureData[stepSize * (stepNumber - 1)].getPoint().x());

            averageValue = 0;
            ++stepNumber;
        }

        resultPoints.append(currPoint);
    }
    return resultPoints;
}
