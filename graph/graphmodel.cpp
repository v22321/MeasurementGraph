#include "graphmodel.h"
#include <QDebug>

GraphModel::GraphModel(const MeasureData &_startElement)
    : m_maxY(_startElement.getPoint().y()), m_maxX(_startElement.getPoint().x()),
    m_minY(_startElement.getPoint().y()), m_minX(_startElement.getPoint().x())
{}

void GraphModel::setMaxY(double _maxY)
{
    m_maxY = _maxY;
    qInfo() << "Max Y: " << m_maxY;
    // qInfo() << "EMIT OPE >>";
    // emit ope(m_data.x(), m_data.y());
}

double GraphModel::maxY()
{
    return m_maxY;
}

void GraphModel::setMaxX(double _maxX)
{
    m_maxX = _maxX;
    qInfo() << "Max X: " << m_maxX;
}

double GraphModel::maxX()
{
    return m_maxX;
}

void GraphModel::setMinY(double _minY)
{
    m_minY = _minY;
    qInfo() << "Min Y: " << m_minY;
}

double GraphModel::minY()
{
    return m_minY;
}

void GraphModel::setMinX(double _minX)
{
    m_minX = _minX;
    qInfo() << "Min X: " << m_minX;
}

double GraphModel::minX()
{
    return m_minX;
}

void GraphModel::handleNewPoint(const QPointF &point)
{
    emit newPointAdded(point);
}

void GraphModel::addNewPoint(const QPointF& point)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data.push_back(point);
    // m_data = point;
    endInsertRows();

    emit ope(1, 1);
}

void GraphModel::addData(const QVector<MeasureData> &_measureData)
{
    const long long MAX_WIDTH_POINTS { 2'000 };
    const long long measureDataSize { _measureData.size() };
    const long long stepSize { measureDataSize > MAX_WIDTH_POINTS ? measureDataSize / MAX_WIDTH_POINTS : 1 };

    double averageValue { 0.0 };
    uint32_t stepNumber { 1 };
    for (int32_t index = 0; index < measureDataSize; ++index)
    {
        QPointF currPoint {_measureData[index].getPoint()};
        if (stepSize > 1)
        {
            averageValue += currPoint.y();

            if (index < stepSize * stepNumber - 1)
                continue;

            currPoint.setY(averageValue / double(stepSize));
            currPoint.setX(_measureData[stepSize * (stepNumber - 1)].getPoint().x());

            averageValue = 0;
            ++stepNumber;
        }

        addNewPoint(currPoint);

        if (currPoint.x() > maxX()) setMaxX(currPoint.x());
        if (currPoint.y() > maxY()) setMaxY(currPoint.y());
        if (currPoint.x() < minX()) setMinX(currPoint.x());
        if (currPoint.y() < minY()) setMinY(currPoint.y());
    }
}

int GraphModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size();
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
        return m_data[index.row()].x();
    else
        return m_data[index.row()].y();
}
