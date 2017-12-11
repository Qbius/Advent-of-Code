#include <iostream>
#include <string>
#include <fstream>

int main()
{
    std::string input;
    std::getline(std::ifstream{ "input.txt" }, input);

    int depth{ 0 };
    int score{ 0 };
    int garbage_score{ 0 };

    bool within_garbage{ false };
    for (auto it{ input.begin() }; it != input.end(); ++it)
    {
        switch (*it)
        {
            case '{':
            {
                if (!within_garbage)
                    ++depth;
                else
                    ++garbage_score;
            }
            break;

            case '}':
            {
                if (!within_garbage)
                {
                    score += depth;
                    --depth;
                }
                else
                    ++garbage_score;
            }
            break;

            case '<':
            {
                if (within_garbage)
                    ++garbage_score;
                else
                    within_garbage = true;
            }
            break;

            case '>':
            {
                within_garbage = false;
            }
            break;

            case '!':
            {
                ++it;
            }
            break;

            default:
                if (within_garbage)
                    ++garbage_score;
                break;
        }
    }

    std::cout << score << " " << garbage_score;
    system("PAUSE > nul");
}
