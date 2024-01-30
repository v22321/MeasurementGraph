#ifndef IPOINTSADAPTER_H
#define IPOINTSADAPTER_H

#include "data_parser/measuredata.h"
#include "defines.h"

#include <QGuiApplication>
#include <QPointF>
#include <QVector>
#include <QScreen>

class IPointsAdapter {
public:
    IPointsAdapter() = default;
    virtual ~IPointsAdapter() {}

    int32_t getScreenWidth()
    {
        if (GlobalDefs::CUSTOM_WIDTH_GRAPH_PIXELS == 0)
        {
            QSize screenSize { QGuiApplication::primaryScreen()->size() };
            return screenSize.width();
        }
        else
            return GlobalDefs::CUSTOM_WIDTH_GRAPH_PIXELS;
    }
    virtual QVector<QPointF> convertToPoints(const QVector<MeasureData>& _data) = 0;
};

#endif // IPOINTSADAPTER_H
