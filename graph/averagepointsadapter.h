#ifndef AVERAGEPOINTSADAPTER_H
#define AVERAGEPOINTSADAPTER_H

#include "ipointsadapter.h"

class AveragePointsAdapter : public IPointsAdapter
{
public:
    AveragePointsAdapter();

    QVector<QPointF> convertToPoints(const QVector<MeasureData>& _data) override;
};

#endif // AVERAGEPOINTSADAPTER_H
