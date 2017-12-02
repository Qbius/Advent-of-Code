#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<int> get_numbers(const std::string& str)
{
    std::vector<int> numbers;

    for (size_t from = 0, to;; from = to)
    {
        to = str.find('\t', from + 1);
        numbers.push_back(std::stoi(str.substr(from, to - from)));
        if (to == std::string::npos)
            break;
    }

    return numbers;
}

template <typename TFunctor>
int spreadsheet_sum(const std::string& filename, const TFunctor& f)
{
    int sum{ 0 };

    std::ifstream input_file{ filename };
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            sum += f(get_numbers(line));
        }

    }

    return sum;
}



int max_min_difference(const std::vector<int>& numbers)
{
    return *std::max_element(numbers.begin(), numbers.end()) - *std::min_element(numbers.begin(), numbers.end());
}

int proper_divisors_division(const std::vector<int>& numbers)
{
    for (auto it_l = numbers.begin(); it_l != numbers.end(); ++it_l)
        for (auto it_r = std::next(it_l); it_r != numbers.end(); ++it_r)
            if (std::max(*it_l, *it_r) % std::min(*it_l, *it_r) == 0)
                return std::max(*it_l, *it_r) / std::min(*it_l, *it_r);
}

int main()
{
    std::cout << spreadsheet_sum("input.txt", proper_divisors_division) << std::endl;
    system("PAUSE > nul");
}