#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <string>
#include <numeric>
#include <sstream>
#include <vector>
#include <algorithm>

std::map<std::string, int> weights;

int weight(const std::string&);

class node_cluster
{
    std::vector<std::string> nodes;

    friend bool operator<(const node_cluster& lhs, const node_cluster& rhs);

public:

    mutable bool checked{ false };

    node_cluster(const std::vector<std::string>& nodes) :
        nodes(nodes)
    {
    }

    bool contains(const std::string& node) const
    {
        return std::find(nodes.begin(), nodes.end(), node) != nodes.end();
    }

    bool check_weights(const std::string& to) const
    {
        checked = true;

        std::vector<int> wghs;
        for (const auto& node : nodes)
        {
            wghs.push_back(weight(node));
        }

        return std::find_if(std::next(wghs.begin()), wghs.end(), [&wghs](const int e) -> bool { return e != wghs.front(); }) == wghs.end();
    }

    int odd_one_proper() const
    {
        std::vector<int> wghs;
        for (const auto& node : nodes)
        {
            wghs.push_back(weight(node));
        }

        auto odd_weight{ std::find_if(std::next(wghs.begin()), wghs.end(), [&wghs](const int e) -> bool { return e != wghs.front(); }) };
        auto odd_node{ std::next(nodes.begin(), std::distance(wghs.begin(), odd_weight)) };

        int mode_weight{ wghs.front() };
        for (auto it{ std::next(wghs.begin()) }; it != wghs.end(); ++it)
        {
            if (mode_weight == *it)
                break;
            else
                mode_weight = *it;
        }

        return weights[*odd_node] + mode_weight - *odd_weight;
    }

    int total_weight() const
    {
        int w{ 0 };
        for (const auto& node : nodes)
        {
            w += weight(node);
        }
        return w;
    }
};


std::map<node_cluster, std::string> dependencies;
std::set<std::string> surface;

std::map<node_cluster, std::string>::const_iterator find_dependency(const std::string& of)
{
    auto it{ dependencies.begin() };
    for (; it != dependencies.end(); ++it)
    {
        if (it->second == of)
            break;
    }
    return it;
}

int weight(const std::string& node)
{
    int w{ weights[node] };

    auto found{ find_dependency(node) };
    if (found != dependencies.end())
    {
        w += found->first.total_weight();
    }

    return w;
}

bool operator<(const node_cluster& lhs, const node_cluster& rhs)
{
    return lhs.nodes < rhs.nodes;
}

int check_surface()
{
    while (surface.size() > 1)
    {
        std::set<std::string> new_surface;
        for (const auto& node : surface)
        {
            for (const auto& dependency : dependencies)
            {
                if (dependency.first.contains(node) && !dependency.first.checked)
                {
                    if (!dependency.first.check_weights(dependency.second))
                        return dependency.first.odd_one_proper();
                    new_surface.insert(dependency.second);
                }
            }
        }
        std::swap(surface, new_surface);
    }

    return 0;
}

int test(const std::string& filename)
{
    const auto trim_comma = [](const std::string& str) {
        return (str.find(',') != std::string::npos) ? str.substr(0, str.size() - 1) : str;
    };

    std::ifstream input_file{ filename };
    for (std::string line; std::getline(input_file, line);)
    {
        const std::string main{ trim_comma(line.substr(0, line.find(' '))) };
        const std::string weight{ line.substr(line.find(' ') + 1, 4) };

        weights.insert(std::make_pair(main, std::stoi(weight.substr(1, weight.find(')') - 1))));

        if (line.find('>') != std::string::npos)
        {
            std::stringstream ss{ line.substr(line.find('>') + 2, std::string::npos) };
            std::vector<std::string> dependent;

            for (std::string word; ss >> word;)
            {
                dependent.push_back(trim_comma(word));
            }

            dependencies.insert(std::make_pair(node_cluster{ dependent }, main));
        }
        else
        {
            surface.insert(main);
        }
    }

    const int
        test1{ weight("vnrmx") },
        test2{ weight("lrihy") },
        test3{ weight("arqoys") },
        test4{ weight("nsmoh") };

    return check_surface();
}

int main()
{
    std::cout << test("input.txt");
    system("PAUSE > nul");
}