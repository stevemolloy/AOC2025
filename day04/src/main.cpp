#include <cstddef>
#include <ranges>
#include <vector>
#include <print>
#include <fstream>
#include <string>

using std::println;
using std::print;
using std::vector;
using std::ranges::views::enumerate;

template<typename T>
using Vector2D = vector<vector<T>>;

void dump_layout(Vector2D<int> layout);
int count_neighbours(const Vector2D<int> &layout, size_t row, size_t col);
int get_nbor_count(Vector2D<int> &layout);

constexpr size_t arr_reserve = 256;

int main(void) {
    // std::ifstream input_data("data/test.txt");
    std::ifstream input_data("data/input.txt");

    std::string data_line;
    Vector2D<int> layout;
    layout.reserve(arr_reserve);
    while (std::getline(input_data, data_line)) {
        vector<int> row;
        row.reserve(arr_reserve);
        for (char c: data_line) {
            if (c=='@')      row.push_back(1);
            else if (c=='.') row.push_back(0);
            else {
                println("Received '{}', expecting '@' or '.'", c);
                return 1;
            }
        }
        layout.push_back(row);
    }

    int change = get_nbor_count(layout);
    int part1 = change;

    int part2 = 0;
    while (change>0) {
        part2 += change;
        change = get_nbor_count(layout);
    }

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

void dump_layout(Vector2D<int> layout) {
    for (auto row: layout) {
        for (auto cell: row) print("{}", cell);
        println("");
    }
}

int count_neighbours(const Vector2D<int> &layout, size_t row, size_t col) {
    int sum = 0;
    if (layout[row][col] == 0) return sum;

    size_t row_len = layout.size();
    size_t col_len = layout[row].size();

    if (row > 0)                            sum += layout[row-1][col];
    if (row < row_len-1)                    sum += layout[row+1][col];
    if (col > 0)                            sum += layout[row][col-1];
    if (col < col_len-1)                    sum += layout[row][col+1];
    if (row > 0 && col > 0)                 sum += layout[row-1][col-1];
    if (row > 0 && col < col_len-1)         sum += layout[row-1][col+1];
    if (row < row_len-1 && col > 0)         sum += layout[row+1][col-1];
    if (row < row_len-1 && col < col_len-1) sum += layout[row+1][col+1];

    return sum;
}

int get_nbor_count(Vector2D<int> &layout) {
    Vector2D<int> copy_layout = layout;
    size_t sum = 0;
    for (const auto [r, row]: enumerate(layout)) {
        for (const auto [c, cell]: enumerate(row)) {
            if (cell != 1) continue;
            if (count_neighbours(layout, r, c) < 4) {
                sum += 1;
                copy_layout[r][c] = 0;
            }
        }
    }
    layout = std::move(copy_layout);
    return sum;
}

