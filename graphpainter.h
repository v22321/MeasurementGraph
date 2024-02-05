#ifndef GRAPHPAINTER_H
#define GRAPHPAINTER_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QtConcurrent/QtConcurrent>

class GraphPainter : public QQuickPaintedItem
{
    Q_OBJECT

    const qint32 GRAPH_PARALLEL_MIN_POINTS {1'000};

    const qint32 GRAPH_LEFT_MARGIN {150};
    const qint32 GRAPH_RIGHT_MARGIN {GRAPH_LEFT_MARGIN / 3};
    const qint32 GRAPH_BOTTOM_MARGIN {50};
    const qint32 GRAPH_TOP_MARGIN {GRAPH_BOTTOM_MARGIN};

    const qint32 GRAPH_DIV_X {8};
    const qint32 GRAPH_DIV_Y {10};
public:
    GraphPainter(QQuickItem* _parent = nullptr);

    void paint(QPainter* _painter) override;
    /// Delete graph if window size is changed
    void graphSizeChangedHandle();

public slots:
    void updateData(const QSharedPointer<QVector<QPointF>>& _points);
    void setMaxMinValues(const QPair<QPointF, QPointF>& _maxMinXYPair);

private:
    QSharedPointer<QVector<QPointF>> m_points;
    QVector<QSharedPointer<QFutureWatcher<size_t>>> m_watchers;
    qint32 m_watchersReadyCount;

    double m_deltaX;
    double m_deltaY;
    double m_minX;
    double m_minY;

signals:
    void s_graphUpdated();
};

#endif // GRAPHPAINTER_H
