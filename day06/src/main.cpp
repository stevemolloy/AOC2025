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

typedef struct {
    vector<vector<ulong>> vals;
    std::vector<char> functors;
} ParsedData;

ParsedData parse_data_part1(std::string filename);
ulong solve_part1(ParsedData parsed_data);
std::string get_last_line(std::ifstream& in);

int main(void) {
    // std::string filename = "data/test.txt";
    // std::string filename = "data/test2.txt";
    std::string filename = "data/input.txt";

    ParsedData parsed_data = parse_data_part1(filename);
    ulong part1 = solve_part1(parsed_data);
    if (filename == "data/input.txt") assert(part1 == 6169101504608);

    std::ifstream input_data(filename);
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
    std::string data_line;
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
    std::reverse(parsed_data.functors.begin(), parsed_data.functors.end());

    ulong part2 = 0;
    size_t index = 0;
    for (size_t i=0; i<lengths.size(); i++) {
        part2 += std::ranges::fold_left(
                part2vals.begin()+index,
                part2vals.begin()+index+lengths[i],
                parsed_data.functors[i]=='+' ? 0 : 1,
                parsed_data.functors[i]=='+' ? (BinFunc)std::plus<ulong>() : (BinFunc)std::multiplies<ulong>()
            );
        index += lengths[i] + 1;
    }

    if (filename == "data/input.txt") assert(part2 == 10442199710797);

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

ParsedData parse_data_part1(std::string filename) {
    ParsedData result;
    std::ifstream input_data(filename);
    std::string data_line;
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
                result.functors.push_back('+');
                continue;
            } else if (data_line[start] == '*') {
                result.functors.push_back('*');
                continue;
            } else {
                println("ERROR: Unknown character '{}'", data_line[start]);
                exit(1);
            }

            if (first_iter) {
                vector<ulong> col;
                col.push_back(val);
                result.vals.push_back(std::move(col));
            } else {
                result.vals[counter].push_back(val);
                counter += 1;
            }
        }

        first_iter = false;
    }
    input_data.close();

    return result;
}

ulong solve_part1(ParsedData parsed_data) {
    ulong res = 0;
    for (size_t i=0; i<parsed_data.functors.size(); i++) {
        ulong result;
        if (parsed_data.functors[i] == '+') result = std::ranges::fold_left(parsed_data.vals[i], 0, std::plus<ulong>());
        else if (parsed_data.functors[i] == '*') result = std::ranges::fold_left(parsed_data.vals[i], 1, std::multiplies<ulong>());
        res += result;
    }
    return res;
}

std::string get_last_line(std::ifstream& in) {
    std::string line;
    while (in >> std::ws && std::getline(in, line));
    return line;
}


