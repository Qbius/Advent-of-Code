#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int resolve_list(std::vector<int>& instructions)
{
    
    auto it{ instructions.begin() };
    for (int count = 0;; ++count)
    {
        const int jump{ *it };
        
        if (jump >= 3)
            --*it;
        else
            ++*it;

        if ((jump < 0) ? std::distance(it, instructions.begin()) > jump : std::distance(it, instructions.end()) <= jump)
            return count + 1;
        else
            std::advance(it, jump);
    }
}

int get_instructions(const std::string& filename)
{
    std::ifstream input_file{ filename };
    
    std::vector<int> instructions;
    if (input_file.is_open())
    {
        for (std::string line; std::getline(input_file, line);)
        {
            instructions.push_back(std::stoi(line));
        }
    }

    return resolve_list(instructions);
}

int main()
{
    std::cout << get_instructions("input.txt");
    system("PAUSE > nul");
}