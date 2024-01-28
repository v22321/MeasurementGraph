#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include <QAbstractTableModel>
#include <QPointF>
#include <QVector>
#include "../data_parser/measuredata.h"

class GraphModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(double maxY READ maxY NOTIFY ope)
    Q_PROPERTY(double maxX READ maxX NOTIFY ope)
    Q_PROPERTY(double minY READ minY NOTIFY ope)
    Q_PROPERTY(double minX READ minX NOTIFY ope)
public:
    void setMaxY(double _maxY);
    double maxY();
    void setMaxX(double _maxX);
    double maxX();

    void setMinY(double _minY);
    double minY();
    void setMinX(double _minX);
    double minX();


    void handleNewPoint(const QPointF& point);
    void addNewPoint(const QPointF& point);

    void addData(const QVector<MeasureData>& _measureData);

signals:
    void newPointAdded(const QPointF& point);
    void ope(double a, double b);
    void ope2(double a, double b);
public:
    GraphModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QVector<QPointF> m_data;
    // QPointF m_data;
    double m_maxY;
    double m_minY;
    double m_maxX;
    double m_minX;
};


#endif // GRAPHMODEL_H
