#include "defines.h"
#include "datacollector.h"
#include "graph/basepointsadapter.h"
#include "graph/averagepointsadapter.h"

#include <QDebug>
#include <QDir>

DataCollector::DataCollector(QObject *parent) : QObject(parent) {}

void DataCollector::init()
{
    qInfo() << "Collector is working...";
}

void DataCollector::collect(const QString &_fileName)
{
    if (_fileName.isEmpty())
    {
        qWarning() << "Collector can\'t parse empty file";
        emit s_hasError();
        return;
    }

    const auto dataReader { parseFile(_fileName) };
    if (!dataReader)
        return;

    /// update headers
    emit s_newHeaders(dataReader->headers());

    /// convert data to points
    emit s_pointsReady(getPoints(dataReader->measurements()));
}

QSharedPointer<AbstractDataReader> DataCollector::parseFile(const QString &_fileName)
{
    /// Get measurement data
    QSharedPointer<AbstractDataReader> dataReader(
        new SsdDataReader(QString("%1%2%3").arg(
            GlobalDefs::MEASUREMENT_FILES_DIR,
            QDir::separator(),
            _fileName))
        );
    bool hasError { dataReader->readData() };
    if (hasError)
    {
        qWarning() << "Can't read file. Stop";
        emit s_hasError();
        return {};
    }
    if (dataReader->headers().size() == 0)
    {
        qWarning() << "Not found headers";
        emit s_hasError();
        return {};
    }
    if (dataReader->measurements().size() == 0)
    {
        qWarning() << "Not found measurement results";
        emit s_hasError();
        return {};
    }

    return dataReader;
}

QVector<QPointF> DataCollector::getPoints(const QVector<MeasureData> &_measureData)
{
    QSharedPointer<IPointsAdapter> pointsAdapter;
    if (_measureData.size() < pointsAdapter->getScreenWidth())
        pointsAdapter = QSharedPointer<BasePointsAdapter>::create();
    else
        pointsAdapter = QSharedPointer<AveragePointsAdapter>::create();

    const auto& resultPoints { pointsAdapter->convertToPoints(_measureData) };
    qInfo() << "Points count: " << resultPoints.size();
    if (resultPoints.size() > 0)
    {
        QPair<QPointF, QPointF> borderPoints { QPointF(resultPoints[0].x(), resultPoints[0].y()),
                                               QPointF(resultPoints[0].x(), resultPoints[0].y())};
        for (const auto& point : resultPoints)
        {
            if (point.x() > borderPoints.first.x()) borderPoints.first.setX(point.x());
            if (point.y() > borderPoints.first.y()) borderPoints.first.setY(point.y());
            if (point.x() < borderPoints.second.x()) borderPoints.second.setX(point.x());
            if (point.y() < borderPoints.second.y()) borderPoints.second.setY(point.y());
        }
        emit s_setMaxMinXY(borderPoints);
    }

    return resultPoints;
}
