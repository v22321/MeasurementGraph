#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include "data_parser/header.h"
#include "data_parser/ssddatareader.h"

#include <QObject>
#include <QPointF>

///
/// \brief The DataCollector class Collect measurement data and headers in a separate thread
///
class DataCollector : public QObject
{
    Q_OBJECT
public:
    DataCollector(QObject *parent = nullptr);

    void init();
    void collect(const QString& _fileName);

private:
    QSharedPointer<AbstractDataReader> parseFile(const QString& _fileName);
    QVector<QPointF> getPoints(const QVector<MeasureData>& _measureData);

signals:
    void s_pointsReady(QVector<QPointF> _points);
    void s_newHeaders(const QVector<Header>& _headers);
    void s_hasError();
    void s_setMaxMinXY(const QPair<QPointF, QPointF>&);
};

#endif // DATACOLLECTOR_H
