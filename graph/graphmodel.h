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

    Q_PROPERTY(long double maxY READ maxY NOTIFY borderChanged)
    Q_PROPERTY(long double maxX READ maxX NOTIFY borderChanged)
    Q_PROPERTY(long double minY READ minY NOTIFY borderChanged)
    Q_PROPERTY(long double minX READ minX NOTIFY borderChanged)
    Q_PROPERTY(QString measurementInfo READ measurementInfo NOTIFY measurementInfoChanged)

public:
    GraphModel();
    void setMaxMinPoints(const QPair<QPointF, QPointF>& _maxMinXYPair);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // Setters and getters
    void setMaxY(const long double _maxY);
    long double maxY() const;
    void setMaxX(const long double _maxX);
    long double maxX() const;
    void setMinY(const long double _minY);
    long double minY() const;
    void setMinX(const long double _minX);
    long double minX() const;

    QString measurementInfo();
    void addInformation(const QVector<Header>& _headers);

    void addPoint(const QPointF& point);
    void resetPoints(QVector<QPointF> _measureData);

private:
    QVector<QPointF> m_points;
    QVector<Header> m_headers;

    QStringList m_fileList;
    long double m_maxY;
    long double m_minY;
    long double m_maxX;
    long double m_minX;

signals:
    void newPointAdded(const QPointF& point);
    void pointsChanged();
    void borderChanged();
    void measurementInfoChanged();
};


#endif // GRAPHMODEL_H
