#include "file.h"

// ================================================================
// | File Class - Constructors                                    |
// ================================================================

File::File(const QString& path, QMap<QString, int>& tags_map) :
    m_path(path),
    m_tags_map(tags_map) {}

File::File(File&& other) :
    m_path(std::move(other.m_path)),
    m_tags(std::move(other.m_tags)),
    m_tags_map(other.m_tags_map) {}

// ================================================================
// | File Class - Assignment Operators                            |
// ================================================================

File& File::operator=(File&& other)
{
    m_path = std::move(other.m_path);
    m_tags = std::move(other.m_tags);

    return *this;
}

// ================================================================
// | File Class - Public                                          |
// ================================================================

const QString& File::get_path() const noexcept
{
    return m_path;
}

const QSet<QString>& File::get_tags() const noexcept
{
    return m_tags;
}

void File::set_path(const QString& path)
{
    m_path = path;
}

void File::add_tag(const QString& tag)
{
    if (!m_tags.contains(tag))
    {
        m_tags.insert(tag);
        ++m_tags_map[tag];
    }
}

void File::update_tag(const QString& old_tag, const QString& new_tag)
{
    m_tags.remove(old_tag);
    m_tags.insert(new_tag);
}

void File::remove_tag(const QString& tag)
{
    if (m_tags.remove(tag))
        --m_tags_map[tag];
}

QString File::to_string() const
{
    QString result = m_path + " --- ";

    for (const auto& tag : m_tags)
    {
        result += tag;
        result += ", ";
    }

    if (m_tags.isEmpty())
        result += "(untagged)";
    else
        result.chop(2);

    return result;
}
