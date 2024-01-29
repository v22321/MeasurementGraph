#ifndef IPOINTSADAPTER_H
#define IPOINTSADAPTER_H

#include "data_parser/measuredata.h"

#include <QGuiApplication>
#include <QPointF>
#include <QVector>
#include <QScreen>

class IPointsAdapter {
public:
    int32_t getScreenWidth()
    {
        QSize screenSize { QGuiApplication::primaryScreen()->size() };
        return screenSize.width();
    }
    virtual QVector<QPointF> convertToPoints(const QVector<MeasureData>& _data) = 0;
};

#endif // IPOINTSADAPTER_H
