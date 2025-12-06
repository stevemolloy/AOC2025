#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <sstream>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::print;
using std::vector;

int main(void) {
    // std::ifstream input_data("data/test.txt");
    std::ifstream input_data("data/input.txt");

    std::string data_line;
    vector<vector<ulong>> vals;
    std::vector<char> functors;
    bool first_iter = true;
    while (std::getline(input_data, data_line)) {
        println("{}", data_line);

        size_t counter = 0;
        ulong val;

        size_t start=0, end=0;
        char dl = ' ';
        while ((start = data_line.find_first_not_of(dl, end)) != std::string::npos) {
            end = data_line.find(dl, start);

            if (data_line[start] != '*' && data_line[start] != '+') {
                val = (ulong)std::atoi(data_line.substr(start, end - start).c_str());
            } else if (data_line[start] == '+') {
                functors.push_back('+');
                continue;
            } else if (data_line[start] == '*') {
                functors.push_back('*');
                continue;
            } else {
                println("ERROR: Unknown character '{}'", data_line[start]);
                exit(1);
            }

            if (first_iter) {
                vector<ulong> col;
                col.push_back(val);
                vals.push_back(std::move(col));
            } else {
                vals[counter].push_back(val);
                counter += 1;
            }
        }

        first_iter = false;
    }

    ulong part1 = 0;
    for (size_t i=0; i<functors.size(); i++) {
        ulong result;
        if (functors[i] == '+')
            result = std::ranges::fold_left(vals[i], 0, std::plus<ulong>());
        else if (functors[i] == '*')
            result = std::ranges::fold_left(vals[i], 1, std::multiplies<ulong>());
        part1 += result;
        println("{}", result);
    }
    assert(part1 == 6169101504608);

    ulong part2 = 0;
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

