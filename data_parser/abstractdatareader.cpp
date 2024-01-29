#include "abstractdatareader.h"

#include <QDebug>

AbstractDataReader::AbstractDataReader(const QString &_filePath)
    : m_filePath(_filePath),
      m_headers({}),
      m_measurements({})
{
}

QVector<Header> AbstractDataReader::headers() const
{
    return m_headers;
}

QVector<MeasureData> AbstractDataReader::measurements() const
{
    return m_measurements;
}

bool AbstractDataReader::readData()
{
    bool resultErr {true};
    QFile file(m_filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo() << "Open file: " << m_filePath;
        resultErr = parseLines(file);
        file.close();
    }
    else
    {
        qInfo() << "Open file error: " << m_filePath;
    }

    return resultErr;
}
