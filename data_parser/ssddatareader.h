#ifndef SSDDATAREADER_H
#define SSDDATAREADER_H

#include <QFile>

#include "abstractdatareader.h"

///
/// \brief The SsdDataReader class - Ssd files reader
///
class SsdDataReader : public AbstractDataReader
{
public:
    using AbstractDataReader::AbstractDataReader;

protected:
    bool parseLines(QFile& _file) override;

};

#endif // SSDDATAREADER_H
