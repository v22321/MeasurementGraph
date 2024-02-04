#ifndef UIWRAPPER_H
#define UIWRAPPER_H

#include "graph/graphmodel.h"
#include "datacollector.h"

#include <QString>
#include <QtCharts/QVXYModelMapper>
#include <QQmlContext>
#include <QThread>

///
/// \brief The UIWrapper class - Interaction with UI
///
class UIWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList dataFiles READ dataFiles CONSTANT)

public:
    explicit UIWrapper(QObject *parent = nullptr);
    ~UIWrapper();

    void init(QQmlContext* _context);

private:
    ///
    /// \brief updateGraph Update graph with measurement data
    /// \param _points
    /// \return
    ///
    bool updateGraph(QVector<QPointF> _points);
    void updateMaxMinXY(const QPair<QPointF, QPointF>& _maxMinXYPair);
    void setNewHeaders(const QVector<Header>& _headers);

    QStringList dataFiles() const;

private:
    QStringList m_dataFiles;
    QSharedPointer<GraphModel> m_graphData;
    QSharedPointer<QVXYModelMapper> m_seriesMapper;

    /// Collector measurement data from file in thread
    QSharedPointer<DataCollector> m_collector;
    QSharedPointer<QThread> m_collectorThread;

    QSharedPointer<QVector<QPointF>> m_points;

signals:
    /// Start create graph
    void s_createGraph(const QString& _fileName);

    void s_hasError();
    void s_graphUpdated(QSharedPointer<QVector<QPointF>> _points);
    void s_setMaxMinPoints(const QPair<QPointF, QPointF>& _maxMinXYPair);
};

#endif // UIWRAPPER_H
