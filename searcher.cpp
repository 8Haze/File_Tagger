#include "searcher.h"

#include <algorithm>

// ================================================================
// | Searcher Class - Private                                     |
// ================================================================

bool Searcher::legal_characters_in_search_string(const QString& search_string)
{
    auto legal_character = [](const QChar& character)
    {
        return (character >= '0' && character <= '9') ||
               (character >= 'a' && character <= 'z') ||
               (character >= 'A' && character <= 'Z') ||
               (character == '_') ||
               (character == '-') ||
               (character == ' ');
    };

    for (const auto& character : search_string)
        if (!legal_character(character))
            return false;

    return true;
}

// ================================================================
// | Searcher Class - Public                                      |
// ================================================================

bool Searcher::parse(QString search_string)
{
    reset();

    while (!search_string.isEmpty() && search_string.back() == ' ')
        search_string.chop(1);

    if (search_string.isEmpty())
        return true;

    if (!legal_characters_in_search_string(search_string))
        return false;

    std::reverse(search_string.begin(), search_string.end());

    while (!search_string.isEmpty())
    {
        QString tag;
        bool to_include = true;

        while (!search_string.isEmpty() && search_string.back() != ' ')
        {
            if (search_string.back() == '-')
            {
                if (tag.isEmpty())
                    to_include = false;
                else
                    return false;
            }
            else
                tag.push_back(search_string.back());

            search_string.chop(1);
        }

        if (!tag.isEmpty())
        {
            if (to_include)
                included_tags.push_back(std::move(tag));
            else
                excluded_tags.push_back(std::move(tag));
        }

        if (!search_string.isEmpty())
            search_string.chop(1);
    }

    return true;
}

std::vector<int> Searcher::search(const std::vector<File>& files)
{
    std::vector<int> to_hide;

    for (size_t a = 0; a < files.size(); ++a)
    {
        bool skip_rest = false;

        for (const auto& included_tag : included_tags)
        {
            if (!files[a].get_tags().contains(included_tag))
            {
                to_hide.push_back(a);
                skip_rest = true;
                break;
            }
        }

        if (skip_rest)
            continue;

        for (const auto& excluded_tag : excluded_tags)
        {
            if (files[a].get_tags().contains(excluded_tag))
            {
                to_hide.push_back(a);
                break;
            }
        }
    }

    return to_hide;
}

void Searcher::reset()
{
    included_tags.clear();
    excluded_tags.clear();
}
