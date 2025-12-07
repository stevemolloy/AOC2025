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
using std::vector;

vector<std::string> read_file(std::string filename) {
    std::ifstream file_stream(filename);
    vector<std::string> file_contents;
    std::string line;
    while (std::getline(file_stream, line))
        file_contents.push_back(line);
    return file_contents;
}

int main(void) {
    // std::string filename = "data/test.txt";
    std::string filename = "data/input.txt";

    vector<std::string> file_contents = read_file(filename);

    println("------------------------------");
    for (auto line: file_contents)
        println("{}", line);
    println("------------------------------");

    size_t split_count = 0;
    for (size_t i=1; i<file_contents.size(); i++) {
        for (const auto [j, c]: std::ranges::views::enumerate(file_contents[i])) {
            if (c=='.' && file_contents[i-1][j]=='S') {
                file_contents[i][j] = '|';
            } else if (c=='^' && file_contents[i-1][j]=='|') {
                if ((int)j>0) file_contents[i][j-1] = '|';
                if ((int)j<(int)file_contents[i].size()-1) file_contents[i][j+1] = '|';
                split_count += 1;
            } else if (c=='.' && file_contents[i-1][j]=='|') {
                file_contents[i][j] = '|';
            }
        }
    }

    println("------------------------------");
    for (auto line: file_contents)
        println("{}", line);
    println("------------------------------");

    size_t part1 = split_count;
    if (filename == "data/input.txt") assert(part1 == 1717);
    ulong part2 = 0;

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

