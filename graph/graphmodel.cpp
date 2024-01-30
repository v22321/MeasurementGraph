#include "graphmodel.h"
#include "ipointsadapter.h"
#include "basepointsadapter.h"
#include "averagepointsadapter.h"

GraphModel::GraphModel()
    : m_maxY(0.), m_maxX(0.), m_minY(0.), m_minX(0.)
{}

void GraphModel::setStartPoint(const QPointF &_point)
{
    m_maxY = _point.y();
    m_maxX = _point.x();
    m_minY = _point.y();
    m_minX = _point.x();
}

int GraphModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_points.size();
}

int GraphModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant GraphModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    Q_UNUSED(role)

    if(section == 0)
        return "x";
    else
        return "y";
}

QVariant GraphModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)

    if (index.column() == 0)
        return m_points[index.row()].x();
    else
        return m_points[index.row()].y();
}


void GraphModel::setMaxY(const double _maxY)
{
    if (m_maxY == _maxY) return;
    m_maxY = _maxY;
    emit borderChanged();
}

double GraphModel::maxY() const
{
    return m_maxY;
}

void GraphModel::setMaxX(const double _maxX)
{
    if (m_maxX == _maxX) return;
    m_maxX = _maxX;
    emit borderChanged();
}

double GraphModel::maxX() const
{
    return m_maxX;
}

void GraphModel::setMinY(const double _minY)
{
    if (m_minY == _minY) return;
    m_minY = _minY;
    emit borderChanged();
}

double GraphModel::minY() const
{
    return m_minY;
}

void GraphModel::setMinX(const double _minX)
{
    if (m_minX == _minX) return;
    m_minX = _minX;
    emit borderChanged();
}

double GraphModel::minX() const
{
    return m_minX;
}

QString GraphModel::measurementInfo()
{
    QString allHeadersStr;

    std::for_each(m_headers.cbegin(), m_headers.cend(), [&allHeadersStr](const auto& el) {
        allHeadersStr += el.value() += "<br>";
    });

    return allHeadersStr;
}

void GraphModel::addInformation(const QVector<Header>& _headers)
{
    if (m_headers == _headers) return;
    m_headers = _headers;
    emit measurementInfoChanged();
}

void GraphModel::addPoint(const QPointF& point)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_points.push_back(point);
    endInsertRows();
    emit pointsChanged();
}

void GraphModel::resetPoints(QVector<QPointF> _measureData)
{
    beginResetModel();
    m_points.swap(_measureData);
    endResetModel();

    if (m_points.size() > 0)
    {
        setStartPoint(m_points[0]);
        for (const auto& point : m_points)
        {
            if (point.x() > maxX()) setMaxX(point.x());
            if (point.y() > maxY()) setMaxY(point.y());
            if (point.x() < minX()) setMinX(point.x());
            if (point.y() < minY()) setMinY(point.y());
        }
    }
    emit pointsChanged();
}
