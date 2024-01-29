#ifndef BASEPOINTSADAPTER_H
#define BASEPOINTSADAPTER_H

#include "ipointsadapter.h"

class BasePointsAdapter : public IPointsAdapter
{
public:
    BasePointsAdapter();

    QVector<QPointF> convertToPoints(const QVector<MeasureData>& _data) override;
};

#endif // BASEPOINTSADAPTER_H
