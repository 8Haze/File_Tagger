#include "binary_serializer.h"

#include <QFile>

// ================================================================
// | Binary_Serializer Class - Public                             |
// ================================================================

bool Binary_Serializer::save(const Serializable& serializable, const QString& file_path)
{
    QFile file(file_path);
    file.open(QFile::WriteOnly);

    if (!file.isOpen())
        return false;

    QDataStream data_stream(&file);
    data_stream << serializable.to_variant();

    file.close();

    return true;
}

bool Binary_Serializer::load(Serializable& serializable, const QString& file_path)
{
    QFile file(file_path);
    file.open(QFile::ReadOnly);

    if (!file.isOpen())
        return false;

    QDataStream data_stream(&file);
    QVariant variant;
    data_stream >> variant;

    serializable.from_variant(variant);

    file.close();

    return true;
}
