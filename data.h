#ifndef DATA_H
#define DATA_H

#include <vector>

#include "file.h"

class Data : public QObject, public Serializable
{
private:

    Q_OBJECT

    QMap<QString, int> m_tags;
    std::vector<File> m_files;
    bool saved = true;

public:

    Data() = default;

    QMap<QString, int>& get_tags() noexcept;
    const QMap<QString, int>& get_tags() const noexcept;
    std::vector<File>& get_files() noexcept;
    const std::vector<File>& get_files() const noexcept;
    bool is_saved() const noexcept;

    void mark_as_saved() noexcept;

    virtual QVariant to_variant() const override;
    virtual void from_variant(const QVariant& variant) override;

    ~Data() = default;

};

Q_DECLARE_METATYPE(Data)

#endif // DATA_H
