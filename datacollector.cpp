#include "datacollector.h"
#include "graph/ipointsadapter.h"
#include "graph/basepointsadapter.h"
#include "graph/averagepointsadapter.h"

#include <QDebug>

DataCollector::DataCollector(QObject *parent) : QObject(parent) {}

void DataCollector::initialize()
{
    qInfo() << "START THREAD";
}

void DataCollector::parseFile(const QString &_fileName)
{
    /// Get measurement data
    QSharedPointer<AbstractDataReader> dataReader(new SsdDataReader("SampleFiles/" + _fileName));
    bool hasError = dataReader->readData();
    if (hasError)
    {
        qWarning() << "Can't read file. Stop";
        emit s_hasError();
        return;
    }
    if (dataReader->headers().size() == 0)
    {
        qWarning() << "Not found headers";
        emit s_hasError();
        return;
    }
    if (dataReader->measurements().size() == 0)
    {
        qWarning() << "Not found measurement results";
        emit s_hasError();
        return;
    }

    emit s_newHeaders(dataReader->headers());

    /// Convert to points
    const auto _measureData {dataReader->measurements()};
    QSharedPointer<IPointsAdapter> pointsAdapter;
    if (_measureData.size() < pointsAdapter->getScreenWidth())
        pointsAdapter = QSharedPointer<BasePointsAdapter>::create();
    else
        pointsAdapter = QSharedPointer<AveragePointsAdapter>::create();

    emit s_pointsReady(pointsAdapter->convertToPoints(_measureData));
}
