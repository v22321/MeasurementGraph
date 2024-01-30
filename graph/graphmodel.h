#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include "../data_parser/measuredata.h"
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

    Q_PROPERTY(double maxY READ maxY NOTIFY borderChanged)
    Q_PROPERTY(double maxX READ maxX NOTIFY borderChanged)
    Q_PROPERTY(double minY READ minY NOTIFY borderChanged)
    Q_PROPERTY(double minX READ minX NOTIFY borderChanged)
    Q_PROPERTY(QString measurementInfo READ measurementInfo NOTIFY measurementInfoChanged)

public:
    // GraphModel();
    GraphModel();
    void setStartPoint(const QPointF& _point);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // Setters and getters
    void setMaxY(const double _maxY);
    double maxY() const;
    void setMaxX(const double _maxX);
    double maxX() const;
    void setMinY(const double _minY);
    double minY() const;
    void setMinX(const double _minX);
    double minX() const;

    QString measurementInfo();
    void addInformation(const QVector<Header>& _headers);

    void addPoint(const QPointF& point);
    void resetPoints(QVector<QPointF> _measureData);
    void addPoints(const QVector<MeasureData>& _measureData);

private:
    QVector<QPointF> m_points;
    QVector<Header> m_headers;

    QStringList m_fileList;
    double m_maxY;
    double m_minY;
    double m_maxX;
    double m_minX;

signals:
    void newPointAdded(const QPointF& point);
    void pointsChanged();
    void borderChanged();
    void measurementInfoChanged();
};


#endif // GRAPHMODEL_H
