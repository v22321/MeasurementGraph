#include "graphpainter.h"

GraphPainter::GraphPainter(QQuickItem* _parent)
    : QQuickPaintedItem(_parent),
      m_watchersReadyCount(0),
      m_deltaX(0.0),
      m_deltaY(0.0),
      m_minX(0.0),
      m_minY(0.0)
{
    m_points = QSharedPointer<QVector<QPointF>>::create();

    const auto currentThreadCount { QThread::idealThreadCount() > 1
                                    ? QThread::idealThreadCount() - 1
                                    : 1 };
    m_watchers.reserve(currentThreadCount);
    for (qint32 watcherNum {0}; watcherNum < currentThreadCount; ++watcherNum)
    {
        m_watchers.emplace_back(QSharedPointer<QFutureWatcher<qsizetype>>::create());

        auto currentWatcher = m_watchers[watcherNum];
        connect (currentWatcher.data(), &QFutureWatcher<void>::finished, this, [this, currentWatcher]()
        {
            qInfo() << "Number of loading points: " << currentWatcher->result();
            update();
            if (--m_watchersReadyCount == 0)
                emit s_graphUpdated();
        });
    }

    connect(this, &GraphPainter::heightChanged, this, &GraphPainter::graphSizeChangedHandle);
    connect(this, &GraphPainter::widthChanged, this, &GraphPainter::graphSizeChangedHandle);
}

void GraphPainter::paint(QPainter* _painter)
{
    // axis X
    _painter->drawLine(GRAPH_LEFT_MARGIN,            height() - GRAPH_BOTTOM_MARGIN,
                      width() - GRAPH_RIGHT_MARGIN, height() - GRAPH_BOTTOM_MARGIN);
    // axis Y
    _painter->drawLine(GRAPH_LEFT_MARGIN, height() - GRAPH_BOTTOM_MARGIN,
                      GRAPH_LEFT_MARGIN, GRAPH_BOTTOM_MARGIN);

    // paint div
    auto divYVal { (height() - GRAPH_BOTTOM_MARGIN - GRAPH_TOP_MARGIN) / GRAPH_DIV_Y };
    for (qint32 i {0}; i <= GRAPH_DIV_Y; ++i)
    {
        _painter->drawText(QRectF(GRAPH_LEFT_MARGIN - 132,
                                 height() - GRAPH_BOTTOM_MARGIN - i * divYVal - 8,
                                 GRAPH_LEFT_MARGIN - 12,
                                 height() - GRAPH_BOTTOM_MARGIN - i * divYVal + 4),
                          QString::number(m_minY + m_deltaY / (height() - GRAPH_BOTTOM_MARGIN - GRAPH_TOP_MARGIN) * i * divYVal,
                                          'e', 8));
    }

    auto divXVal { (width() - GRAPH_LEFT_MARGIN - GRAPH_RIGHT_MARGIN) / GRAPH_DIV_X };
    for (qint32 i {0}; i <= GRAPH_DIV_X; ++i)
    {
        _painter->drawText(QRectF(GRAPH_LEFT_MARGIN + i * divXVal - 24,
                                 height() - GRAPH_BOTTOM_MARGIN + 12,
                                 GRAPH_LEFT_MARGIN + i * divXVal,
                                 height() - GRAPH_BOTTOM_MARGIN + 20),
                          QString::number(m_minX + m_deltaX / (width() - GRAPH_LEFT_MARGIN - GRAPH_RIGHT_MARGIN) * i * divXVal,
                                          'e', 2));
    }

    // paint points ans lines
    _painter->setPen(QPen(Qt::blue, 3));
    _painter->drawPoints(m_points->data(), m_points->size());

    _painter->setPen(QPen(Qt::blue, 1));
    _painter->drawPolyline(m_points->data(), m_points->size());
}

void GraphPainter::graphSizeChangedHandle()
{
    if (m_points->size() > 0)
    {
        m_points->clear();
        update();
    }
}

void GraphPainter::updateData(const QSharedPointer<QVector<QPointF>>& _points)
{
    if (!_points)
        return;

    m_points->resize(_points->size());

    const qsizetype currentWatchersCount { _points->size() < GRAPH_PARALLEL_MIN_POINTS
                                        ? 1 : m_watchers.size()};
    m_watchersReadyCount = currentWatchersCount;
    for (qsizetype num {0}; num < currentWatchersCount; ++num)
    {
        QFuture<qsizetype> future = QtConcurrent::run([this, _points, currentWatchersCount](qsizetype _watcherNumber) -> qsizetype {
            if (!_points || !m_points || _points->size() != m_points->size())
            {
                qInfo() << "Get error in points copying";
                return 0;
            }

            qsizetype watcherPoints { _points->size() / currentWatchersCount };
            qsizetype copyStartIndex { _watcherNumber * watcherPoints };
            qsizetype copyStopIndex  { _watcherNumber < currentWatchersCount - 1
                                    ? (_watcherNumber + 1) * watcherPoints
                                    : _points->size()
                                  };

            for (qsizetype i {copyStartIndex}; i < copyStopIndex; ++i)
            {
                auto currentY = height() - GRAPH_BOTTOM_MARGIN
                                  - (_points->at(i).y() - m_minY) / m_deltaY
                                    * (height() - GRAPH_BOTTOM_MARGIN - GRAPH_TOP_MARGIN);
                auto currentX = GRAPH_LEFT_MARGIN
                                  + (_points->at(i).x() - m_minX) / m_deltaX
                                    * (width() - GRAPH_LEFT_MARGIN - GRAPH_RIGHT_MARGIN);

                (*m_points)[i].setX(currentX);
                (*m_points)[i].setY(currentY);
            }

            return copyStopIndex;
        }, num);

        auto currentWatcher = m_watchers[num];
        currentWatcher->setFuture(future);
    }

    qInfo() << "All watchers are started";
}

void GraphPainter::setMaxMinValues(const QPair<QPointF, QPointF>& _maxMinXYPair)
{
    m_deltaX = _maxMinXYPair.first.x() - _maxMinXYPair.second.x();
    m_deltaY = _maxMinXYPair.first.y() - _maxMinXYPair.second.y();

    m_minX = _maxMinXYPair.second.x();
    m_minY = _maxMinXYPair.second.y();

    qInfo() << "Fetch max and min values";
    qInfo() << "MIN: X " << m_minX << "\t Y " << m_minY;
    qInfo() << "MAX: X " << _maxMinXYPair.first.x() << "\t Y " << _maxMinXYPair.first.y();
}
