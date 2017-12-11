#include <iostream>
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

namespace
{
    bool is_number(const std::string& str)
    {
        for (auto it{ str.begin() }; it != str.end(); ++it)
        {
            if (!isdigit(*it) && !(*it == '-' && it == str.begin()))
                return false;
        }
        return true;
    }
}

class バカ
{
    std::unordered_map<std::string, int> registers;
    int main_register{ 0 };
    std::unordered_map<std::string, int> numbers;

    std::vector<std::string> arguments;
    std::vector<std::function<int()>> commands;

    void reset_ram()
    {
        arguments.clear();
        commands.clear();
    }

    int& last_argument()
    {
        const std::string last{ arguments.back() };
        arguments.pop_back();
        if (is_number(last))
        {
            if (numbers.find(last) == numbers.end()) numbers.insert(std::make_pair(last, std::stoi(last)));
            return numbers.find(last)->second;
        }
        else
        {
            if (registers.find(last) == registers.end()) registers.insert(std::make_pair(last, 0));
            return registers.find(last)->second;
        }
    }

    std::map<std::string, std::function<int()>> keywords = {
            
        { "inc", [this]() -> int {
            const int& rhs{ last_argument() };
            int& lhs{ last_argument() };
            lhs += rhs;
            return lhs;
        } },

        { "dec", [this]() -> int {
            const int& rhs{ last_argument() };
            int& lhs{ last_argument() };
            lhs -= rhs;
            return lhs;
        } },

        { "<", [this]() -> int {
            const int& rhs{ last_argument() };
            int& lhs{ last_argument() };
            return lhs < rhs;
        } },

        { "<=", [this]() -> int {
            const int& rhs{ last_argument() };
            int& lhs{ last_argument() };
            return lhs <= rhs;
        } },

        { ">", [this]() -> int {
            const int& rhs{ last_argument() };
            int& lhs{ last_argument() };
            return lhs > rhs;
        } },

        { ">=", [this]() -> int {
            const int& rhs{ last_argument() };
            int& lhs{ last_argument() };
            return lhs >= rhs;
        } },

        { "==", [this]() -> int {
            const int& rhs{ last_argument() };
            int& lhs{ last_argument() };
            return lhs == rhs;
        } },

        { "!=", [this]() -> int {
            const int& rhs{ last_argument() };
            int& lhs{ last_argument() };
            return lhs != rhs;
        } },

        { "if", [this]() -> int {
            if (main_register == 0)
                commands.clear();

            return main_register;
        } }
    };

public:

    int greatest_value()
    {
        int max{ std::numeric_limits<int>::min() };
        for (const auto& rgstr : registers)
        {
            if (rgstr.second > max)
                max = rgstr.second;
        }
        return max;
    }

    void process_line(const std::string& line)
    {
        std::stringstream ss{ line };
        for (std::string token; ss >> token;)
        {
            auto found{ keywords.find(token) };
            if (found != keywords.end())
            {
                commands.push_back(found->second);
            }
            else
            {
                arguments.push_back(token);
            }
        }

        while (!commands.empty())
        {
            auto f{ commands.back() };
            commands.pop_back();
            main_register = f();
        }

        arguments.clear();
    }

    void dump_registers()
    {
        for (const auto& rgstr : registers)
        {
            std::cout << rgstr.first << ": " << rgstr.second << std::endl;
        }
    }
};

int test(const std::string& filename)
{
    バカ interpreter;

    int max{ std::numeric_limits<int>::min() };

    std::ifstream input_file{ filename };
    for (std::string line; std::getline(input_file, line);)
    {
        interpreter.process_line(line);
        if (interpreter.greatest_value() > max)
            max = interpreter.greatest_value();
    }

    return max;
}

int main()
{
    std::cout << test("input.txt");
    system("PAUSE > nul");
}
