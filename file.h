#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QMap>

#include "serializable.h"

class File : public QObject, public Serializable
{
private:

    Q_OBJECT

    QString m_path;
    QSet<QString> m_tags;
    QMap<QString, int>& m_tags_map;
    //std::unique_ptr<QImage> m_thumbnail; // optional

public:

    File(const QString& path, QMap<QString, int>& tags_map);
    File(File&& other);

    File(const File&) = delete;

    File& operator=(File&& other);

    File& operator=(const File&) = delete;

    const QString& get_path() const noexcept;
    const QSet<QString>& get_tags() const noexcept;

    void set_path(const QString& path);

    void add_tag(const QString& tag);
    void update_tag(const QString& old_tag, const QString& new_tag);
    void remove_tag(const QString& tag);

    QString to_string() const;

    virtual QVariant to_variant() const override;
    virtual void from_variant(const QVariant& variant) override;

    ~File() = default;

};

Q_DECLARE_METATYPE(File)

#endif // FILE_H
