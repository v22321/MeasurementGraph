#include "ssddatareader.h"
#include "header.h"
#include "measuredata.h"

void SsdDataReader::readData()
{
    QFile file(m_filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo() << "Open file successfull";
        parseLines(file);
        file.close();
    }
    else
    {
        qInfo() << "Open file error!";
    }
}

QVector<Header> SsdDataReader::headers() const
{
    return m_headers;
}

QVector<MeasureData> SsdDataReader::measurements() const
{
    return m_measurements;
}

void SsdDataReader::parseLines(QFile& _file)
{
    qInfo() << "ALA";


    // // Создание потока ввода с использованием строки
    // std::istringstream iss(inputString);

    // // Извлечение слов из потока ввода и добавление их в вектор
    // std::string word;
    // while (iss >> word) {
    //     // words.push_back(word);
    //     qDebug() << word;
    // }


    // qInfo() << _file.atEnd();
    // Читаем файл построчно до конца
    while (!_file.atEnd())
    {
        QByteArray line = _file.readLine();
        QString lineStr {QString::fromUtf8(line)};
        // qInfo() << "TYTA>> " << lineStr;
        // Создание потока ввода с использованием строки

        if (lineStr.isEmpty() || lineStr.startsWith(" ") || lineStr.startsWith("\n"))
        {
            continue;
        }

        if (lineStr.startsWith("#"))
        {
            // qDebug() << lineStr;
            m_headers.emplace_back(lineStr);
            continue;
        }

        std::istringstream iss(line.toStdString());
        // Извлечение слов из потока ввода и добавление их в вектор
        double word;
        double words[2];
        int i = 0;
        while (iss >> word) {
            // words.push_back(word);
            // qDebug() << word;
            words[i++] = word;
        }

        m_measurements.emplace_back(words[0], words[1]);
    }
}
