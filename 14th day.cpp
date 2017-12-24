#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

namespace
{
    std::string char_to_hex(unsigned char chr)
    {
        auto f = [](unsigned char c) {
            if (c < 10)
                return c + '0';
            else
                return c - 10 + 'a';
        };

        std::string res;
        res += f(chr / 16);
        res += f(chr % 16);

        return res;
    }
}

template <typename TContainer>
class circular_iterator
{
    typename TContainer::iterator it;
    int distance;
    int max;

public:

    circular_iterator(TContainer& con) :
        it(con.begin()),
        distance(0),
        max(con.size())
    {
    }

    void operator+=(int offset)
    {
        const int new_position{ (distance + offset) % max };
        std::advance(it, new_position - distance);
        distance = new_position;
    }

    circular_iterator& operator++()
    {
        *this += 1;
        return *this;
    }

    circular_iterator operator++(int)
    {
        circular_iterator copy{ *this };
        ++*this;
        return copy;
    }

    void operator-=(int offset)
    {
        const int new_position{ ((distance - offset) + (((distance - offset) < 0) ? max : 0)) % max };
        std::advance(it, new_position - distance);
        distance = new_position;
    }

    circular_iterator& operator--()
    {
        *this -= 1;
        return *this;
    }

    circular_iterator operator--(int)
    {
        circular_iterator copy{ *this };
        --*this;
        return copy;
    }

    bool operator==(const circular_iterator<TContainer>& rhs)
    {
        typename TContainer::iterator
            l_copy{ it },
            r_copy{ rhs.it };

        l_copy -= distance;
        r_copy -= rhs.distance;

        return l_copy == r_copy;
    }

    typename TContainer::value_type& operator*()
    {
        return *it;
    }

    typename TContainer::value_type* operator->()
    {
        return &*it;
    }

};

template <int LIST_SIZE>
class wrinkling_list
{
    std::array<int, LIST_SIZE> list;
    circular_iterator<std::array<int, LIST_SIZE>> current_position;
    int skip_size;

public:

    wrinkling_list() :
        current_position(list),
        skip_size(0)
    {
        auto it{ current_position };
        for (int i = 0; i < LIST_SIZE; ++it, ++i)
        {
            *it = i;
        }
    }

    void wrinkle(int length)
    {
        circular_iterator<std::array<int, LIST_SIZE>>
            r_begin{ current_position },
            r_end{ current_position };

        r_end += length - 1;
        for (int i = 0; i < length / 2; ++i, ++r_begin, --r_end)
        {
            int temp = *r_begin;
            *r_begin = *r_end;
            *r_end = temp;
        }

        current_position += length + skip_size;
        ++skip_size;
    }

    int first_two_product()
    {
        return *list.begin() * *std::next(list.begin());
    }

    std::string produce_hash();
};

std::string wrinkling_list<256>::produce_hash()
{
    std::vector<int> xored;

    auto it{ list.begin() };
    for (int i = 0; i < 16; ++i)
    {
        int xord{ 0 };
        for (int j = 0; j < 16; ++j, ++it)
        {
            xord ^= *it;
        }
        xored.push_back(xord);
    }

    std::string hash;
    for (const auto xord : xored)
    {
        hash += char_to_hex(xord);
    }

    return hash;
}

std::string produce_hash(const std::string& input)
{
    wrinkling_list<256> list;

    for (int i = 0; i < 64; ++i)
    {
        for (const auto c : input)
        {
            list.wrinkle(c);
        }

        list.wrinkle(17);
        list.wrinkle(31);
        list.wrinkle(73);
        list.wrinkle(47);
        list.wrinkle(23);
    }

    return list.produce_hash();
}

template <int SIZE>
class defrag_row
{
    std::map<char, std::vector<int>> char2ones{
        { '0',{ 0,0,0,0 } },
        { '1',{ 0,0,0,1 } },
        { '2',{ 0,0,1,0 } },
        { '3',{ 0,0,1,1 } },
        { '4',{ 0,1,0,0 } },
        { '5',{ 0,1,0,1 } },
        { '6',{ 0,1,1,0 } },
        { '7',{ 0,1,1,1 } },
        { '8',{ 1,0,0,0 } },
        { '9',{ 1,0,0,1 } },
        { 'a',{ 1,0,1,0 } },
        { 'b',{ 1,0,1,1 } },
        { 'c',{ 1,1,0,0 } },
        { 'd',{ 1,1,0,1 } },
        { 'e',{ 1,1,1,0 } },
        { 'f',{ 1,1,1,1 } },
    };

    std::array<int, SIZE> row;

public:

    defrag_row(const std::string& hash)
    {
        if (hash.size() != SIZE / 4)
            throw "invalid knot hash";

        int i{ 0 };
        for (const char c : hash)
        {
            for (const auto& value : char2ones[c])
            {
                row[i] = value;
                ++i;
            }
        }
    }

    defrag_row()
    {
    }

    int ones_count() const
    {
        int count{ 0 };
        for (const int value : row)
        {
            if (value == 1)
                ++count;
        }
        return count;
    }

    int mark_regions(int base_index)
    {
        int index{ base_index };
        int prev{ 0 };
        for (int& value : row)
        {
            if (value == 0 && prev != 0)
                ++index;
            else if (value != 0)
                value = index;

            prev = value;
        }
        return index;
    }
};

template <int SIZE>
defrag_row<SIZE> operator&(const defrag_row<SIZE>& row1, const defrag_row<SIZE>& row2)
{
    defrag_row<SIZE> result;
    for (int i = 0; i < SIZE; ++i)
    {
        if (row1[i] != 0 && row2[i] != 0)
            result[i] = row1[i];
    }
    return result;
}

template <int SIZE>
class defrag_table
{
    std::array<defrag_row<SIZE>, SIZE> table;

public:

    defrag_table(const std::string& base)
    {
        int i{ 0 };
        for (auto& row : table)
        {
            row = produce_hash(base + '-' + std::to_string(i));
            ++i;
        }
    }

    int ones_count() const
    {
        int count{ 0 };
        for (const auto& row : table)
        {
            count += row.ones_count();
        }
        return count;
    }

    int region_count()
    {
        std::array<defrag_row<SIZE>, SIZE> copy{ table };

        int index{ 1 };
        for (auto& row : copy)
        {
            index = row.mark_regions(index);
        }

        for (auto it{ copy.begin() }; it != std::prev(copy.end()); ++it)
        {

        }
        return 7;
    }

private:

    bool are_same_region(int row, int x, int y)
    {
        if (row >= SIZE || x >= SIZE || y >= SIZE)
            return false;

        bool res{ true };
        for (int i = std::min(x, y) + 1; i < std::max(x, y); ++i)
        {
            if (table[row][i] == 0)
                res = false;
        }

        if (!res)
        {

        }

        return true;
    }
};

int test(const std::string& hash_key)
{
    defrag_table<128> table{ hash_key };
    return table.region_count();

}

int main()
{
    std::string input;
    std::cin >> input;
    std::cout << test(input);
    system("PAUSE > nul");
}