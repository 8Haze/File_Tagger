#ifndef SEARCHER_H
#define SEARCHER_H

#include <vector>

#include <QList>

#include "file.h"

// an option to quickly search for all untagged ones?

class Searcher
{
private:

    std::vector<QString> included_tags;
    std::vector<QString> excluded_tags;

    bool legal_characters_in_search_string(const QString& search_string);

public:

    Searcher() = default;

    bool parse(QString search_string);
    std::vector<int> search(const std::vector<File>& files);
    void reset();

    ~Searcher() = default;

};

#endif // SEARCHER_H
