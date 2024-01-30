#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QObject>
#include "data_parser/ssddatareader.h"

class DataCollector : public QObject
{
    Q_OBJECT
public:
    DataCollector(QObject *parent = nullptr);

    void initialize();
    void parseFile(const QString& _fileName);

signals:
    void s_pointsReady(const QVector<QPointF>& _points);
    void s_newHeaders(const QVector<Header>& _headers);
    void s_hasError();
};

#endif // DATACOLLECTOR_H
