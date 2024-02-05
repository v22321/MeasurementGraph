#include "graphmodel.h"

GraphModel::GraphModel()
    : m_fileList(""), m_maxY(0.), m_minY(0.), m_maxX(0.), m_minX(0.)
{}

void GraphModel::setMaxMinPoints(const QPair<QPointF, QPointF>& _maxMinXYPair)
{
    m_maxX = _maxMinXYPair.first.x();
    m_maxY = _maxMinXYPair.first.y();
    m_minX = _maxMinXYPair.second.x();
    m_minY = _maxMinXYPair.second.y();
    emit borderChanged();
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


void GraphModel::setMaxY(const qreal _maxY)
{
    if (m_maxY == _maxY) return;
    m_maxY = _maxY;
    emit borderChanged();
}

qreal GraphModel::maxY() const
{
    return m_maxY;
}

void GraphModel::setMaxX(const qreal _maxX)
{
    if (m_maxX == _maxX) return;
    m_maxX = _maxX;
    emit borderChanged();
}

qreal GraphModel::maxX() const
{
    return m_maxX;
}

void GraphModel::setMinY(const qreal _minY)
{
    if (m_minY == _minY) return;
    m_minY = _minY;
    emit borderChanged();
}

qreal GraphModel::minY() const
{
    return m_minY;
}

void GraphModel::setMinX(const qreal _minX)
{
    if (m_minX == _minX) return;
    m_minX = _minX;
    emit borderChanged();
}

qreal GraphModel::minX() const
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
    emit pointsChanged();
}
