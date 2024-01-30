#include "uiwrapper.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QSharedPointer>
#include <QDebug>
#include <QDir>
#include <QThread>

#include "data_parser/ssddatareader.h"
#include "graph/graphmodel.h"


UIWrapper::UIWrapper(QObject *parent) :
    QObject(parent)
{
    connect(this, &UIWrapper::s_createGraph, this, &UIWrapper::createGraph, Qt::QueuedConnection);
}

void UIWrapper::init(QQmlContext *_context)
{
    QString filesDir { QCoreApplication::applicationDirPath() + "/SampleFiles/" };
    QDir directory(filesDir);
    m_filesList = directory.entryList(QDir::Files);

    m_graphData = QSharedPointer<GraphModel>::create();
    m_seriesMapper = QSharedPointer<QVXYModelMapper>::create();

    _context->setContextProperty("wrapper", this);
    _context->setContextProperty("seriesMapper", m_seriesMapper.data());
    _context->setContextProperty("graphData", m_graphData.data());
}

bool UIWrapper::createGraph(const QString& _fileName)
{
    bool hasError {true};

    /// Get measurement data
    QSharedPointer<AbstractDataReader> dataReader(new SsdDataReader("SampleFiles/" + _fileName));
    hasError = dataReader->readData();
    if (hasError)
    {
        qWarning() << "Can't read file. Stop";
        emit s_hasError();
        return -1;
    }

    const auto& measurements { dataReader->measurements() };
    if (measurements.size() == 0)
    {
        qWarning() << "Empty measurements";
        emit s_hasError();
        return -1;
    }

    const auto& firstElement { measurements.at(0) };
    m_graphData->setStartPoint(firstElement.getPoint());

    /// Set graph data
    m_graphData->addPoints(dataReader->measurements());
    m_graphData->addInformation(dataReader->headers());

    m_seriesMapper->setXColumn(0);
    m_seriesMapper->setYColumn(1);
    m_seriesMapper->setModel(m_graphData.data());
}

QStringList UIWrapper::fileNames()
{
    return m_filesList;
}
