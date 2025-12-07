#include <cassert>
#include <cstdlib>
#include <ranges>
#include <string>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::print;
using std::vector;
using std::string;

vector<string> read_file(const string &filename);
void print_num_grid(const vector<vector<long>> &num_grid);
size_t solve_part1(vector<string> &working_grid);
long sum_paths(const vector<vector<long>> &num_grid);
vector<vector<long>> make_num_grid(const vector<string> &working_grid);
void draw_paths(vector<vector<long>> &num_grid);

int main(void) {
    // string filename = "data/test.txt";
    string filename = "data/input.txt";

    vector<string> file_contents = read_file(filename);
    vector<string> working_grid(file_contents);

    size_t part1 = solve_part1(working_grid);
    if (filename == "data/input.txt") assert(part1 == 1717);

    vector<vector<long>> num_grid = make_num_grid(working_grid);
    draw_paths(num_grid);
    long part2 = sum_paths(num_grid);
    if (filename == "data/input.txt") assert(part2 == 231507396180012);

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

vector<string> read_file(const string &filename) {
    std::ifstream file_stream(filename);
    vector<string> file_contents;
    string line;
    while (std::getline(file_stream, line))
        file_contents.push_back(std::move(line));
    return file_contents;
}

void print_num_grid(const vector<vector<long>> &num_grid) {
    println("------------------------------");
    for (auto row: num_grid) {
        for (long v: row) {
            if (v==0) print("{}", '.');
            else if (v==-1) print("{}", '^');
            else print("{}", (char)(v+'0'));
        }
        println("");
    }
    println("------------------------------");

}

size_t solve_part1(vector<string> &working_grid) {
    size_t part1 = 0;
    size_t max_i = working_grid.size();
    for (size_t i=1; i<max_i; i++) {
        const auto& prev_row = working_grid[i-1];
        auto& curr_row = working_grid[i];
        for (const auto [j, c]: std::ranges::views::enumerate(curr_row)) {
            char p = prev_row[j];
            if (c=='.' && p=='S') {
                curr_row[j] = '|';
            } else if (c=='^' && p=='|') {
                if ((int)j>0) curr_row[j-1] = '|';
                if ((int)j<(int)curr_row.size()-1) curr_row[j+1] = '|';
                part1 += 1;
            } else if (c=='.' && p=='|') {
                curr_row[j] = '|';
            }
        }
    }
    return part1;
}

long sum_paths(const vector<vector<long>> &num_grid) {
    long part2 = 0;
    vector<long> last_row = num_grid.back();
    for (size_t i=0; i<last_row.size(); i++) {
        long val = last_row[i];
        part2 += val>=0 ? val : 0;
    }
    return part2;
}

vector<vector<long>> make_num_grid(const vector<string> &working_grid) {
    vector<vector<long>> num_grid;
    for (auto row: working_grid) {
        vector<long> num_row;
        for (char c: row) {
            if (c=='^')      num_row.push_back(-1);
            else if (c=='S') num_row.push_back(1);
            else             num_row.push_back(0);
        }
        num_grid.push_back(std::move(num_row));
    }
    return num_grid;
}

void draw_paths(vector<vector<long>> &num_grid) {
    size_t max_row_ind = num_grid.size();
    for (size_t row_ind=1; row_ind<max_row_ind; row_ind++) {
        size_t max_i = num_grid[row_ind].size();
        for (size_t i=0; i<max_i; i++) {
            if (num_grid[row_ind-1][i] > 0 && num_grid[row_ind][i] >= 0) {
                num_grid[row_ind][i] += num_grid[row_ind-1][i];
            } else if (num_grid[row_ind-1][i] > 0 && num_grid[row_ind][i]<0) {
                num_grid[row_ind][i-1] += num_grid[row_ind-1][i];
                num_grid[row_ind][i+1] += num_grid[row_ind-1][i];
            }
        }
    }
}

