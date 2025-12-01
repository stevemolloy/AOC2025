#include <charconv>
#include <cstdio>
#include <print>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

constexpr std::string data_dir = "data/";

int main(void) {
    // std::ifstream input_data(data_dir + "test.txt");
    std::ifstream input_data(data_dir + "input.txt");

    std::string data_line;
    int dir;
    int val = 50;
    int move;
    int part1 = 0;
    int part2 = 0;
    while (std::getline(input_data, data_line)) {
        std::string_view sv(data_line);
        if (sv[0] == 'L') {
            sv.remove_prefix(1);
            std::from_chars(sv.data(), sv.data()+sv.length(), move);
            dir = -1;
        } else if (sv[0] == 'R') {
            sv.remove_prefix(1);
            std::from_chars(sv.data(), sv.data()+sv.length(), move);
            dir = 1;
        } else {
            std::cout << sv[0] << std::endl;
            fprintf(stdout, "Oh oh!\n");
            return 1;
        }

        std::print("Move = {:+03d}, ", move*dir);

        while (move > 0) {
            val += dir;
            val %= 100;
            if (val == 0) {
                part2 += 1;
            }
            move -= 1;
        }

        if (val == 0) {
            part1 += 1;
        }

        // 6606 is too low
        // 7738 is too high
        // 7087 is too high
        std::println("val = {:+03d}, part1 = {}, part2 = {}", val, part1, part2);
    }

    return 0;
}
