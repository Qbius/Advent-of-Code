#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <iterator>

bool check_if_unique(const std::string& passphrase)
{
    std::set<std::set<char>> passphrases_letters;

    for (size_t from{ 0 }, to{ 0 }; to != std::string::npos; from = to + 1)
    {
        to = passphrase.find(' ', from + 1);
        
        const std::string next_passphrase{ passphrase.substr(from, to - from) };
        std::set<char> letters;
        std::copy(next_passphrase.begin(), next_passphrase.end(), std::inserter(letters, letters.begin()));

        if (passphrases_letters.find(letters) == passphrases_letters.end())
            passphrases_letters.insert(letters);
        else
            return false;
    }

    return true;
}

int correct_passphrases_count(const std::string& filename)
{
    int count{ 0 };

    std::ifstream input_file{ filename };
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            if (check_if_unique(line)) ++count;
        }
    }

    return count;
}

int main()
{
    std::cout << correct_passphrases_count("input.txt");
    system("PAUSE > nul");
}