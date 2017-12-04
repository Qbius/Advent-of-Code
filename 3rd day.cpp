#include <numeric>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

int ulam_manhattan_distance(int n)
{
    if (n > 1)
    {
        int outer_ring_base{ static_cast<int>(std::ceil(std::sqrt(n))) };
        outer_ring_base += (outer_ring_base % 2) ? 0 : 1;
        int outer_ring_wall_length{ outer_ring_base - 1 };
        int lowest_ring_middle{ static_cast<int>(std::pow(outer_ring_base, 2) - 3.5 * outer_ring_wall_length) };
        return (std::abs(n - lowest_ring_middle) % outer_ring_wall_length) + outer_ring_wall_length / 2;
    }
    else
        return 0;
}

int first_larger_ulam_adjacent_sum(int limit)
{
    std::vector<int> sums{ 1, 1, 2, 4, 5, 10, 11, 23, 25 };
    
    int next_threetile{ 9 };
    int threetile_diff{ 3 };

    bool second_half{ true };
    auto get_facing = [&threetile_diff, &second_half](int tile_number) {
        tile_number -= threetile_diff;
        tile_number -= threetile_diff - (second_half ? 0 : 1);
        tile_number -= threetile_diff - 1;
        tile_number -= threetile_diff - (second_half ? 1 : 2);
        tile_number -= 1;
        return tile_number;
    };

    for (int i = 10; sums.back() <= limit; ++i)
    {
        sums.push_back(sums.back());
        
        switch (i - next_threetile)
        {
            // threetile
            case 0:
            {
                sums.back() += sums[get_facing(i) - 1];
                sums.back() += sums[get_facing(i) - 2];

                if (std::floor(std::sqrt(i)) == std::sqrt(i))
                {
                    ++threetile_diff;
                }
            }
            break;

            // twotile
            case 1:
            {
                second_half = !second_half;
                sums.back() += sums[get_facing(i)];
            }
            break;

            // first fourtile
            case 2:
            {
                sums.back() += sums[sums.size() - 3];
                sums.back() += sums[get_facing(i) - 1];
                sums.back() += sums[get_facing(i)];

                next_threetile += threetile_diff;

                const double next_threetile_test{ std::sqrt(next_threetile + 1) };
                if (std::floor(next_threetile_test) == next_threetile_test)
                    ++next_threetile;
            }
            break;

            // fourtile
            default:
            {
                sums.back() += sums[get_facing(i) - 2];
                sums.back() += sums[get_facing(i) - 1];
                sums.back() += sums[get_facing(i)];
            }
            break;
        }
    }

    return sums.back();
}

void test()
{
    int x;
    std::cin >> x;
    std::cout << first_larger_ulam_adjacent_sum(x) << '\n' << std::endl;
}

int main()
{
    for (;;) test();
}