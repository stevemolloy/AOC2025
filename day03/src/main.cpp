#include <cmath>
#include <vector>
#include <algorithm>
#include <print>
#include <fstream>
#include <string>

using std::println;
using std::max_element;

constexpr int BATT_PART2 =  12;

int main(void) {
    // std::ifstream input_data("data/test.txt");
    std::ifstream input_data("data/input.txt");

    unsigned long int part1 = 0;
    unsigned long int part2 = 0;

    std::string data_line;
    while (std::getline(input_data, data_line)) {
        std::vector<unsigned long int> vals;
        for (char c: data_line) {
            if (c == '\n') continue;
            vals.push_back(c - '0');
        }

        auto b1 = max_element(vals.begin(), vals.end()-1);
        auto b2 = max_element(b1+1, vals.end()-0);
        part1 += *b1*10 + *b2;

        auto prev = vals.begin();
        for (int i=BATT_PART2-1; i>=0; i--) {
            auto largest = max_element(prev, vals.end()-i);
            prev = largest + 1;
            part2 += *largest * std::pow(10, i);
        }
    }

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
