#ifndef SSDDATAREADER_H
#define SSDDATAREADER_H

#include "abstractdatareader.h"
#include <QDebug>
#include <QString>
#include <QFile>


class SsdDataReader : public AbstractDataReader
{
public:
    using AbstractDataReader::AbstractDataReader;

    void readData() override;

    QVector<Header> headers() const override;
    QVector<MeasureData> measurements() const override;

private:
    void parseLines(QFile& _file);

private:
    QVector<Header> m_headers;
    QVector<MeasureData> m_measurements;

};

#endif // SSDDATAREADER_H
