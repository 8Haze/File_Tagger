#ifndef DATA_H
#define DATA_H

#include <vector>

#include "file.h"

// serialization, initialization...

class Data
{
private:

    QMap<QString, int> m_tags;
    std::vector<File> m_files;
    //std::vector<size_t> m_viewed_files_indexes;

public:

    Data() = default;

    QMap<QString, int>& get_tags() noexcept;
    const QMap<QString, int>& get_tags() const noexcept;

    std::vector<File>& get_files() noexcept;
    const std::vector<File>& get_files() const noexcept;

    //std::vector<size_t>& get_viewed_files_indexes() noexcept;
    //const std::vector<size_t>& get_viewed_files_indexes() const noexcept;

    ~Data() = default;

};

#endif // DATA_H
