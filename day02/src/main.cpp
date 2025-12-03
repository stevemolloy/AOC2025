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

    unsigned long int part2 = 0;

    std::regex nums_regex("^(\\d+)\\1+$");

    std::string data_line;
    std::vector<unsigned long int> vals;
    while (std::getline(input_data, data_line, ',')) {
        std::println("{}", data_line);
        char dash;
        unsigned long int lim_low = 0, lim_hi = 0;
        std::stringstream range_ss(data_line);
        range_ss >> lim_low >> dash >> lim_hi;

        for (unsigned long int i=lim_low; i<=lim_hi; i++) {
            std::string test_str = std::to_string(i);

            if (std::regex_match(test_str.begin(), test_str.end(), nums_regex)) {
                vals.push_back(i);
            }
        }
    }

    unsigned long int part1 = 0;
    for (auto val: vals) {
        part1 += val;
    }

    std::println("Part 1: {}", part1);
    std::println("Part 2: {}", part2);

    return 0;
}
