#include <print>
#include <fstream>

constexpr std::string data_dir = "data/";

int main(void) {
    // std::ifstream input_data(data_dir + "test.txt");
    std::ifstream input_data(data_dir + "input.txt");

    int move, dir;
    int val = 50;
    int part1 = 0, part2 = 0;

    std::string data_line;
    while (std::getline(input_data, data_line)) {
        std::string_view sv(data_line);

        if (sv[0] == 'L')      dir = -1;
        else if (sv[0] == 'R') dir = 1;
        else {
            std::println("Character {} was unexpected!", sv[0]);
            return 1;
        }

        sv.remove_prefix(1);
        std::from_chars(sv.data(), sv.data()+sv.length(), move);

        while (move > 0) {
            val = (val + dir) % 100;
            if (val == 0) part2 += 1;
            move -= 1;
        }

        if (val == 0) part1 += 1;
    }

    std::println("Part 1: {}", part1);
    std::println("Part 2: {}", part2);

    return 0;
}
