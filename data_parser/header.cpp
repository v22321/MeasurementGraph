#include "header.h"

Header::Header(const QString& _headerStr)
    : m_value(_headerStr)
{}

QString Header::value() const
{
    return m_value;
}

void Header::setValue(const QString &_value)
{
    if (m_value == _value) return;
    m_value = _value;
}
