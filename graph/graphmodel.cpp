#include "graphmodel.h"
#include <QDebug>

GraphModel::GraphModel(const MeasureData &_startElement)
    : m_maxY(_startElement.getPoint().y()), m_maxX(_startElement.getPoint().x()),
    m_minY(_startElement.getPoint().y()), m_minX(_startElement.getPoint().x())
{}

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
}

double GraphModel::maxY() const
{
    return m_maxY;
}

void GraphModel::setMaxX(const double _maxX)
{
    if (m_maxX == _maxX) return;
    m_maxX = _maxX;
}

double GraphModel::maxX() const
{
    return m_maxX;
}

void GraphModel::setMinY(const double _minY)
{
    if (m_minY == _minY) return;
    m_minY = _minY;
}

double GraphModel::minY() const
{
    return m_minY;
}

void GraphModel::setMinX(const double _minX)
{
    if (m_minX == _minX) return;
    m_minX = _minX;
}

double GraphModel::minX() const
{
    return m_minX;
}

void GraphModel::addPoint(const QPointF& point)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_points.push_back(point);
    endInsertRows();
    emit pointsChanged();
}

void GraphModel::addPoints(const QVector<MeasureData> &_measureData)
{
    const size_t measureDataSize { _measureData.size() };
    /// If we have many points, we need to calculate the average
    const size_t stepSize { measureDataSize > MAX_WIDTH_POINTS ? measureDataSize / MAX_WIDTH_POINTS : 1 };

    double averageValue { 0.0 };
    uint32_t stepNumber { 1 };
    for (size_t index = 0; index < measureDataSize; ++index)
    {
        QPointF currPoint { _measureData[index].getPoint() };
        /// If we have many points, we need to calculate the average
        if (stepSize > 1)
        {
            averageValue += currPoint.y();

            if (index < stepSize * stepNumber - 1)
                continue;

            currPoint.setY(averageValue / stepSize);
            currPoint.setX(_measureData[stepSize * (stepNumber - 1)].getPoint().x());

            averageValue = 0;
            ++stepNumber;
        }

        addPoint(currPoint);

        /// Update max/min values
        if (currPoint.x() > maxX()) setMaxX(currPoint.x());
        if (currPoint.y() > maxY()) setMaxY(currPoint.y());
        if (currPoint.x() < minX()) setMinX(currPoint.x());
        if (currPoint.y() < minY()) setMinY(currPoint.y());
    }
}
