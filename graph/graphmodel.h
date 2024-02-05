#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include "../data_parser/header.h"

#include <QAbstractTableModel>
#include <QPointF>
#include <QVector>

///
/// \brief The GraphModel class - Graph data model
///
class GraphModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(qreal maxY READ maxY NOTIFY borderChanged)
    Q_PROPERTY(qreal maxX READ maxX NOTIFY borderChanged)
    Q_PROPERTY(qreal minY READ minY NOTIFY borderChanged)
    Q_PROPERTY(qreal minX READ minX NOTIFY borderChanged)
    Q_PROPERTY(QString measurementInfo READ measurementInfo NOTIFY measurementInfoChanged)

public:
    GraphModel();
    void setMaxMinPoints(const QPair<QPointF, QPointF>& _maxMinXYPair);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // Setters and getters
    void setMaxY(const qreal _maxY);
    qreal maxY() const;
    void setMaxX(const qreal _maxX);
    qreal maxX() const;
    void setMinY(const qreal _minY);
    qreal minY() const;
    void setMinX(const qreal _minX);
    qreal minX() const;

    QString measurementInfo();
    void addInformation(const QVector<Header>& _headers);

    void addPoint(const QPointF& point);
    void resetPoints(QVector<QPointF> _measureData);

private:
    QVector<QPointF> m_points;
    QVector<Header> m_headers;

    QStringList m_fileList;
    qreal m_maxY;
    qreal m_minY;
    qreal m_maxX;
    qreal m_minX;

signals:
    void newPointAdded(const QPointF& point);
    void pointsChanged();
    void borderChanged();
    void measurementInfoChanged();
};


#endif // GRAPHMODEL_H
