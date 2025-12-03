#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include <print>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

int main(void) {
    std::string data_dir = "data/";
    // std::ifstream input_data(data_dir + "test.txt");
    std::ifstream input_data(data_dir + "input.txt");

    unsigned long int part1 = 0;
    unsigned long int part2 = 0;

    std::string data_line;
    while (std::getline(input_data, data_line)) {
        std::vector<unsigned long int> vals;
        for (char c: data_line) {
            if (c == '\n') 
                continue;
            vals.push_back(c - '0');
        }

        assert(vals.size() == 100);

        auto b1 = std::max_element(vals.begin(), vals.end()-1);
        auto b2 = std::max_element(b1+1, vals.end());

        part1 += *b1*10 + *b2;
    }

    std::println("Part 1: {}", part1);
    std::println("Part 2: {}", part2);

    return 0;
}
