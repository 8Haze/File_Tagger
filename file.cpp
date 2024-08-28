#include "file.h"

#include <QFileInfo>

// ================================================================
// | File Class - Constructors                                    |
// ================================================================

File::File(const QString& path, QMap<QString, int>& tags_map) :
    QObject(nullptr),
    m_path(path),
    m_tags_map(tags_map) {}

File::File(File&& other) :
    QObject(nullptr),
    m_path(std::move(other.m_path)),
    m_tags(std::move(other.m_tags)),
    m_tags_map(other.m_tags_map),
    m_thumbnail(std::move(other.m_thumbnail)),
    m_thumbnail_attempted(other.m_thumbnail_attempted) {}

// ================================================================
// | File Class - Assignment Operators                            |
// ================================================================

File& File::operator=(File&& other)
{
    m_path = std::move(other.m_path);
    m_tags = std::move(other.m_tags);
    m_thumbnail = std::move(other.m_thumbnail);
    m_thumbnail_attempted = other.m_thumbnail_attempted;

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

const std::unique_ptr<QImage>& File::get_thumbnail() const noexcept
{
    return m_thumbnail;
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

void File::generate_thumbnail(const QSize& size)
{
    if (m_thumbnail_attempted)
        return;

    m_thumbnail_attempted = true;

    QFileInfo file_info(m_path);

    if (const auto& ext = file_info.suffix(); ext == "jpg" || ext == "png" || ext == "jpeg" || ext == "gif" || ext == "jfif")
    {
        QImage picture;
        if (picture.load(m_path))
            m_thumbnail = std::make_unique<QImage>(picture.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

QVariant File::to_variant() const
{
    QVariantMap map;
    QVariantList list;

    map.insert("file_path", m_path);

    for (const auto& tag : m_tags)
        list.append(tag);

    map.insert("tags", list);

    return map;
}

void File::from_variant(const QVariant& variant)
{
    QVariantMap map = variant.toMap();
    QVariantList list = map.value("tags").toList();

    m_path = map.value("file_path").toString();

    for (const auto& tag : list)
        add_tag(tag.toString());
}
