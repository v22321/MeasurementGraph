#include "graphpainter.h"

GraphPainter::GraphPainter(QQuickItem* _parent)
    : QQuickPaintedItem(_parent),
      m_watchersReadyCount(0)
{
    m_watchers.reserve(QThread::idealThreadCount() - 1);
    for (qint32 threadNum {0}; threadNum < QThread::idealThreadCount() - 1; ++threadNum)
    {
        m_watchers.emplace_back(QSharedPointer<QFutureWatcher<size_t>>::create());

        auto currentWatcher = m_watchers[threadNum];
        connect (currentWatcher.data(), &QFutureWatcher<void>::finished, this, [this, currentWatcher]()
        {
            qInfo() << "Result: " << currentWatcher->result();
            update();
            if (++m_watchersReadyCount == QThread::idealThreadCount() - 1)
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
                                 height() - GRAPH_BOTTOM_MARGIN - 8 - i * divYVal,
                                 GRAPH_LEFT_MARGIN - 12,
                                 height() - GRAPH_BOTTOM_MARGIN + 4 - i * divYVal),
                          QString::number(m_minY + m_deltaY / (height() - GRAPH_BOTTOM_MARGIN - GRAPH_TOP_MARGIN) * i * divYVal,
                                          'e', 8));
    }

    auto divXVal { (width() - GRAPH_BOTTOM_MARGIN - GRAPH_TOP_MARGIN) / GRAPH_DIV_X };
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
    _painter->drawPoints(m_points.data(), m_points.size());

    _painter->setPen(QPen(Qt::blue, 1));
    _painter->drawPolyline(m_points.data(), m_points.size());
}

void GraphPainter::graphSizeChangedHandle()
{
    if (m_points.size() > 0)
    {
        m_points.clear();
        update();
    }
}

void GraphPainter::updateData(const QSharedPointer<QVector<QPointF>>& _points)
{
    if (!_points)
        return;

    m_points.resize(_points->size());
    m_watchersReadyCount = 0;

    for (size_t num {0}; num < m_watchers.size(); ++num)
    {
        QFuture<size_t> future = QtConcurrent::run([this, _points](size_t _watcherNumber) -> size_t {
            if (!_points || _points->size() == 0)
            {
                qInfo() << "Empty point list";
                return -1;
            }

            size_t watcherPoints { _points->size() / m_watchers.size() };
            size_t copyIndexStart { _watcherNumber * watcherPoints };
            size_t copyIndexStop  { _watcherNumber < m_watchers.size() - 1
                                    ? (_watcherNumber + 1) * watcherPoints
                                    : _points->size()
                                  };

            for (size_t i {copyIndexStart}; i < copyIndexStop; ++i)
            {
                double currentY = height() - GRAPH_BOTTOM_MARGIN - (_points->at(i).y() - m_minY) / m_deltaY  * (height() - GRAPH_BOTTOM_MARGIN - GRAPH_TOP_MARGIN);
                double currentX = (_points->at(i).x() - m_minX) / m_deltaX  * (width() - GRAPH_LEFT_MARGIN - GRAPH_RIGHT_MARGIN) + GRAPH_LEFT_MARGIN;

                m_points[i].setX(currentX);
                m_points[i].setY(currentY);
            }

            return copyIndexStop;
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
