#ifndef GRAPHPAINTER_H
#define GRAPHPAINTER_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QtConcurrent/QtConcurrent>

class GraphPainter : public QQuickPaintedItem
{
    Q_OBJECT

    const qint32 GRAPH_PARALLEL_MIN_POINTS {1'000};

    const qreal GRAPH_LEFT_MARGIN {150.};
    const qreal GRAPH_RIGHT_MARGIN {GRAPH_LEFT_MARGIN / 3};
    const qreal GRAPH_BOTTOM_MARGIN {50.};
    const qreal GRAPH_TOP_MARGIN {GRAPH_BOTTOM_MARGIN};

    const qint32 GRAPH_DIV_X {8};
    const qint32 GRAPH_DIV_Y {10};
public:
    GraphPainter(QQuickItem* _parent = nullptr);

    void paint(QPainter* _painter) override;
    /// Delete graph if window size is changed
    void graphSizeChangedHandle();

public slots:
    /// Update current graph with new _points
    void updateData(const QSharedPointer<QVector<QPointF>>& _points);
    /// Set max and min points (QPair< MAX point, MIN point >)
    void setMaxMinValues(const QPair<QPointF, QPointF>& _maxMinXYPair);

private:
    QSharedPointer<QVector<QPointF>> m_points;
    QVector<QSharedPointer<QFutureWatcher<qsizetype>>> m_watchers;
    qint32 m_watchersReadyCount;

    qreal m_deltaX;
    qreal m_deltaY;
    qreal m_minX;
    qreal m_minY;

signals:
    void s_graphUpdated();
};

#endif // GRAPHPAINTER_H
