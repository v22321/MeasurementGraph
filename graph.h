#ifndef GRAPH_H
#define GRAPH_H
#include <QQuickPaintedItem>
#include <QPainter>
#include <vector>
#include <QtConcurrent/QtConcurrent>
#include <vector>

class Graph : public QQuickPaintedItem
{
    Q_OBJECT
public:
    Graph(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;
    void contentsSizeChangedHandle();

public slots:
    void updateData(QSharedPointer<QVector<QPointF>>);
    void setBorders(const QPair<QPointF, QPointF> &_maxMinXYPair);

private:
    QVector<QPointF> points;
    QVector<QPointF> _pointsTmp;
    std::vector<QPointF> points1;
    std::vector<QPointF> points2;

    QVector<QPointF> resultPoints;
    // int num {0};

    std::vector<QFutureWatcher<int>> watchers;

    long double kX {1};
    long double xMin{0};
    long double kY {1};
    long double yMin{0};
};

#endif // GRAPH_H
