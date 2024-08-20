#ifndef BINARY_SERIALIZER_H
#define BINARY_SERIALIZER_H

#include "serializable.h"

class Binary_Serializer
{
public:

    Binary_Serializer() = default;

    virtual bool save(const Serializable& serializable, const QString& file_path);
    virtual bool load(Serializable& serializable, const QString& file_path);

    ~Binary_Serializer() = default;

};

#endif // BINARY_SERIALIZER_H
