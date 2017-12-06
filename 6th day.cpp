#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>

typedef std::pair<int, int> bank;

bool bank_less(const bank& lhs, const bank& rhs)
{
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second > rhs.second);
}

class memory
{
    std::vector<bank> banks;

    int state_count{ 1 };
    std::map<std::vector<int>, int> states;


    int save_state()
    {
        std::vector<int> state;
        for (const auto& bank : banks)
            state.push_back(bank.first);

        auto found{ states.find(state) };
        states.insert(std::make_pair(state, state_count++));
        return (found == states.end()) ? 0 : state_count - found->second;
    }

public:

    memory(const std::vector<int>& blocks_each_bank)
    {
        int id{ 0 };
        for (const auto blocks_n : blocks_each_bank)
        {
            banks.push_back(std::make_pair(blocks_n, id++));
        }

        save_state();
    }

    int reallocate()
    {
        auto max{ std::max_element(banks.begin(), banks.end(), bank_less) };

        int to_distribute{ 0 };
        std::swap(max->first, to_distribute);
        for (auto it{ std::next(max) }; to_distribute > 0; ++it, --to_distribute)
        {
            if (it == banks.end())
                it = banks.begin();

            ++it->first;
        }

        return save_state();
    }
};

int memory_test()
{
    std::string line;
    std::getline(std::cin, line);

    std::vector<int> blocks_n;
    std::stringstream ss{ line };
    for (std::string word; ss >> word;)
    {
        blocks_n.push_back(std::stoi(word));
    }

    memory mem{ blocks_n };

    int count;
    while ((count = mem.reallocate()) == 0);
    return count - 1;
}

int main()
{
    std::cout << memory_test();
    system("PAUSE > nul");
}