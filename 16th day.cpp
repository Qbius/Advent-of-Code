#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <deque>

template <int DEPTH>
std::string resolve_moves(const std::string& base, const std::vector<std::string>& moves)
{
    std::deque<std::string> lineups;

    std::string lineup{ base };
    for (int i{ 0 }; i < DEPTH; ++i)
    {
        for (const auto& move : moves)
        {
            const std::string specs{ move.substr(1, std::string::npos) };

            switch (move.front())
            {
                case 's':
                {
                    for (int i{ 0 }; i < std::stoi(specs); ++i)
                    {
                        lineup = lineup.back() + lineup;
                        lineup.pop_back();
                    }
                }
                break;

                case 'x':
                {
                    const int posA{ std::stoi(specs.substr(0, specs.find('/'))) };
                    const int posB{ std::stoi(specs.substr(specs.find('/') + 1, std::string::npos)) };

                    std::iter_swap(std::next(lineup.begin(), posA), std::next(lineup.begin(), posB));
                }
                break;

                case 'p':
                {
                    const char nameA{ specs.substr(0, specs.find('/')).front() };
                    const char nameB{ specs.substr(specs.find('/') + 1, std::string::npos).front() };

                    std::iter_swap(std::find(lineup.begin(), lineup.end(), nameA), std::find(lineup.begin(), lineup.end(), nameB));
                }
                break;

                default:
                break;
            }
        }

        auto found{ std::find(lineups.begin(), lineups.end(), lineup) };
        if (found != lineups.end())
        {
            while (found != lineups.begin())
            {
                lineups.pop_front();
                auto found{ std::find(lineups.begin(), lineups.end(), lineup) };
            }

            const auto move{ (DEPTH - i - 1) % lineups.size() };
            return *std::next(found, move);
        }
        else
            lineups.push_back(lineup);
    }

    return lineup;
}

std::vector<std::string> dance_moves(const std::string& filename)
{
    std::ifstream input_file{ filename };

    std::string line; 
    std::getline(input_file, line);

    std::vector<std::string> moves;
    for (size_t from{ 0 }, to{ 0 }; to != std::string::npos; from = to + 1)
    {
        to = line.find(',', from);
        moves.push_back(line.substr(from, to - from));
    }
    return moves;
}

int main()
{
    std::cout << resolve_moves<1000000000>("abcdefghijklmnop", dance_moves("input.txt"));
    system("PAUSE > nul");
}