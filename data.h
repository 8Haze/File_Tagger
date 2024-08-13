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

    QMap<QString, int>& get_tags() noexcept; // just forward from the model
    const QMap<QString, int>& get_tags() const noexcept; // just forward from the model

    ~Data() = default;

};

// tags_model - QMap<QString, int> m_tags;,
// files_model -

#endif // DATA_H
