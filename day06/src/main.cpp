#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;

using BinFunc = std::function<ulong(ulong, ulong)>;

std::string get_last_line(std::ifstream& in) {
    std::string line;
    while (in >> std::ws && std::getline(in, line));
    return line;
}

int main(void) {
    // std::string filename = "data/test.txt";
    // std::string filename = "data/test2.txt";
    std::string filename = "data/input.txt";

    std::ifstream input_data(filename);

    std::string data_line;
    vector<vector<ulong>> vals;
    std::vector<char> functors;
    bool first_iter = true;
    while (std::getline(input_data, data_line)) {
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
    input_data.close();

    ulong part1 = 0;
    for (size_t i=0; i<functors.size(); i++) {
        ulong result;
        if (functors[i] == '+') result = std::ranges::fold_left(vals[i], 0, std::plus<ulong>());
        else if (functors[i] == '*') result = std::ranges::fold_left(vals[i], 1, std::multiplies<ulong>());
        part1 += result;
    }

    if (filename == "data/input.txt") assert(part1 == 6169101504608);

    input_data.open(filename);
    std::string last_line = get_last_line(input_data);
    std::reverse(last_line.begin(), last_line.end());
    input_data.close();

    size_t spc_counter = 1;  // Offset at the start due to boundary conds
    vector<size_t> lengths;
    for (char c : last_line) {
        if (c == ' ') {
            spc_counter += 1;
        } else if (c=='*' || c=='+') {
            lengths.push_back(spc_counter);
            spc_counter = 0;
        } else {
            println("ERROR: Got unexpected character: '{}'", c);
            exit(1);
        }
    }

    input_data.open(filename);
    vector<vector<ulong>> part2digits;
    while (std::getline(input_data, data_line)) {
        std::reverse(data_line.begin(), data_line.end());

        vector<ulong> row_digits;
        for (char c : data_line) {
            if (c == ' ') row_digits.push_back(0);
            else row_digits.push_back(c - '0');
        }
        part2digits.push_back(row_digits);
    }
    input_data.close();
    part2digits.pop_back();

    vector<ulong> part2vals;
    for (size_t i=0; i<part2digits[0].size(); i++) {
        ulong val = 0;
        for (size_t j=0; j<part2digits.size(); j++) {
            if (part2digits[j][i] ==0) continue;  // Safe since input data contains no zeros
            val = val*10 + part2digits[j][i];
        }
        part2vals.push_back(val);
    }
    std::reverse(functors.begin(), functors.end());

    ulong part2 = 0;
    size_t index = 0;
    for (size_t i=0; i<lengths.size(); i++) {
        part2 += std::ranges::fold_left(
                part2vals.begin()+index,
                part2vals.begin()+index+lengths[i],
                functors[i]=='+' ? 0 : 1,
                functors[i]=='+' ? (BinFunc)std::plus<ulong>() : (BinFunc)std::multiplies<ulong>()
            );
        index += lengths[i] + 1;
    }

    if (filename == "data/input.txt") assert(part2 == 10442199710797);

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

