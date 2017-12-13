#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <list>

int find_connected_to(const std::vector<std::vector<int>>& mini_groups, int id)
{
    std::list<std::set<int>> connected_groups;
    auto find_all_containing = [&connected_groups](int n) {
        std::vector<std::list<std::set<int>>::iterator> all_found;
        for (auto it{ connected_groups.begin() }; it != connected_groups.end(); ++it)
        {
            if (it->find(n) != it->end())
                all_found.push_back(it);
        }
        return all_found;
    };

    for (const auto& mini_group : mini_groups)
    {
        std::list<std::set<int>>::iterator found{ connected_groups.end() };
        for (const int n : mini_group)
        {
            auto all_f{ find_all_containing(n) };
            if (!all_f.empty())
            {
                for (auto found_it{ std::next(all_f.begin()) }; found_it != all_f.end();)
                {
                    std::copy((*found_it)->begin(), (*found_it)->end(), std::inserter(*all_f.front(), all_f.front()->begin()));
                    connected_groups.erase(*(found_it++));
                }
            }
        }

        auto all_f{ find_all_containing(mini_group.front()) };
        if (!all_f.empty())
            found = all_f.front();

        if (found == connected_groups.end())
        {
            connected_groups.push_back(std::set<int>{});
            found = std::prev(connected_groups.end());
        }

        for (const int n : mini_group)
        {
            found->insert(n);
        }
    }
    
    std::cout << "Total number of groups: " << connected_groups.size() << std::endl;

    auto all_found{ find_all_containing(id) };
        if (all_found.size() > 0)
            return (*all_found.begin())->size();
        else
            return 0;
}

void trim_comma(std::string& str)
{
    if (!str.empty() && str.back() == ',')
        str = str.substr(0, str.size() - 1);
}

bool is_number(const std::string& str)
{
    for (const char c : str)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

int test(const std::string& filename)
{
    std::ifstream input_file{ filename };
       
    std::vector<std::vector<int>> mini_groups;
    for (std::string line; std::getline(input_file, line);)
    {
        std::vector<int> mini_group;
        std::stringstream ss{ line };
        for (std::string word; ss >> word;)
        {
            trim_comma(word);
            if (is_number(word))
                mini_group.push_back(std::stoi(word));
        }
        mini_groups.push_back(mini_group);
    }

    return find_connected_to(mini_groups, 0);
}

int main()
{
    std::cout << "IDs within the group containg 0: " << test("input.txt");
    system("PAUSE > nul");
}