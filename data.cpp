#include "data.h"

// ================================================================
// | Data Class - Public                                          |
// ================================================================

QMap<QString, int>& Data::get_tags() noexcept
{
    saved = false;
    return m_tags;
}

const QMap<QString, int>& Data::get_tags() const noexcept
{
    return m_tags;
}

std::vector<File>& Data::get_files() noexcept
{
    saved = false;
    return m_files;
}

const std::vector<File>& Data::get_files() const noexcept
{
    return m_files;
}

bool Data::is_saved() const noexcept
{
    return saved;
}

void Data::mark_as_saved() noexcept
{
    saved = true;
}

QVariant Data::to_variant() const
{
    QVariantList list;

    for (const auto& file : m_files)
        list.append(file.to_variant());

    return list;
}

void Data::from_variant(const QVariant& variant)
{
    m_tags.clear();
    m_files.clear();

    QVariantList list = variant.toList();

    for (const QVariant& item : list)
    {
        m_files.emplace_back("", m_tags);
        m_files.back().from_variant(item);
    }
}
