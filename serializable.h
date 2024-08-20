#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QVariant>

class Serializable
{
public:

    virtual QVariant to_variant() const = 0;
    virtual void from_variant(const QVariant& variant) = 0;

    virtual ~Serializable() = default;

};

#endif // SERIALIZABLE_H
