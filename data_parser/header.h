#ifndef HEADER_H
#define HEADER_H

#include <QString>

class Header
{
public:
    Header(const QString& _headerStr);

    QString value() const;
    void setValue(const QString& _value);

private:
    QString m_value;
};

#endif // HEADER_H
