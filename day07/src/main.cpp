#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <ranges>
#include <string>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::print;
using std::vector;

vector<std::string> read_file(std::string filename) {
    std::ifstream file_stream(filename);
    vector<std::string> file_contents;
    std::string line;
    while (std::getline(file_stream, line))
        file_contents.push_back(line);
    return file_contents;
}

void print_num_grid(vector<vector<long>> num_grid) {
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

int main(void) {
    // std::string filename = "data/test.txt";
    std::string filename = "data/input.txt";

    vector<std::string> file_contents = read_file(filename);
    vector<std::string> working_grid(file_contents);

    println("------------------------------");
    for (auto line: working_grid)
        println("{}", line);
    println("------------------------------");

    size_t split_count = 0;
    for (size_t i=1; i<working_grid.size(); i++) {
        for (const auto [j, c]: std::ranges::views::enumerate(working_grid[i])) {
            if (c=='.' && working_grid[i-1][j]=='S') {
                working_grid[i][j] = '|';
            } else if (c=='^' && working_grid[i-1][j]=='|') {
                if ((int)j>0) working_grid[i][j-1] = '|';
                if ((int)j<(int)working_grid[i].size()-1) working_grid[i][j+1] = '|';
                split_count += 1;
            } else if (c=='.' && working_grid[i-1][j]=='|') {
                working_grid[i][j] = '|';
            }
        }
    }
    
    vector<std::string> final_grid(working_grid);

    size_t part1 = split_count;
    if (filename == "data/input.txt") assert(part1 == 1717);

    vector<vector<long>> num_grid;
    for (auto row: final_grid) {
        vector<long> num_row;
        num_row.push_back(0);
        for (char c: row) {
            if (c=='^') num_row.push_back(-1);
            else if (c=='S') num_row.push_back(1);
            else num_row.push_back(0);
        }
        num_row.push_back(0);
        num_grid.push_back(num_row);
    }

    print_num_grid(num_grid);

    for (size_t row_ind=1; row_ind<num_grid.size(); row_ind++) {
        vector<long> this_row = num_grid[row_ind];
        vector<long> prev_row = num_grid[row_ind-1];
        for (size_t i=0; i<this_row.size(); i++) {
            if (prev_row[i] > 0 && this_row[i] >= 0) {
                num_grid[row_ind][i] += prev_row[i];
            } else if (prev_row[i] > 0 && this_row[i]<0) {
                num_grid[row_ind][i-1] += prev_row[i];
                num_grid[row_ind][i+1] += prev_row[i];
            }
        }
    }
    print_num_grid(num_grid);

    long part2 = 0;
    vector<long> last_row = num_grid.back();
    for (size_t i=0; i<last_row.size(); i++) {
        long val = last_row[i];
        part2 += val>=0 ? val : 0;
    }

    if (filename == "data/input.txt") assert(part2 == 231507396180012);

    println("Part 1: {}", part1);
    // 40805746741 is too low
    // 18446744073709549817 is too high
    println("Part 2: {}", part2);

    return 0;
}

