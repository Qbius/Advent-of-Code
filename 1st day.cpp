#include <iostream>
#include <string>

int consecutive_digit_sum(const std::string& str)
{
    int sum{ 0 };

    if (!str.empty())
    {
        char prev = str.back();
        for (char c : str)
        {
            if (c == prev)
                sum += c - '0';
            prev = c;
        }
    }

    return sum;
}

int consecutive_digit_sum(const std::string& str, int offset)
{
    int sum{ 0 };

    if (!str.empty())
    {
        auto 
            it{ str.begin() }, 
            it_offset{ str.begin() };
        std::advance(it_offset, offset);

        for (; it != str.end(); ++it, ++it_offset)
        {
            if (it_offset == str.end())
                it_offset = str.begin();

            if (*it == *it_offset)
                sum += *it - '0';
        }
    }

    return sum;
}

void test()
{
    std::string input;
    std::cin >> input;
    std::cout << consecutive_digit_sum(input, input.size() / 2) << '\n' << std::endl;
}

int main()
{
    for (;;) test();
}