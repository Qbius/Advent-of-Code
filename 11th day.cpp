#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

int to_origin(const std::pair<float, float> point)
{
    if (std::abs(point.first / 2) >= std::abs(point.second))
        return std::abs(point.first);
    else
        return std::abs(point.first) + std::abs(std::abs(point.second) - (std::abs(point.first) / 2));
}

int minimal_steps_back(const std::vector<std::string>& path)
{
    std::map<std::string, std::pair<float, float>> step_values{
        { "n", { 0, 1 } },
        { "s", { 0, -1 } },
        { "ne", { 1, 0.5 } },
        { "nw", { -1, 0.5 } },
        { "se", { 1, -0.5 } },
        { "sw", { -1, -0.5 } }
    };

    std::pair<float, float> point{ 0, 0 };
    int max{ 0 };
    for (const auto& step : path)
    {
        const auto& value{ step_values[step] };
        point.first += value.first;
        point.second += value.second;
    
        const int distance{ to_origin(point) };
        if (distance > max)
            max = distance;
    }

    return max;
}

int test(const std::string& filename)
{
    std::ifstream input_file{ filename };
    std::string line;
    std::getline(input_file, line);

    std::stringstream ss{ line };
    std::vector<std::string> path;
    for (std::string step; ss >> step;)
    {
        path.push_back(step);
    }

    return minimal_steps_back(path);
}

int main()
{
    std::cout << test("input.txt") << std::endl;
    system("PAUSE > nul");
}