#include "uiwrapper.h"
#include "defines.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>

UIWrapper::UIWrapper(QObject *parent) :
    QObject(parent),
    m_graphData(QSharedPointer<GraphModel>::create()),
    m_seriesMapper(QSharedPointer<QVXYModelMapper>::create()),
    m_collector(QSharedPointer<DataCollector>::create()),
    m_collectorThread(QSharedPointer<QThread>::create())
{
    /// Get files from custom dir
    QString filesDir { QString("%1%2%3").arg(QCoreApplication::applicationDirPath(),
                                             QDir::separator(),
                                             GlobalDefs::MEASUREMENT_FILES_DIR) };
    QDir directory(filesDir);
    m_dataFiles = directory.entryList(QDir::Files);

    /// Setup collector
    m_collector->moveToThread(m_collectorThread.data());
    connect (m_collectorThread.data(), &QThread::started, m_collector.data(), &DataCollector::init);
    connect (m_collectorThread.data(), &QThread::finished, m_collectorThread.data(), &QThread::deleteLater);
    connect (this, &UIWrapper::s_createGraph, m_collector.data(), &DataCollector::collect);
    connect (m_collector.data(), &DataCollector::s_pointsReady, this, &UIWrapper::updateGraph);
    connect (m_collector.data(), &DataCollector::s_newHeaders, this, &UIWrapper::setNewHeaders);
    connect (m_collector.data(), &DataCollector::s_hasError, this, &UIWrapper::s_hasError);
    connect (m_collector.data(), &DataCollector::s_setMaxMinXY, this, &UIWrapper::updateMaxMinXY);
    m_collectorThread->start();
}

UIWrapper::~UIWrapper()
{
    if (m_collectorThread)
    {
        m_collectorThread->quit();
        m_collectorThread->wait();
    }
}

void UIWrapper::init(QQmlContext *_context)
{
    if (!_context)
        return;

    _context->setContextProperty("wrapper", this);
    _context->setContextProperty("seriesMapper", m_seriesMapper.data());
    _context->setContextProperty("graphData", m_graphData.data());
}

bool UIWrapper::updateGraph(const QSharedPointer<QVector<QPointF>>& _points)
{
    if (!m_graphData || !m_seriesMapper || !_points)
    {
        qWarning() << "Not found models: graph data or series mapper. Can't update graph";
        return true;
    }
    /// Set graph data
    emit s_graphUpdated(_points);

    // m_seriesMapper->setXColumn(0);
    // m_seriesMapper->setYColumn(1);
    // m_seriesMapper->setModel(m_graphData.data());

    return false;
}

void UIWrapper::updateMaxMinXY(const QPair<QPointF, QPointF> &_maxMinXYPair)
{
    if (!m_graphData)
    {
        qWarning() << "Not found graph data";
        return;
    }

    // m_graphData->setMaxMinPoints(_maxMinXYPair);
    emit s_setMaxMinPoints(_maxMinXYPair);
}

void UIWrapper::setNewHeaders(const QVector<Header> &_headers)
{
    if (!m_graphData)
    {
        qWarning() << "Not found graph data. Can't update headers";
        return;
    }

    m_graphData->addInformation(_headers);
}

QStringList UIWrapper::dataFiles() const
{
    return m_dataFiles;
}
