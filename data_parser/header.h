#ifndef HEADER_H
#define HEADER_H

#include <QString>

class Header
{
public:
    Header(const QString& _headerStr);
    bool operator ==(const Header& _other) const{
        if (value() != _other.value()) return false;

        return true;
    }

    QString value() const;
    void setValue(const QString& _value);

private:
    QString m_value;
};

#endif // HEADER_H
