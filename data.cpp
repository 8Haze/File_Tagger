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
