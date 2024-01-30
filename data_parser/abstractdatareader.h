#ifndef ABSTRACTDATAREADER_H
#define ABSTRACTDATAREADER_H

#include "header.h"
#include "measuredata.h"

#include <QVector>
#include <QFile>

///
/// \brief The AbstractDataReader class - File reader interface
///
class AbstractDataReader {
public:
    explicit AbstractDataReader(const QString& _filePath);
    virtual ~AbstractDataReader();

    QVector<Header> headers() const;
    QVector<MeasureData> measurements() const;

    ///
    /// \brief readData - Read file's data
    /// \return error
    ///
    bool readData();

protected:
    ///
    /// \brief parseLines - Parse file's lines
    /// \param _file
    /// \return error
    ///
    virtual bool parseLines(QFile& _file) = 0;

protected:
    const QString m_filePath;

    QVector<Header> m_headers;
    QVector<MeasureData> m_measurements;
};

#endif // ABSTRACTDATAREADER_H
