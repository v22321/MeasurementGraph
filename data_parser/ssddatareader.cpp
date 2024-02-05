#include "ssddatareader.h"
#include "header.h"
#include "measuredata.h"

#include <QDebug>

bool SsdDataReader::parseLines(QFile& _file)
{
    if (!_file.isOpen())
        return true;

    while (!_file.atEnd())
    {
        QString lineStr { QString::fromUtf8(_file.readLine()) };
        // ???
        if (lineStr.isEmpty() || lineStr.startsWith(" ") || lineStr.startsWith("\n"))
        {
            continue;
        }

        /// Fill headers
        if (lineStr.startsWith("#"))
        {
            m_headers.emplace_back(lineStr);
            continue;
        }

        /// Fill measurement results
        std::istringstream strStream(lineStr.toStdString());
        qreal currentNumber;
        QVector<qreal> currentMeasurements;
        while (strStream >> currentNumber)
        {
            currentMeasurements.emplace_back(currentNumber);
            if (currentMeasurements.size() > 2)
            {
                qWarning() << "Error with measurement results";
                return true;
            }

            if (strStream.eof())
                break;
        }

        if ((!strStream.eof() && strStream.fail()) || currentMeasurements.size() != 2) {
            qWarning() << "Error with measurement results";
            return true;
        }

        m_measurements.emplace_back(currentMeasurements[0], currentMeasurements[1]);
    }

    return false;
}
