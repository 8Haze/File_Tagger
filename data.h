#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QMap>

// serialization, initialization...

class Data
{
private:

    QMap<QString, int> m_tags;

public:

    Data() = default;

    QMap<QString, int>& get_tags() noexcept;
    const QMap<QString, int>& get_tags() const noexcept;

    ~Data() = default;

};

#endif // DATA_H
