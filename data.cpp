#include "data.h"

// ================================================================
// | Data Class - Public                                          |
// ================================================================

QMap<QString, int>& Data::get_tags() noexcept
{
    return m_tags;
}

const QMap<QString, int>& Data::get_tags() const noexcept
{
    return m_tags;
}

std::vector<File>& Data::get_files() noexcept
{
    return m_files;
}

const std::vector<File>& Data::get_files() const noexcept
{
    return m_files;
}

/*std::vector<size_t>& Data::get_viewed_files_indexes() noexcept
{
    return m_viewed_files_indexes;
}

const std::vector<size_t>& Data::get_viewed_files_indexes() const noexcept
{
    return m_viewed_files_indexes;
}*/
