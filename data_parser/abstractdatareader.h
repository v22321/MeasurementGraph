#ifndef ABSTRACTDATAREADER_H
#define ABSTRACTDATAREADER_H

#include <QVector>
#include <QDebug>
#include "header.h"
#include "measuredata.h"

class AbstractDataReader {
public:
    AbstractDataReader(const QString& _filePath) : m_filePath(_filePath)
    { qInfo() << "FILENAME: " << m_filePath; }

    virtual void readData() = 0;

    virtual QVector<Header> headers() const = 0;
    virtual QVector<MeasureData> measurements() const = 0;

protected:
    const QString m_filePath;
};

#endif // ABSTRACTDATAREADER_H
