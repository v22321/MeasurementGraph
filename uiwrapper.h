#ifndef UIWRAPPER_H
#define UIWRAPPER_H

#include "graph/graphmodel.h"
#include "datacollector.h"

#include <QString>
#include <QObject>
#include <QtCharts/QVXYModelMapper>
#include <QQmlContext>

class UIWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList fileNames READ fileNames CONSTANT)

    const QString MEASUREMENT_FILES_DIR { "SampleFiles" };
public:
    explicit UIWrapper (QObject *parent = nullptr);
    ~UIWrapper();

    void init(QQmlContext* _context);

    QStringList fileNames();

private:
    bool createGraph(const QVector<QPointF>& _points);
    void setNewHeaders(const QVector<Header>& _headers);

private:
    QStringList m_filesList;
    QSharedPointer<GraphModel> m_graphData;
    QSharedPointer<QVXYModelMapper> m_seriesMapper;

    QSharedPointer<DataCollector> m_collector;
    QSharedPointer<QThread> m_collectorThread;

signals:
    void s_hasError();
    void s_graphUpdated();
    void s_createGraph(const QString& _fileName);
};

#endif // UIWRAPPER_H
