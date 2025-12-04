#include <cmath>
#include <cstddef>
#include <ranges>
#include <vector>
#include <algorithm>
#include <print>
#include <fstream>
#include <string>

using std::println;
using std::print;
using std::vector;
using std::ranges::views::enumerate;

void dump_layout(vector<vector<int>> layout);
int count_neighbours(vector<vector<int>> layout, size_t row, size_t col);
int get_nbor_count(vector<vector<int>> layout);

int main(void) {
    // std::ifstream input_data("data/test.txt");
    std::ifstream input_data("data/input.txt");

    unsigned long int part2 = 0;

    std::string data_line;
    vector<vector<int>> layout;
    while (std::getline(input_data, data_line)) {
        vector<int> row;
        for (char c: data_line) {
            if (c=='@') {
                row.push_back(1);
            } else if (c=='.') {
                row.push_back(0);
            } else {
                println("Received '{}', expecting '@' or '.'", c);
                return 1;
            }
        }
        layout.push_back(row);
    }

    int part1 = get_nbor_count(layout);

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

void dump_layout(vector<vector<int>> layout) {
    for (auto row: layout) {
        for (auto cell: row) print("{}", cell);
        println("");
    }
}

int count_neighbours(vector<vector<int>> layout, size_t row, size_t col) {
    int sum = 0;
    if (layout.at(row).at(col) == 0) return sum;

    size_t row_len = layout.size();
    size_t col_len = layout.at(row).size();

    if (row != 0)                             sum += layout.at(row-1).at(col);
    if (row != row_len-1)                     sum += layout.at(row+1).at(col);
    if (col != 0)                             sum += layout.at(row).at(col-1);
    if (col != col_len-1)                     sum += layout.at(row).at(col+1);
    if (row != 0 && col != 0)                 sum += layout.at(row-1).at(col-1);
    if (row != 0 && col != col_len-1)         sum += layout.at(row-1).at(col+1);
    if (row != row_len-1 && col != 0)         sum += layout.at(row+1).at(col-1);
    if (row != row_len-1 && col != col_len-1) sum += layout.at(row+1).at(col+1);

    return sum;
}

int get_nbor_count(vector<vector<int>> layout) {
    size_t sum = 0;
    for (auto const [r, row]: enumerate(layout)) {
        for (auto const [c, cell]: enumerate(row)) {
            int nbors = count_neighbours(layout, r, c);
            if (nbors < 4 && cell==1) sum += 1;
        }
    }
    return sum;
}

